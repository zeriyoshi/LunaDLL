
#include <fstream>
#include <iostream>
#include <sstream>

#include "HardcodedGraphicsManager.h"
#include "../Defines.h"
#include "../GlobalFuncs.h"
#include "../SdlMusic/MusicManager.h" //Need to get SMBX's application path
#include "../libs/ini-reader/INIReader.h" //Ini files reader


std::string HardcodedGraphicsManager::root="";

HardcodedGraphicsManager::HardcodedGraphicsManager()
{}


HardcodedGraphicsManager::~HardcodedGraphicsManager()
{}

int HardcodedGraphicsManager::patchGraphics(unsigned int offset_i, const char* filepath)
{
	//Check existing of address
	std::map<unsigned int, int>::iterator it = hardcoded_data_map.find(offset_i);
	if(it == hardcoded_data_map.end())
	{
		return -3;
	}

	//get internal memory size
	int sizeOfMemory = hardcoded_data_map[offset_i]-8;

	std::ifstream graphFile;
    graphFile.open(filepath, std::ios::in|std::ios::binary);
	if(!graphFile.is_open())
		return -2;

	graphFile.seekg(0, graphFile.end);
	std::streamoff length = graphFile.tellg();
	graphFile.seekg(0, graphFile.beg);

	if(length > sizeOfMemory)
	{
		graphFile.close();
		return sizeOfMemory;
	}

	char* buffer = new char[sizeOfMemory];
	for(int i=0;i<sizeOfMemory;i++)
	{
		buffer[i] = 0;
	}
	graphFile.read(buffer, length);
	// The actual patching
	void* addr = FN_OFFSET2ADDR(offset_i);
	memset(addr, 0, sizeOfMemory);
	memcpy(addr, buffer, sizeOfMemory);

	// finalize
	graphFile.close();
	delete buffer;
    return -1;
}

void HardcodedGraphicsManager::loadIniImage(unsigned int hex, unsigned int hex_m, INIReader &ini, std::string sct, std::string value)
{
    std::string imageFile = ini.Get(sct, value, "");
    if(imageFile.empty()) return;
	std::string imageFile_src = imageFile;
    imageFile = root+"graphics\\common\\"+imageFile;

    const char *str = imageFile.c_str();

    //Here we should load file  hexKey - address, imageFile:
    //is a name of image file in the <SMBX>\graphics\common
    patchGraphics(hex, str);

    //if mask no used - abort
    if(hex_m==0) return;

    //get filename of mask
	imageFile = imageFile_src;
    for(int i=imageFile.size()-1; i>0; i--)
        if(imageFile[i]=='.')
        {
            imageFile.insert(i, "m");
            break;
        }

    imageFile = root+"graphics\\common\\"+imageFile;
    const char *str2 = imageFile.c_str();
    patchGraphics(hex_m, str2);
}

void HardcodedGraphicsManager::loadGraphics()
{
	if(hardcoded_data_map.empty())
		buildMap();

    std::wstring smbxPath = getModulePath();
    smbxPath = smbxPath.append(L"\\");
    root = WStr2Str(smbxPath);

    std::string ttscrpath=root+"graphics.ini";
	if( !file_existsX(ttscrpath) ) return;

    INIReader GraphicsINI( ttscrpath );
    if (GraphicsINI.ParseError() < 0)
    {
        MessageBoxA(0, std::string(ttscrpath+"\n\nError of read INI file").c_str(), "Error", 0);
        return;
    }
	
    //Splash
    loadIniImage(0x000ca018, 0 ,GraphicsINI, "splash", "game");
    loadIniImage(0x00032a21, 0 ,GraphicsINI, "splash", "editor");

    //Title
    loadIniImage(0x002f460c, 0x002f3a8d, GraphicsINI, "title", "title");


	/*
	;Note: Mask should be detected automatically (file with m suffix like other stuff)
	[splash]
	loading=big_splash_loading.gif //000d944b
	coin_ani=editor_splash_coin.gif//000d9010
	coin_ani=coin.gif//002dfe6d, 002df9d4

	[title]
	curtain=curtain.gif //002f6733, 002e9beb
	www=www.gif//002f336b,002f2d5a
	selector_red=selector_red.gif
	selector_green=selector_green.gif//002f9742
	scroll_up=scroll_up.gif//002fb1c1, 002fb568
	scroll_down=scroll_down.gif//002faa77, 002fae1e
	worldmap_back=world_back.gif

	[HUD]
	itemslot=itemslot.gif
	itemslot_p1=itemslot_p1.gif
	itemslot_p2=itemslot_p2.gif
	heard_on=heart.gif//002fa65e
	heard_off=heart_n.gif//002fa247
	star=stars.gif
	key=key.gif
	coin=coins.gif
	bomb=bombs.gif
	rupee=emerald.gif//002f8c65, 002f88d4
	1up=1up.gif
	2up=2up.gif//002f84ed, 002f810a

	[yoshi]
	tongue=yoshi_tongue.gif
	tongue_line=yoshi_tongue_line.gif
	wings=wings.gif

	[shoes]
	green=boot_green.gif
	red=boot_red.gif
	blue=boot_blue.gif

	[clowncar]
	clowncar=clowncar.gif

	[cursors]
	white=cursor_white.gif
	blue=cursor_blue.gif
	cyan=cursor_cyan.gif
	gray=cursor_gray.gif
	green=cursor_green.gif
	magenta=cursor_magenta.gif
	violet=cursor_violet.gif
	red=cursor_red.gif
	yellow=cursor_yellow.gif
	rubber=cursor_rubber.gif

	[battle]
	versus=btl_vs.gif
	mario=btl_mario.gif
	luigi=btl_luigi.gif
	peach=btl_peach.gif
	toad=btl_toad.gif
	link=btl_link.gif
	wins=btl_wins.gif

	[misc]
	warp_point=warp.gif
	messagebox=message_box.gif
	npc_talkable=nps_message.gif
	*/

	/******************Hex section************************************/
	std::vector<std::string > list= GraphicsINI.getAllSectionKeys("hex");
	for(unsigned int i=0; i<list.size(); i++)
	{
		bool wrong=false;
		RemoveSubStr(list[i], "0x");
		for(unsigned int j=0;j<list[i].size();j++)
		{
			if(!isdigit(list[i][j]))
			{
				if((list[i][j]!='a')&&(list[i][j]!='A')&&
					(list[i][j]!='b')&&(list[i][j]!='B')&&
					(list[i][j]!='c')&&(list[i][j]!='C')&&
					(list[i][j]!='d')&&(list[i][j]!='D')&&
					(list[i][j]!='e')&&(list[i][j]!='E')&&
					(list[i][j]!='f')&&(list[i][j]!='F'))
				{
					wrong=true; break;
				}
			}
		}
		if(wrong) continue;

		unsigned int hexKey;   
		std::stringstream ss;
		ss << std::hex << list[i];
		ss >> hexKey;
		std::string imageFile = GraphicsINI.Get("hex", list[i], "");
	    if(imageFile.empty()) continue;
		
		imageFile = root+"graphics\\common\\"+imageFile;
		const char *str = imageFile.c_str();

		//Here we should load file  hexKey - address, imageFile:
		//is a name of image file in the <SMBX>\graphics\common
		patchGraphics(hexKey, str);
	}
	/******************Hex section**end*******************************/

}
