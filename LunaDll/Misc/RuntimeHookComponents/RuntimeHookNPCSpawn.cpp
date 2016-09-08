//0.1 とりあえずspawn全部hook

#include <windows.h>
#include <stdio.h>
#include<iostream>
#include<fstream>
#include <vector>
#include <DelayImp.h>
#include "../AsmPatch.h"
#include "../RuntimeHook.h"
#include "../../LuaMain/LunaLuaMain.h"
#include "../../Globals.h"
#include "../../SMBXInternal/Animation.h"
#include "../../SMBXInternal/Blocks.h"
#include "../../SMBXInternal/NPCs.h"
#include "../../SMBXInternal/Blocks.h"
#include "../../SMBXInternal/BGOs.h"
#include "../../SMBXInternal/CameraInfo.h"
#include "../../SMBXInternal/Layer.h"
#include "../../SMBXInternal/PlayerMOB.h"
#include "../../SMBXInternal/SMBXEvents.h"
#include "../../SMBXInternal/Tile.h"
#include "../../SMBXInternal/Warp.h"
#include "../../SMBXInternal/Water.h"
#include "../../LuaMain/LuaProxy.h"
using namespace std;

#define A_PATCH extern "C" void __stdcall

#define ID_OFFSET 0xE2
#define NPC_MEM_SIZE 344
#define NPC_PTR 0x00B259E8
#define NPC_COUNT 0x00B2595A
#define PLAYER_MEM_SIZE 0x184
#define PLAYER_PTR 0x00B25A20
#define PLAYER_COUNT 0x00B2595E

#define LakituSMW 0x00A5D501 //7BYTE MOV EDI,ESI , Variable NPC(can get from source?)
#define HammerBros 0x00A599E5 //Dest calc MOV ECX+EAX*8,ESI
#define VenusFireTrap 0x00A55B68 //Normal MOV EDI,ESI
#define LakituSMB3 0x00A5B593 //Normal MOV EDI,ESI
//Treat ECX as safe for the next line: MOVSX   EDI, WORD PTR [ECX] 

#define BowserStatue 0x00A5F323 //Normal MOV EDI,ESI
#define Birdo 0x00A5BD2F //Normal MOV EDI,ESI
#define VolcanoLotus 0x00A45FC7 //Normal MOV EDI,ESI
#define Larry 0x00A47723 //Normal MOV EDI,ESI
#define Ludwig 0x00A486B8 //Normal MOV EDI,ESI
#define Nazrin 0x00A49AFC //Normal MOV EDI,ESI
#define THPB 0x00A4B8E9 //Dest calc MOV EAX+EDX*8,ESI
#define Wart 0x00A4BE87 //Dest calc MOV EDX+ECX*8,ESI
#define BowserSMB1Hammer 0x00A4CADC //Dest calc MOV EAX+EDX*8,ESI
#define BowserSMB1Fire 0x00A4CE82 //Normal MOV EDI,ESI

//#define GraySnifit 0x00A4E589 //Source calc MOV ESI,ECX+EAX*8 2つ分書き変える
//00A4E589 | 66:C786 2A010000 6400 | MOV     WORD PTR[ESI + 12A], 64 
//00A4E592 | 66 : C786 E2000000 8500 | MOV     WORD PTR[ESI + E2], 85
#define GraySnifit 0x00A4E592		//EBP+8==PlayerIndex

#define RedVineHead 0x00A5282D //Source calc MOV EDI,ECX+EAX*8
#define GreenVineHead1 0x00A52876 //Source calc MOV EDI,EDX+ECX*8
#define GreenVineHead2 0x00A528BB //Source calc MOV EDI,EAX+EDX*8
#define BillBlaster 0x00A2175D //(or billy gun)  Source calc MOV EDI,EAX+EDX*8
#define BowserSMB3 0x00A1F3F4 //Source calc MOV EDI,EDX+ECX*8
#define FPropellerBlock 0x009CD544 //Normalizable MOV EDI,(*((WORD*)ESI)+0x80)*43+(*(DWORD*)0x00B259E8))


#define BlockSpawnProc 0x009DC981 //source block: DWORD PTR[EBP-138]?
//009DCE0D | 66:899CD0 E2000000       | MOV     WORD PTR [EAX+EDX*8+E2], BX
/*
Blockの中身が1000以上かどうか(コインかアイテムか判定する部分)
009DC968 | 66:3D E803 | CMP     AX, 3E8 |
009DC96C | 0F8C D0100000 | JL      009DDA42 |
009DC972 | 66 : 2D E803 | SUB     AX, 3E8 | フックは この直後辺りが良い

HOOK?
Source:*(WORD*)(*(DWORD*)B2BA14+ESI*2)辺り?
//input:eax ebp

009DC97C | 8BD8                     | MOV     EBX, EAX                           |
009DC97E | 8B45 0C                  | MOV     EAX, DWORD PTR [EBP+C]             |
009DC981 | 899D 70FFFFFF            | MOV     DWORD PTR [EBP-90], EBX            |<-書き変える

*/

//should treat eax as safe

#define Generator 0x00A07BF0 //MOV     DWORD PTR[EBP - 4E0], 0  dest==esi
//00A07BF0 | C785 20FBFFFF 00000000 | 

#define EggAnim 0x009E64B4
/*
候補
009E4A75 | 66:8987 E2000000         | MOV     WORD PTR [EDI+E2], AX              |　FAIL

009E64A9 | 66:8986 E2000000         | MOV     WORD PTR [ESI+E2], AX              |  CALLED

009E67AE | 66:8987 E2000000         | MOV     WORD PTR [EDI+E2], AX              |  FAIL

009E64B4 | A1 C45BB200 | MOV     EAX, DWORD PTR [B25BC4] | 0x009E64A9直接書き変えたら上手く動かなかったのでこっちを書き変え

Animは消えるのでSource無し
7Byte


*/

//#define PlayerHammer 0x009C6B02
// 009C6B02 | 66:C784D0 E2000000 AB00 | MOV     WORD PTR[EAX + EDX * 8 + E2], AB | ハンマー
// ピーチやキノピオの場合も一旦ハンマー出してから書き変えてるのでこれだけで良い
// source:EBX

#define PlayerHammer 0x009C6D23 //呼び出しを遅くした　IDの変更より後
//009C6D23 | 66:C784C1 36010000 FFFF | MOV     WORD PTR[ECX + EAX * 8 + 136], FFFF |



//#define PlayerFireball 0x009C7CB7
// 009C7CB7 | 66:C784CA E2000000 0D00  | MOV     WORD PTR [EDX+ECX*8+E2], D       
// ハンマーと同様これだけで良い
// source:EBX

#define PlayerFireball 0x009C7D2D //呼び出しを遅くした　IDの変更より後
//009C7D2D | 66:C784CA 36010000 FFFF | MOV     WORD PTR[EDX + ECX * 8 + 136], FFFF |

#define PlayerBomb 0x00908F6A
// 009C8F6A | 66:C786 E2000000 8600    | MOV     WORD PTR [ESI+E2], 86        
// 良く分からないけどある
// source:EBX

//#define LinkSwordShoot 0x0099FBAE
//0099FBAE | 66:C784C1 E2000000 0A01  | MOV     WORD PTR [ECX+EAX*8+E2], 10A        |

//#define LinkShoot  0x0099FB2E
//0099FB2E | 66:C784C1 E2000000 0D00  | MOV     WORD PTR [ECX+EAX*8+E2], D          |
//Fire出した後にiceかshadowかで書き変えるのでこれだけでよい
// source:EBX

#define LinkShoot  0x0099FBE4
//0099FBE4 | 66:C784C1 36010000 FFFF  | MOV     WORD PTR [ECX+EAX*8+136], FFFF      |

#define YoshiFire 0x009BF0CC
//009BF0CC | 66:C784D0 E2000000 6C00  | MOV     WORD PTR [EAX+EDX*8+E2], 6C         |
//source:ESI?


#define PlayerYoshi1 0x0099BCE6 //知らない
//0099BCE6 | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source EBX

#define PlayerYoshi2 0x0099D0D9 //キー入力で降りた時
//0099D0D9 | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source EBX
#define PlayerYoshi3 0x009B59E8 //ダメージ受けて降りた時
//009B59E8 | 66:C787 2A010000 6400    | MOV     WORD PTR [EDI+12A], 64
//source:getPlayerAddrByIndex(*(WORD*)(*(DWORD*)(EBP+8)))) ? 
#define PlayerShoe1 0x0099CC30 //?
//0099CC30 | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source:EBX
#define PlayerShoe2 0x0099B8BE //?
//0099B8BE | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source EBX
#define PlayerClown1 0x00998719 //キー入力
//00998719 | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source EBX
#define PlayerClown2 0x009B60FA //?
//009B60FA | 66:C787 2A010000 6400    | MOV     WORD PTR [EDI+12A], 64              |
//source EBX
#define PlayerClown3 0x009B6B1D //死亡して降りた時(ダメージ受けても降りない)
//009B6B1D | 66:C786 2A010000 6400 | MOV     WORD PTR[ESI + 12A], 64 |
//source EBX
#define PlayerClown4 0x009C4FE4 //? 降りる場合でなくても呼ばれる
//009C4FE4 | 66:C786 2A010000 6400    | MOV     WORD PTR [ESI+12A], 64              |
//source EBX
#define ItemDrop 0x00A24478
//00A24478 | 66:8984D1 E2000000 | MOV     WORD PTR[ECX + EDX * 8 + E2], AX |
//source 無し
/*
enum AsmConsts::R32 {
	AsmConsts::R32_NIL = 0xFF,
	AsmConsts::R32_EAX = 0x0,
	AsmConsts::R32_ECX = 0x1,
	AsmConsts::R32_EDX = 0x2,
	AsmConsts::R32_EBX = 0x3,
	AsmConsts::R32_ESP = 0x4,
	AsmConsts::R32_EBP = 0x5,
	AsmConsts::R32_ESI = 0x6,
	AsmConsts::R32_EDI = 0x7
};
*/

DWORD A_NPCSpawn::getPlayerAddrByIndex(int);
DWORD A_NPCSpawn::getNPCAddrByIndex(int);
WORD A_NPCSpawn::getCurrentNPCCount();
void A_NPCSpawn::mPatchE();
void A_NPCSpawn::registerSpawnEvent(void*);
void A_NPCSpawn::clearCallback();
namespace A_NPCSpawn {
	enum SMBXOBJTYPE {
		SMBX_NONE = 0x0,
		SMBX_BLOCK = 0x1,
		SMBX_BGO = 0x2,
		SMBX_CAMERA = 0x3,
		SMBX_SECTION = 0x4,
		SMBX_PLAYER = 0x5,
		SMBX_PLAYER_SETTINGS = 0x6,
		SMBX_NPC = 0x7,
		SMBX_ANIMATION = 0x8,
		SMBX_LAYER = 0x9,
		SMBX_LEVEL = 0xA,
		SMBX_WARP = 0xB
	};
}

extern void NPCSpawnHook() {
	A_NPCSpawn::mPatchE();
}

extern void setNPCSpawnCallback(void* f) {
	A_NPCSpawn::registerSpawnEvent(f);
}

extern void clearNPCSpawnCallback() {
	A_NPCSpawn::clearCallback();
}

//もしNPCCountがspawnごとに逐一インクリメントされない場合は自前でカウンタ用意して増やす。
/*
同一フレームで次のような事が起こる場合:
<Player1> spawn NPCCountの位置にUID書きこむ	(NPCCountがインクリメントされない,SMBXか別の変数で管理している)
<Player2> spawn NPCCountの位置にUID書き込む	(Player1のNPCに書き込むことになる)

判明次第自前でカウンタ用意して修正する。
*/

A_PATCH spawnHookLakituSMW(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)(*((DOUBLE*)(source + 0xF0))); //lakitu.ai1 value
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookHammerBros(DWORD source, DWORD dest1, DWORD dest2) { //UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)0x1E;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookVenusFireTrap(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0xF6;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookLakituSMB3(DWORD source, DWORD dest) { //UID writing OK 直後のECXは破壊してはいけない
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x30;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBowserStatue(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x55;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBirdo(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x28;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookVolcanoLotus(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x114;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookLarry(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x10D;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookLudwig(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x11A;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookNazrin(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x86;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookTHPB(DWORD source, DWORD dest1, DWORD dest2) { //UID writing OK
																// *(WORD*)(*(DWORD*)(EBP+8)) <-NPCのインデックス?
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)0xD2;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, A_NPCSpawn::getNPCAddrByIndex(*(WORD*)(*(DWORD*)(source + 0x8))), A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookWart(DWORD source, DWORD dest1, DWORD dest2) { //UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)0xCA;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBowserSMB1Hammer(DWORD source, DWORD dest1, DWORD dest2) { //UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)0x1E;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBowserSMB1Fire(DWORD source, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x57;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookGraySnifit(DWORD source, DWORD dest) { //UID writing OK
														//*((WORD*)(dest + 0x12A)) = (WORD)0x64;
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x85;
	A_NPCSpawn::procSpawnEvent(dest, A_NPCSpawn::getNPCAddrByIndex(*(WORD*)(*(DWORD*)(source + 0x8))), A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookRedVineHead(DWORD source1, DWORD source2, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0xD6;
	A_NPCSpawn::procSpawnEvent(dest, source1 + source2 * 8, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookGreenVineHead1(DWORD source1, DWORD source2, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0xD5;
	A_NPCSpawn::procSpawnEvent(dest, source1 + source2 * 8, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookGreenVineHead2(DWORD source1, DWORD source2, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0xE0;
	A_NPCSpawn::procSpawnEvent(dest, source1 + source2 * 8, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBillBlaster(DWORD source1, DWORD source2, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x11;
	A_NPCSpawn::procSpawnEvent(dest, source1 + source2 * 8, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBowserSMB3(DWORD source1, DWORD source2, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x57;
	A_NPCSpawn::procSpawnEvent(dest, source1 + source2 * 8, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookFPropellerBlock(DWORD source_raw, DWORD dest) { //UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x6C; //毎フレーム書き変え続ければ落ちない
	DWORD source = (*((WORD*)source_raw) + 0x80) * 344 + (*(DWORD*)0x00B259E8);
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookGenerator(DWORD dest, DWORD dest_raw) { //UID writing OK
	*(DWORD*)(dest_raw - 0x4E0) = (DWORD)0x0; //よく知らないけど元通りにしておく

											  //初期化されなかったり、初期化のタイミングが遅いものを初期化
	*(WORD*)(dest + 0x64) = 0; //isGenerator? -> false
	*(WORD*)(dest + 0xE6) = 0; //unused
	*(WORD*)(dest + 0x08) = 0; //unused

	A_NPCSpawn::procSpawnEvent(dest, A_NPCSpawn::getNPCAddrByIndex(*(WORD*)(dest_raw - 0x180)), A_NPCSpawn::SMBX_NPC);
}

A_PATCH spawnHookBlock(DWORD dest_raw) { //UID writing OK,use A_NPCSpawn::getNPCAddrByIndex(getCurrentNPCCount()+1)
										 //dest:lastNPC?

										 //dllの先頭
										 /*
										 push ebp     --epb at esp
										 mov ebp,esp  --original ebp at current ebp

										 +eaxを保存しない処理
										 */
										 //DWORD tmpebx;
	__asm {
		push ebx	//ebxの内容保持
	}
	DWORD blockidx;

	__asm {
		mov eax, dword ptr[ebp]
			mov eax, dword ptr[eax + 0x8]
			mov blockidx, eax
	}

	DWORD source = *((DWORD*)0xB25A04) + *(WORD*)blockidx * 0x68;
	A_NPCSpawn::procSpawnEvent(A_NPCSpawn::getNPCAddrByIndex(A_NPCSpawn::getCurrentNPCCount() + 1), source, A_NPCSpawn::SMBX_BLOCK);

	__asm {
		pop ebx
			mov eax, dword ptr[ebp]     //eaxに元々のebpを入れる
			mov dword ptr[eax - 0x90], ebx //本来のSMBXの操作
			mov eax, dword ptr[eax + 0xC] //元々のeaxは破壊されてるので修復(__asm以前から破壊されている)

	}

	//DebugMsgBox("%lu", getLastNPCAddr());

	//*((WORD*)(A_NPCSpawn::getNPCAddrByIndex(getCurrentNPCCount()+1) + 0xE6)) = 1234; //*(WORD*)(*(DWORD*)(0xB2BA14) + dest_raw * 2);
	//not refreshed npc count yet,so +1

}

A_PATCH spawnHookEggAnim() { //UID writing OK , mov word ptr[esi+0xE6],<UID>
							 //EDI contentID
							 //ESI dest
							 //AX word[EDX+ECX*4+3E]
							 //EBX DWORD PTR [4010F4]
							 //DebugMsgBox("%lu,%lu", getLastNPCAddr()-dest,contentID);
	DWORD dest;

	__asm {
		mov dest, esi
	}

	A_NPCSpawn::procSpawnEvent(dest, 0, A_NPCSpawn::SMBX_NONE); //the parent is a "destroyed" anim, nonsense
	__asm {
		mov ebx, 0x00B25BC4
			mov eax, dword ptr[ebx]
	}
	/*
	*((WORD*)(dest + ID_OFFSET)) = contentID;
	*((WORD*)(dest + 0x6E)) = 1234;
	*/
}

A_PATCH spawnHookPlayerHammer(DWORD source, DWORD dest1, DWORD dest2) {//UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + 0x136)) = (WORD)0xFFFF;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_PLAYER);
}

A_PATCH spawnHookPlayerFireball(DWORD source, DWORD dest1, DWORD dest2) {//UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + 0x136)) = (WORD)0xFFFF;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_PLAYER);
}

A_PATCH spawnHookPlayerBomb(DWORD source, DWORD dest) {//UID writing OK
	*((WORD*)(dest + ID_OFFSET)) = (WORD)0x86;//(WORD)0x86;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_PLAYER);
}

A_PATCH spawnHookLinkShoot(DWORD source, DWORD dest1, DWORD dest2) {//UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + 0x136)) = (WORD)0xFFFF;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_PLAYER);
}

A_PATCH spawnHookPlayerMount(DWORD source, DWORD dest) {//UID writing OK
	__asm {
		push edx	//edxの内容保持
	}
	*((WORD*)(dest + 0x12A)) = (WORD)0x64;
	A_NPCSpawn::procSpawnEvent(dest, source, A_NPCSpawn::SMBX_PLAYER);
	__asm {
		pop edx	//edxの内容保持
	}
}

A_PATCH spawnHookPlayerMount2(DWORD source, DWORD dest) {//UID writing OK
	__asm {
		push edx	//edxの内容保持
	}
	*((WORD*)(dest + 0x12A)) = (WORD)0x64;
	A_NPCSpawn::procSpawnEvent(dest, A_NPCSpawn::getPlayerAddrByIndex(*(WORD*)(*(DWORD*)(source + 8))), A_NPCSpawn::SMBX_PLAYER);
	__asm {
		pop edx	//edxの内容保持
	}
}

A_PATCH spawnHookItemDrop(DWORD dest1, DWORD dest2, DWORD contentID) {//UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)contentID;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, 0, A_NPCSpawn::SMBX_NONE); //no parent
}

A_PATCH spawnHookYoshiFire(DWORD source, DWORD dest1, DWORD dest2) {//UID writing OK
	*((WORD*)(dest1 + dest2 * 8 + ID_OFFSET)) = (WORD)0x6C;
	A_NPCSpawn::procSpawnEvent(dest1 + dest2 * 8, source, A_NPCSpawn::SMBX_PLAYER);
}

namespace A_NPCSpawn {
	
	/*
	void DebugMsgBox(LPCSTR pszFormat, ...)
	{
	va_list	argp;
	char pszBuf[256];
	va_start(argp, pszFormat);
	vsprintf(pszBuf, pszFormat, argp);
	va_end(argp);
	MessageBox(NULL, pszBuf, "debug info", MB_OK);
	}
	*/

	struct asmData {
		DWORD baseAddr;
		vector<char> byteData;
	};
	vector<void*> spawnCallback;
	/*
	void saveOriginalAsmCode(vector<asmData>* outVec, DWORD base, DWORD len){
	ofstream fout("asmCode.txt",std::ios::app);
	//DebugMsgBox("%d,%d", base, len);
	asmData asmCode;
	asmCode.baseAddr = base;

	for (DWORD i = 0; i < len; i++){

	asmCode.byteData.push_back(*(unsigned char*)(base + i));
	fout << "*(unsigned char*)(0x" << hex << base + i << ")=0x" << hex << (unsigned int)*(unsigned unsigned char*)(base + i) << ";" << endl;
	}
	fout << endl;
	fout.close();
	(*outVec).push_back(asmCode);
	}
	*/
	/* mendoi luaでやる
	DWORD NPCAddrSet[5000];
	WORD transientUID = 1; //0==invalid



	WORD getCurrentUID(){
	return transientUID;
	}

	void setCurrentUID(WORD uid){
	transientUID = uid;
	}

	void initUID(){
	setCurrentUID(1);
	}

	WORD generateUID(){
	WORD tmp = getCurrentUID();
	setCurrentUID(tmp + 1);
	return tmp;
	}

	*/

	WORD getCurrentNPCCount() {
		return *((WORD*)NPC_COUNT);
	}
	void setNPCCount(WORD count) {
		*((WORD*)NPC_COUNT) = count;
	}
	void incrementNPCCount() {
		setNPCCount(getCurrentNPCCount() + 1);
	}
	DWORD getNPCAddrByIndex(int index) {
		return ((*((DWORD*)NPC_PTR)) + 0x80 * NPC_MEM_SIZE) + (DWORD)(index * NPC_MEM_SIZE);
	}

	DWORD getLastNPCAddr() {
		return getNPCAddrByIndex(getCurrentNPCCount());
	}

	DWORD getPlayerAddrByIndex(int index) {
		return (*((DWORD*)PLAYER_PTR)) + (DWORD)(index * PLAYER_MEM_SIZE);
	}

	void callOnSpawn(LuaProxy::NPC spawnedNPC, DWORD sourceAddr, SMBXOBJTYPE sourceObjType) {
		for (void* cb : spawnCallback) {
			((void(*)(LuaProxy::NPC, DWORD, SMBXOBJTYPE))cb)(spawnedNPC, sourceAddr, sourceObjType);
		}

	}

	void procSpawnEvent(DWORD spawnedNPCAddr, DWORD sourceAddr, SMBXOBJTYPE sourceObjType) {
		DWORD cdist = 0;
		if ((cdist = spawnedNPCAddr - (DWORD)*GM_NPCS_PTR_CONSTPTR) % sizeof(::NPCMOB) != 0)return;
		LuaProxy::NPC spawnedNPC = LuaProxy::NPC(cdist / sizeof(::NPCMOB));
		callOnSpawn(spawnedNPC, sourceAddr, sourceObjType); //native
		if (gLunaLua.isValid) {
			std::shared_ptr<Event> npcSpawnEvent = std::make_shared<Event>("onNPCSpawn", false);
			npcSpawnEvent->setDirectEventName("onNPCSpawn");
			npcSpawnEvent->setLoopable(false);
			DWORD dist = 0;
			switch (sourceObjType) {
			case SMBX_BLOCK:
				if ((dist = sourceAddr - (DWORD)*GM_BLOCKS_PTR_CONSTPTR) % sizeof(::Block) != 0)break;
				gLunaLua.callEvent(npcSpawnEvent, spawnedNPC, LuaProxy::Block(dist/sizeof(::Block)), sourceObjType);
				//gLunaLua.callEvent(npcSpawnEvent, (::Block*)sourceAddr, sourceObjType);
				break;
			case SMBX_BGO:
				if ((dist = sourceAddr - (DWORD)*GM_BGOS_PTR_CONSTPTR) % sizeof(::SMBX_BGO) != 0)break;
				gLunaLua.callEvent(npcSpawnEvent, spawnedNPC, LuaProxy::BGO(dist/sizeof(::SMBX_BGO)), sourceObjType); //conflicting name
				break;
			case SMBX_CAMERA:
				break;
			case SMBX_SECTION: 
				break;
			case SMBX_PLAYER:
				if ((dist = sourceAddr - (DWORD)*GM_PLAYERS_PTR_CONSTPTR) % sizeof(::PlayerMOB) != 0)break;
				gLunaLua.callEvent(npcSpawnEvent, spawnedNPC, LuaProxy::Player(dist/ sizeof(::PlayerMOB)), sourceObjType);
				break;
			case SMBX_PLAYER_SETTINGS:
				break;
			case SMBX_NPC:
				if ((dist = sourceAddr - (DWORD)*GM_NPCS_PTR_CONSTPTR) % sizeof(::NPCMOB) != 0)break;
				gLunaLua.callEvent(npcSpawnEvent, spawnedNPC, LuaProxy::NPC(dist/sizeof(::NPCMOB)-129), sourceObjType);
				break;
			case SMBX_ANIMATION:
				if ((dist = sourceAddr - (DWORD)*GM_ANIM_PTR_CONSTPTR) % sizeof(::SMBXAnimation) != 0)break;
				gLunaLua.callEvent(npcSpawnEvent, spawnedNPC, LuaProxy::Animation(dist/sizeof(::SMBXAnimation)), sourceObjType);
				break;
			case SMBX_LAYER:
				break;
			case SMBX_LEVEL:
				break;
			case SMBX_WARP:
				break;
			case SMBX_NONE:
			default:
				break;
			}
			
		}
		
	}


	//WORD PTR [REG+E2], IMMEDIATE_VALUE

	void patch_SPAWN_ID_MOV_VAR(DWORD head, AsmConsts::R32 sourceType, AsmConsts::R32 destType, void* func) {
		//7Byte

		BYTE pushSource = byte(0x50 | sourceType);
		BYTE pushDest = byte(0x50 | destType);
		*(BYTE*)(head + 0x0) = pushDest;
		*(BYTE*)(head + 0x1) = pushSource;
		*(BYTE*)(head + 0x2) = 0xE8; //CALL
		*(DWORD*)(head + 0x3) = (DWORD)((DWORD)func - (head + 0x2) - 5); //DWORD &FUNC
	}

	void patch_SPAWN_ID_MOV(DWORD head, AsmConsts::R32 sourceType, AsmConsts::R32 destType, void* func) {

		BYTE pushSource = byte(0x50 | sourceType);
		BYTE pushDest = byte(0x50 | destType);
		*(BYTE*)(head + 0x0) = pushDest;
		*(BYTE*)(head + 0x1) = pushSource;
		*(BYTE*)(head + 0x2) = 0xE8; //CALL
		*(DWORD*)(head + 0x3) = (DWORD)((DWORD)func - (head + 0x2) - 5); //DWORD &FUNC
		*(BYTE*)(head + 0x7) = 0x90;//NOP
		*(BYTE*)(head + 0x8) = 0x90;//NOP
	}

	void patch_SPAWN_ID_MOV_DCALC(DWORD head, AsmConsts::R32 sourceType, AsmConsts::R32 destBaseType, AsmConsts::R32 destOffsetType, void* func) {
		//10Byte

		BYTE pushSource = byte(0x50 | sourceType);
		BYTE pushDestBase = byte(0x50 | destBaseType);
		BYTE pushDestOffset = byte(0x50 | destOffsetType);
		*(BYTE*)(head + 0x0) = pushDestOffset;
		*(BYTE*)(head + 0x1) = pushDestBase;
		*(BYTE*)(head + 0x2) = pushSource;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
		*(BYTE*)(head + 0x8) = 0x90;//NOP
		*(BYTE*)(head + 0x9) = 0x90;//NOP
	}

	void patch_SPAWN_ID_MOV_DCALC_VAR(DWORD head, AsmConsts::R32 sourceType, AsmConsts::R32 destBaseType, AsmConsts::R32 destOffsetType, void* func) {

		BYTE pushSource = byte(0x50 | sourceType);
		BYTE pushDestBase = byte(0x50 | destBaseType);
		BYTE pushDestOffset = byte(0x50 | destOffsetType);
		*(BYTE*)(head + 0x0) = pushDestOffset;
		*(BYTE*)(head + 0x1) = pushDestBase;
		*(BYTE*)(head + 0x2) = pushSource;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
	}

	void patch_SPAWN_ID_MOV_SCALC(DWORD head, AsmConsts::R32 sourceBaseType, AsmConsts::R32 sourceOffsetType, AsmConsts::R32 destType, void* func) {

		BYTE pushSourceBase = byte(0x50 | sourceBaseType);
		BYTE pushSourceOffset = byte(0x50 | sourceOffsetType);
		BYTE pushDest = byte(0x50 | destType);
		*(BYTE*)(head + 0x0) = pushDest;
		*(BYTE*)(head + 0x1) = pushSourceOffset;
		*(BYTE*)(head + 0x2) = pushSourceBase;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
		*(BYTE*)(head + 0x8) = 0x90;//NOP

	}

	void patch_GENERATOR(DWORD head, void* func) {
		//10Byte

		BYTE pushSource = byte(0x50 | AsmConsts::R32_ESI);
		BYTE pushDest = byte(0x50 | AsmConsts::R32_EBP);
		BYTE pushEAX = byte(0x50 | AsmConsts::R32_EAX);
		BYTE popEAX = byte(0x58 | AsmConsts::R32_EAX);
		*(BYTE*)(head + 0x0) = pushEAX;
		*(BYTE*)(head + 0x1) = pushDest;
		*(BYTE*)(head + 0x2) = pushSource;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
		*(BYTE*)(head + 0x8) = popEAX;
		*(BYTE*)(head + 0x9) = 0x90;//NOP
	}


	//FPropellerBlockフックすると落ちるので対策 EAXをスタックに退避させると落ちない
	void patch_SPAWN_ID_MOV_REGSAFE(DWORD head, AsmConsts::R32 sourceType, AsmConsts::R32 destType, AsmConsts::R32 safeType, void* func) {

		BYTE pushSource = byte(0x50 | sourceType);
		BYTE pushDest = byte(0x50 | destType);
		*(BYTE*)(head + 0x0) = byte(0x50 | safeType);//save safeType
		*(BYTE*)(head + 0x1) = pushDest;
		*(BYTE*)(head + 0x2) = pushSource;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
		*(BYTE*)(head + 0x8) = byte(0x58 | safeType);//restore safeType

	}

	void patch_BLOCK(DWORD head, void* func) {

		BYTE pushDest = byte(0x50 | AsmConsts::R32_ESI);
		*(BYTE*)(head + 0x0) = pushDest;
		*(BYTE*)(head + 0x1) = 0xE8; //CALL
		*(DWORD*)(head + 0x2) = (DWORD)((DWORD)func - (head + 0x1) - 5); //DWORD &FUNC
	}

	void patch_EGGANIM(DWORD head, void* func) {
		//5Byte

		*(BYTE*)(head + 0x0) = 0xE8; //CALL
		*(DWORD*)(head + 0x1) = (DWORD)((DWORD)func - (head + 0x0) - 5); //DWORD &FUNC
	}

	void patch_ITEMDROP(DWORD head, void* func) {

		BYTE pushContent = byte(0x50 | AsmConsts::R32_EAX);
		BYTE pushDestOffset = byte(0x50 | AsmConsts::R32_EDX);
		BYTE pushDestBase = byte(0x50 | AsmConsts::R32_ECX);
		*(BYTE*)(head + 0x0) = pushContent;
		*(BYTE*)(head + 0x1) = pushDestOffset;
		*(BYTE*)(head + 0x2) = pushDestBase;
		*(BYTE*)(head + 0x3) = 0xE8; //CALL
		*(DWORD*)(head + 0x4) = (DWORD)((DWORD)func - (head + 0x3) - 5); //DWORD &FUNC
	}

	//メモリ書き変え対策
	//0x009XXXXXはたまに書き変わって落ちるので毎フレーム更新する必要がある

	void keepAddr() {
		patch_SPAWN_ID_MOV_REGSAFE(FPropellerBlock, AsmConsts::R32_ESI, AsmConsts::R32_EDI, AsmConsts::R32_EAX, &spawnHookFPropellerBlock);
		patch_BLOCK(BlockSpawnProc, &spawnHookBlock);
		patch_EGGANIM(EggAnim, &spawnHookEggAnim);

		patch_SPAWN_ID_MOV_DCALC(PlayerHammer, AsmConsts::R32_EBX, AsmConsts::R32_ECX, AsmConsts::R32_EAX, &spawnHookPlayerHammer);
		patch_SPAWN_ID_MOV_DCALC(PlayerFireball, AsmConsts::R32_EBX, AsmConsts::R32_EDX, AsmConsts::R32_ECX, &spawnHookPlayerFireball);
		patch_SPAWN_ID_MOV(PlayerBomb, AsmConsts::R32_EBX, AsmConsts::R32_ESI, &spawnHookPlayerBomb);
		patch_SPAWN_ID_MOV_DCALC(LinkShoot, AsmConsts::R32_EBX, AsmConsts::R32_ECX, AsmConsts::R32_EAX, &spawnHookLinkShoot);

		patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi1, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi2, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi3, AsmConsts::R32_EBP, AsmConsts::R32_EDI, AsmConsts::R32_EAX, &spawnHookPlayerMount2);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerShoe1, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerShoe2, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerClown1, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerClown2, AsmConsts::R32_EBX, AsmConsts::R32_EDI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		patch_SPAWN_ID_MOV_REGSAFE(PlayerClown3, AsmConsts::R32_EBP, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount2);
		patch_SPAWN_ID_MOV_DCALC(YoshiFire, AsmConsts::R32_ESI, AsmConsts::R32_EAX, AsmConsts::R32_EDX, &spawnHookYoshiFire);
		patch_ITEMDROP(ItemDrop, &spawnHookItemDrop);

	}

	void mPatchE() {

		patch_SPAWN_ID_MOV_VAR(LakituSMW, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookLakituSMW);
		patch_SPAWN_ID_MOV_DCALC(HammerBros, AsmConsts::R32_ESI, AsmConsts::R32_ECX, AsmConsts::R32_EAX, &spawnHookHammerBros);
		patch_SPAWN_ID_MOV(VenusFireTrap, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookVenusFireTrap);
		patch_SPAWN_ID_MOV_REGSAFE(LakituSMB3, AsmConsts::R32_ESI, AsmConsts::R32_EDI, AsmConsts::R32_ECX, &spawnHookLakituSMB3);
		patch_SPAWN_ID_MOV_REGSAFE(BowserStatue, AsmConsts::R32_ESI, AsmConsts::R32_EDI, AsmConsts::R32_EDX, &spawnHookBowserStatue);
		patch_SPAWN_ID_MOV(Birdo, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookBirdo);
		patch_SPAWN_ID_MOV(VolcanoLotus, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookVolcanoLotus);
		patch_SPAWN_ID_MOV_REGSAFE(Larry, AsmConsts::R32_ESI, AsmConsts::R32_EDI, AsmConsts::R32_EDX, &spawnHookLarry);
		patch_SPAWN_ID_MOV(Ludwig, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookLudwig);
		patch_SPAWN_ID_MOV(Nazrin, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookNazrin);
		patch_SPAWN_ID_MOV_DCALC(THPB, AsmConsts::R32_EBP, AsmConsts::R32_EAX, AsmConsts::R32_EDX, &spawnHookTHPB);
		patch_SPAWN_ID_MOV_DCALC(Wart, AsmConsts::R32_ESI, AsmConsts::R32_EDX, AsmConsts::R32_ECX, &spawnHookWart);
		patch_SPAWN_ID_MOV_DCALC(BowserSMB1Hammer, AsmConsts::R32_ESI, AsmConsts::R32_EAX, AsmConsts::R32_EDX, &spawnHookBowserSMB1Hammer);
		patch_SPAWN_ID_MOV(BowserSMB1Fire, AsmConsts::R32_ESI, AsmConsts::R32_EDI, &spawnHookBowserSMB1Fire);
		patch_SPAWN_ID_MOV_REGSAFE(GraySnifit, AsmConsts::R32_EBP, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookGraySnifit);
		patch_SPAWN_ID_MOV_SCALC(RedVineHead, AsmConsts::R32_ECX, AsmConsts::R32_EAX, AsmConsts::R32_EDI, &spawnHookRedVineHead);
		patch_SPAWN_ID_MOV_SCALC(GreenVineHead1, AsmConsts::R32_EDX, AsmConsts::R32_ECX, AsmConsts::R32_EDI, &spawnHookGreenVineHead1);
		patch_SPAWN_ID_MOV_SCALC(GreenVineHead2, AsmConsts::R32_EAX, AsmConsts::R32_EDX, AsmConsts::R32_EDI, &spawnHookGreenVineHead2);
		patch_SPAWN_ID_MOV_SCALC(BillBlaster, AsmConsts::R32_EAX, AsmConsts::R32_EDX, AsmConsts::R32_EDI, &spawnHookBillBlaster);
		patch_SPAWN_ID_MOV_SCALC(BowserSMB3, AsmConsts::R32_EDX, AsmConsts::R32_ECX, AsmConsts::R32_EDI, &spawnHookBowserSMB3);
		//patch_SPAWN_ID_MOV_REGSAFE(FPropellerBlock, AsmConsts::R32_ESI, AsmConsts::R32_EDI,AsmConsts::R32_EAX, &spawnHookFPropellerBlock);
		patch_GENERATOR(Generator, &spawnHookGenerator);

		//patch_BLOCK(BlockSpawnProc, &spawnHookBlock);

		//patch_EGGANIM(EggAnim, &spawnHookEggAnim);

		//patch_SPAWN_ID_MOV_DCALC(PlayerHammer, AsmConsts::R32_EBX, AsmConsts::R32_ECX, AsmConsts::R32_EAX, &spawnHookPlayerHammer);
		//patch_SPAWN_ID_MOV_DCALC(PlayerFireball, AsmConsts::R32_EBX, AsmConsts::R32_EDX, AsmConsts::R32_ECX, &spawnHookPlayerFireball);
		//patch_SPAWN_ID_MOV(PlayerBomb, AsmConsts::R32_EBX, AsmConsts::R32_ESI, &spawnHookPlayerBomb);
		//patch_SPAWN_ID_MOV_DCALC(LinkShoot, AsmConsts::R32_EBX, AsmConsts::R32_ECX, AsmConsts::R32_EAX, &spawnHookLinkShoot);
		//patch_SPAWN_ID_MOV_DCALC(YoshiFire, AsmConsts::R32_ESI, AsmConsts::R32_EAX, AsmConsts::R32_EDX, &spawnHookYoshiFire);

		//patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi1, AsmConsts::R32_EBX, AsmConsts::R32_ESI,AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi2, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerYoshi3, AsmConsts::R32_EBX, AsmConsts::R32_EDI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerShoe1, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerShoe2, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerClown1, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerClown2, AsmConsts::R32_EBX, AsmConsts::R32_EDI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_SPAWN_ID_MOV_REGSAFE(PlayerClown3, AsmConsts::R32_EBX, AsmConsts::R32_ESI, AsmConsts::R32_EAX, &spawnHookPlayerMount);
		//patch_ITEMDROP(ItemDrop, &spawnHookItemDrop);
		keepAddr();
	}


	void mUnpatchY() {
		//generated code
		*(unsigned char*)(0xa5d501) = 0x66;
		*(unsigned char*)(0xa5d502) = 0x89;
		*(unsigned char*)(0xa5d503) = 0x87;
		*(unsigned char*)(0xa5d504) = 0xe2;
		*(unsigned char*)(0xa5d505) = 0x0;
		*(unsigned char*)(0xa5d506) = 0x0;
		*(unsigned char*)(0xa5d507) = 0x0;

		*(unsigned char*)(0xa599e5) = 0x66;
		*(unsigned char*)(0xa599e6) = 0xc7;
		*(unsigned char*)(0xa599e7) = 0x84;
		*(unsigned char*)(0xa599e8) = 0xc1;
		*(unsigned char*)(0xa599e9) = 0xe2;
		*(unsigned char*)(0xa599ea) = 0x0;
		*(unsigned char*)(0xa599eb) = 0x0;
		*(unsigned char*)(0xa599ec) = 0x0;
		*(unsigned char*)(0xa599ed) = 0x1e;
		*(unsigned char*)(0xa599ee) = 0x0;

		*(unsigned char*)(0xa55b68) = 0x66;
		*(unsigned char*)(0xa55b69) = 0xc7;
		*(unsigned char*)(0xa55b6a) = 0x87;
		*(unsigned char*)(0xa55b6b) = 0xe2;
		*(unsigned char*)(0xa55b6c) = 0x0;
		*(unsigned char*)(0xa55b6d) = 0x0;
		*(unsigned char*)(0xa55b6e) = 0x0;
		*(unsigned char*)(0xa55b6f) = 0xf6;
		*(unsigned char*)(0xa55b70) = 0x0;

		*(unsigned char*)(0xa5b593) = 0x66;
		*(unsigned char*)(0xa5b594) = 0xc7;
		*(unsigned char*)(0xa5b595) = 0x87;
		*(unsigned char*)(0xa5b596) = 0xe2;
		*(unsigned char*)(0xa5b597) = 0x0;
		*(unsigned char*)(0xa5b598) = 0x0;
		*(unsigned char*)(0xa5b599) = 0x0;
		*(unsigned char*)(0xa5b59a) = 0x30;
		*(unsigned char*)(0xa5b59b) = 0x0;

		*(unsigned char*)(0xa5f323) = 0x66;
		*(unsigned char*)(0xa5f324) = 0xc7;
		*(unsigned char*)(0xa5f325) = 0x87;
		*(unsigned char*)(0xa5f326) = 0xe2;
		*(unsigned char*)(0xa5f327) = 0x0;
		*(unsigned char*)(0xa5f328) = 0x0;
		*(unsigned char*)(0xa5f329) = 0x0;
		*(unsigned char*)(0xa5f32a) = 0x55;
		*(unsigned char*)(0xa5f32b) = 0x0;

		*(unsigned char*)(0xa5bd2f) = 0x66;
		*(unsigned char*)(0xa5bd30) = 0xc7;
		*(unsigned char*)(0xa5bd31) = 0x87;
		*(unsigned char*)(0xa5bd32) = 0xe2;
		*(unsigned char*)(0xa5bd33) = 0x0;
		*(unsigned char*)(0xa5bd34) = 0x0;
		*(unsigned char*)(0xa5bd35) = 0x0;
		*(unsigned char*)(0xa5bd36) = 0x28;
		*(unsigned char*)(0xa5bd37) = 0x0;

		*(unsigned char*)(0xa45fc7) = 0x66;
		*(unsigned char*)(0xa45fc8) = 0xc7;
		*(unsigned char*)(0xa45fc9) = 0x87;
		*(unsigned char*)(0xa45fca) = 0xe2;
		*(unsigned char*)(0xa45fcb) = 0x0;
		*(unsigned char*)(0xa45fcc) = 0x0;
		*(unsigned char*)(0xa45fcd) = 0x0;
		*(unsigned char*)(0xa45fce) = 0x14;
		*(unsigned char*)(0xa45fcf) = 0x1;

		*(unsigned char*)(0xa47723) = 0x66;
		*(unsigned char*)(0xa47724) = 0xc7;
		*(unsigned char*)(0xa47725) = 0x87;
		*(unsigned char*)(0xa47726) = 0xe2;
		*(unsigned char*)(0xa47727) = 0x0;
		*(unsigned char*)(0xa47728) = 0x0;
		*(unsigned char*)(0xa47729) = 0x0;
		*(unsigned char*)(0xa4772a) = 0xd;
		*(unsigned char*)(0xa4772b) = 0x1;

		*(unsigned char*)(0xa486b8) = 0x66;
		*(unsigned char*)(0xa486b9) = 0xc7;
		*(unsigned char*)(0xa486ba) = 0x87;
		*(unsigned char*)(0xa486bb) = 0xe2;
		*(unsigned char*)(0xa486bc) = 0x0;
		*(unsigned char*)(0xa486bd) = 0x0;
		*(unsigned char*)(0xa486be) = 0x0;
		*(unsigned char*)(0xa486bf) = 0x1a;
		*(unsigned char*)(0xa486c0) = 0x1;

		*(unsigned char*)(0xa49afc) = 0x66;
		*(unsigned char*)(0xa49afd) = 0xc7;
		*(unsigned char*)(0xa49afe) = 0x87;
		*(unsigned char*)(0xa49aff) = 0xe2;
		*(unsigned char*)(0xa49b00) = 0x0;
		*(unsigned char*)(0xa49b01) = 0x0;
		*(unsigned char*)(0xa49b02) = 0x0;
		*(unsigned char*)(0xa49b03) = 0x86;
		*(unsigned char*)(0xa49b04) = 0x0;

		*(unsigned char*)(0xa4b8e9) = 0x66;
		*(unsigned char*)(0xa4b8ea) = 0xc7;
		*(unsigned char*)(0xa4b8eb) = 0x84;
		*(unsigned char*)(0xa4b8ec) = 0xd0;
		*(unsigned char*)(0xa4b8ed) = 0xe2;
		*(unsigned char*)(0xa4b8ee) = 0x0;
		*(unsigned char*)(0xa4b8ef) = 0x0;
		*(unsigned char*)(0xa4b8f0) = 0x0;
		*(unsigned char*)(0xa4b8f1) = 0xd2;
		*(unsigned char*)(0xa4b8f2) = 0x0;

		*(unsigned char*)(0xa4be87) = 0x66;
		*(unsigned char*)(0xa4be88) = 0xc7;
		*(unsigned char*)(0xa4be89) = 0x84;
		*(unsigned char*)(0xa4be8a) = 0xca;
		*(unsigned char*)(0xa4be8b) = 0xe2;
		*(unsigned char*)(0xa4be8c) = 0x0;
		*(unsigned char*)(0xa4be8d) = 0x0;
		*(unsigned char*)(0xa4be8e) = 0x0;
		*(unsigned char*)(0xa4be8f) = 0xca;
		*(unsigned char*)(0xa4be90) = 0x0;

		*(unsigned char*)(0xa4cadc) = 0x66;
		*(unsigned char*)(0xa4cadd) = 0xc7;
		*(unsigned char*)(0xa4cade) = 0x84;
		*(unsigned char*)(0xa4cadf) = 0xd0;
		*(unsigned char*)(0xa4cae0) = 0xe2;
		*(unsigned char*)(0xa4cae1) = 0x0;
		*(unsigned char*)(0xa4cae2) = 0x0;
		*(unsigned char*)(0xa4cae3) = 0x0;
		*(unsigned char*)(0xa4cae4) = 0x1e;
		*(unsigned char*)(0xa4cae5) = 0x0;

		*(unsigned char*)(0xa4ce82) = 0x66;
		*(unsigned char*)(0xa4ce83) = 0xc7;
		*(unsigned char*)(0xa4ce84) = 0x87;
		*(unsigned char*)(0xa4ce85) = 0xe2;
		*(unsigned char*)(0xa4ce86) = 0x0;
		*(unsigned char*)(0xa4ce87) = 0x0;
		*(unsigned char*)(0xa4ce88) = 0x0;
		*(unsigned char*)(0xa4ce89) = 0x57;
		*(unsigned char*)(0xa4ce8a) = 0x0;

		*(unsigned char*)(0xa4e592) = 0x66;
		*(unsigned char*)(0xa4e593) = 0xc7;
		*(unsigned char*)(0xa4e594) = 0x86;
		*(unsigned char*)(0xa4e595) = 0xe2;
		*(unsigned char*)(0xa4e596) = 0x0;
		*(unsigned char*)(0xa4e597) = 0x0;
		*(unsigned char*)(0xa4e598) = 0x0;
		*(unsigned char*)(0xa4e599) = 0x85;
		*(unsigned char*)(0xa4e59a) = 0x0;

		*(unsigned char*)(0xa5282d) = 0x66;
		*(unsigned char*)(0xa5282e) = 0xc7;
		*(unsigned char*)(0xa5282f) = 0x87;
		*(unsigned char*)(0xa52830) = 0xe2;
		*(unsigned char*)(0xa52831) = 0x0;
		*(unsigned char*)(0xa52832) = 0x0;
		*(unsigned char*)(0xa52833) = 0x0;
		*(unsigned char*)(0xa52834) = 0xd6;
		*(unsigned char*)(0xa52835) = 0x0;

		*(unsigned char*)(0xa52876) = 0x66;
		*(unsigned char*)(0xa52877) = 0xc7;
		*(unsigned char*)(0xa52878) = 0x87;
		*(unsigned char*)(0xa52879) = 0xe2;
		*(unsigned char*)(0xa5287a) = 0x0;
		*(unsigned char*)(0xa5287b) = 0x0;
		*(unsigned char*)(0xa5287c) = 0x0;
		*(unsigned char*)(0xa5287d) = 0xd5;
		*(unsigned char*)(0xa5287e) = 0x0;

		*(unsigned char*)(0xa528bb) = 0x66;
		*(unsigned char*)(0xa528bc) = 0xc7;
		*(unsigned char*)(0xa528bd) = 0x87;
		*(unsigned char*)(0xa528be) = 0xe2;
		*(unsigned char*)(0xa528bf) = 0x0;
		*(unsigned char*)(0xa528c0) = 0x0;
		*(unsigned char*)(0xa528c1) = 0x0;
		*(unsigned char*)(0xa528c2) = 0xe0;
		*(unsigned char*)(0xa528c3) = 0x0;

		*(unsigned char*)(0xa2175d) = 0x66;
		*(unsigned char*)(0xa2175e) = 0xc7;
		*(unsigned char*)(0xa2175f) = 0x87;
		*(unsigned char*)(0xa21760) = 0xe2;
		*(unsigned char*)(0xa21761) = 0x0;
		*(unsigned char*)(0xa21762) = 0x0;
		*(unsigned char*)(0xa21763) = 0x0;
		*(unsigned char*)(0xa21764) = 0x11;
		*(unsigned char*)(0xa21765) = 0x0;

		*(unsigned char*)(0xa1f3f4) = 0x66;
		*(unsigned char*)(0xa1f3f5) = 0xc7;
		*(unsigned char*)(0xa1f3f6) = 0x87;
		*(unsigned char*)(0xa1f3f7) = 0xe2;
		*(unsigned char*)(0xa1f3f8) = 0x0;
		*(unsigned char*)(0xa1f3f9) = 0x0;
		*(unsigned char*)(0xa1f3fa) = 0x0;
		*(unsigned char*)(0xa1f3fb) = 0x57;
		*(unsigned char*)(0xa1f3fc) = 0x0;

		*(unsigned char*)(0xa07bf0) = 0xc7;
		*(unsigned char*)(0xa07bf1) = 0x85;
		*(unsigned char*)(0xa07bf2) = 0x20;
		*(unsigned char*)(0xa07bf3) = 0xfb;
		*(unsigned char*)(0xa07bf4) = 0xff;
		*(unsigned char*)(0xa07bf5) = 0xff;
		*(unsigned char*)(0xa07bf6) = 0x0;
		*(unsigned char*)(0xa07bf7) = 0x0;
		*(unsigned char*)(0xa07bf8) = 0x0;
		*(unsigned char*)(0xa07bf9) = 0x0;

		*(unsigned char*)(0x9cd544) = 0x66;
		*(unsigned char*)(0x9cd545) = 0xc7;
		*(unsigned char*)(0x9cd546) = 0x87;
		*(unsigned char*)(0x9cd547) = 0xe2;
		*(unsigned char*)(0x9cd548) = 0x0;
		*(unsigned char*)(0x9cd549) = 0x0;
		*(unsigned char*)(0x9cd54a) = 0x0;
		*(unsigned char*)(0x9cd54b) = 0x6c;
		*(unsigned char*)(0x9cd54c) = 0x0;

		*(unsigned char*)(0x9dc981) = 0x89;
		*(unsigned char*)(0x9dc982) = 0x9d;
		*(unsigned char*)(0x9dc983) = 0x70;
		*(unsigned char*)(0x9dc984) = 0xff;
		*(unsigned char*)(0x9dc985) = 0xff;
		*(unsigned char*)(0x9dc986) = 0xff;

		*(unsigned char*)(0x9e64b4) = 0xa1;
		*(unsigned char*)(0x9e64b5) = 0xc4;
		*(unsigned char*)(0x9e64b6) = 0x5b;
		*(unsigned char*)(0x9e64b7) = 0xb2;
		*(unsigned char*)(0x9e64b8) = 0x0;

		*(unsigned char*)(0x9c6d23) = 0x66;
		*(unsigned char*)(0x9c6d24) = 0xc7;
		*(unsigned char*)(0x9c6d25) = 0x84;
		*(unsigned char*)(0x9c6d26) = 0xc1;
		*(unsigned char*)(0x9c6d27) = 0x36;
		*(unsigned char*)(0x9c6d28) = 0x1;
		*(unsigned char*)(0x9c6d29) = 0x0;
		*(unsigned char*)(0x9c6d2a) = 0x0;
		*(unsigned char*)(0x9c6d2b) = 0xff;
		*(unsigned char*)(0x9c6d2c) = 0xff;

		*(unsigned char*)(0x9c7d2d) = 0x66;
		*(unsigned char*)(0x9c7d2e) = 0xc7;
		*(unsigned char*)(0x9c7d2f) = 0x84;
		*(unsigned char*)(0x9c7d30) = 0xca;
		*(unsigned char*)(0x9c7d31) = 0x36;
		*(unsigned char*)(0x9c7d32) = 0x1;
		*(unsigned char*)(0x9c7d33) = 0x0;
		*(unsigned char*)(0x9c7d34) = 0x0;
		*(unsigned char*)(0x9c7d35) = 0xff;
		*(unsigned char*)(0x9c7d36) = 0xff;

		*(unsigned char*)(0x908f6a) = 0x8d;
		*(unsigned char*)(0x908f6b) = 0x55;
		*(unsigned char*)(0x908f6c) = 0xa8;
		*(unsigned char*)(0x908f6d) = 0x51;
		*(unsigned char*)(0x908f6e) = 0x52;
		*(unsigned char*)(0x908f6f) = 0x6a;
		*(unsigned char*)(0x908f70) = 0x2;
		*(unsigned char*)(0x908f71) = 0xff;
		*(unsigned char*)(0x908f72) = 0x15;

		*(unsigned char*)(0x99fbe4) = 0x66;
		*(unsigned char*)(0x99fbe5) = 0xc7;
		*(unsigned char*)(0x99fbe6) = 0x84;
		*(unsigned char*)(0x99fbe7) = 0xc1;
		*(unsigned char*)(0x99fbe8) = 0x36;
		*(unsigned char*)(0x99fbe9) = 0x1;
		*(unsigned char*)(0x99fbea) = 0x0;
		*(unsigned char*)(0x99fbeb) = 0x0;
		*(unsigned char*)(0x99fbec) = 0xff;
		*(unsigned char*)(0x99fbed) = 0xff;

		*(unsigned char*)(0x99bce6) = 0x66;
		*(unsigned char*)(0x99bce7) = 0xc7;
		*(unsigned char*)(0x99bce8) = 0x86;
		*(unsigned char*)(0x99bce9) = 0x2a;
		*(unsigned char*)(0x99bcea) = 0x1;
		*(unsigned char*)(0x99bceb) = 0x0;
		*(unsigned char*)(0x99bcec) = 0x0;
		*(unsigned char*)(0x99bced) = 0x64;
		*(unsigned char*)(0x99bcee) = 0x0;

		*(unsigned char*)(0x99d0d9) = 0x66;
		*(unsigned char*)(0x99d0da) = 0xc7;
		*(unsigned char*)(0x99d0db) = 0x86;
		*(unsigned char*)(0x99d0dc) = 0x2a;
		*(unsigned char*)(0x99d0dd) = 0x1;
		*(unsigned char*)(0x99d0de) = 0x0;
		*(unsigned char*)(0x99d0df) = 0x0;
		*(unsigned char*)(0x99d0e0) = 0x64;
		*(unsigned char*)(0x99d0e1) = 0x0;

		*(unsigned char*)(0x9b59e8) = 0x66;
		*(unsigned char*)(0x9b59e9) = 0xc7;
		*(unsigned char*)(0x9b59ea) = 0x87;
		*(unsigned char*)(0x9b59eb) = 0x2a;
		*(unsigned char*)(0x9b59ec) = 0x1;
		*(unsigned char*)(0x9b59ed) = 0x0;
		*(unsigned char*)(0x9b59ee) = 0x0;
		*(unsigned char*)(0x9b59ef) = 0x64;
		*(unsigned char*)(0x9b59f0) = 0x0;

		*(unsigned char*)(0x99cc30) = 0x66;
		*(unsigned char*)(0x99cc31) = 0xc7;
		*(unsigned char*)(0x99cc32) = 0x86;
		*(unsigned char*)(0x99cc33) = 0x2a;
		*(unsigned char*)(0x99cc34) = 0x1;
		*(unsigned char*)(0x99cc35) = 0x0;
		*(unsigned char*)(0x99cc36) = 0x0;
		*(unsigned char*)(0x99cc37) = 0x64;
		*(unsigned char*)(0x99cc38) = 0x0;

		*(unsigned char*)(0x99b8be) = 0x66;
		*(unsigned char*)(0x99b8bf) = 0xc7;
		*(unsigned char*)(0x99b8c0) = 0x86;
		*(unsigned char*)(0x99b8c1) = 0x2a;
		*(unsigned char*)(0x99b8c2) = 0x1;
		*(unsigned char*)(0x99b8c3) = 0x0;
		*(unsigned char*)(0x99b8c4) = 0x0;
		*(unsigned char*)(0x99b8c5) = 0x64;
		*(unsigned char*)(0x99b8c6) = 0x0;

		*(unsigned char*)(0x998719) = 0x66;
		*(unsigned char*)(0x99871a) = 0xc7;
		*(unsigned char*)(0x99871b) = 0x86;
		*(unsigned char*)(0x99871c) = 0x2a;
		*(unsigned char*)(0x99871d) = 0x1;
		*(unsigned char*)(0x99871e) = 0x0;
		*(unsigned char*)(0x99871f) = 0x0;
		*(unsigned char*)(0x998720) = 0x64;
		*(unsigned char*)(0x998721) = 0x0;

		*(unsigned char*)(0x9b60fa) = 0x66;
		*(unsigned char*)(0x9b60fb) = 0xc7;
		*(unsigned char*)(0x9b60fc) = 0x87;
		*(unsigned char*)(0x9b60fd) = 0x2a;
		*(unsigned char*)(0x9b60fe) = 0x1;
		*(unsigned char*)(0x9b60ff) = 0x0;
		*(unsigned char*)(0x9b6100) = 0x0;
		*(unsigned char*)(0x9b6101) = 0x64;
		*(unsigned char*)(0x9b6102) = 0x0;

		*(unsigned char*)(0x9b6b1d) = 0x66;
		*(unsigned char*)(0x9b6b1e) = 0xc7;
		*(unsigned char*)(0x9b6b1f) = 0x86;
		*(unsigned char*)(0x9b6b20) = 0x2a;
		*(unsigned char*)(0x9b6b21) = 0x1;
		*(unsigned char*)(0x9b6b22) = 0x0;
		*(unsigned char*)(0x9b6b23) = 0x0;
		*(unsigned char*)(0x9b6b24) = 0x64;
		*(unsigned char*)(0x9b6b25) = 0x0;

		*(unsigned char*)(0x9bf0cc) = 0x66;
		*(unsigned char*)(0x9bf0cd) = 0xc7;
		*(unsigned char*)(0x9bf0ce) = 0x84;
		*(unsigned char*)(0x9bf0cf) = 0xd0;
		*(unsigned char*)(0x9bf0d0) = 0xe2;
		*(unsigned char*)(0x9bf0d1) = 0x0;
		*(unsigned char*)(0x9bf0d2) = 0x0;
		*(unsigned char*)(0x9bf0d3) = 0x0;
		*(unsigned char*)(0x9bf0d4) = 0x6c;
		*(unsigned char*)(0x9bf0d5) = 0x0;

		*(unsigned char*)(0xa24478) = 0x66;
		*(unsigned char*)(0xa24479) = 0x89;
		*(unsigned char*)(0xa2447a) = 0x84;
		*(unsigned char*)(0xa2447b) = 0xd1;
		*(unsigned char*)(0xa2447c) = 0xe2;
		*(unsigned char*)(0xa2447d) = 0x0;
		*(unsigned char*)(0xa2447e) = 0x0;
		*(unsigned char*)(0xa2447f) = 0x0;


	}

	void registerSpawnEvent(void* func) {
		spawnCallback.push_back(func);
	}

	void clearCallback() {
		spawnCallback.clear();
	}

	bool WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved) {
		/*
		if (fdwReason == DLL_PROCESS_ATTACH ){
		mPatchE();
		}
		*/
		if (fdwReason == DLL_PROCESS_DETACH) {

			mUnpatchY();
		}
		return true;
	}
}
