#include "../../LuaProxy.h"
#include "../../../Globals.h"
#include "../../../GlobalFuncs.h"
#include "../../../Rendering/Rendering.h"

// Stores reference to a loaded image
LuaProxy::Graphics::LuaImageResource::LuaImageResource(int imgResource) {
    this->imgResource = imgResource;
}

// Deconstructor for when a loaded image resource is no longer referenced by Lua
LuaProxy::Graphics::LuaImageResource::~LuaImageResource() {
    gLunaRender.DeleteImage(imgResource);
}

void LuaProxy::Graphics::activateHud(bool activate)
{
    gSkipSMBXHUD = !activate;
}

LuaProxy::Graphics::LuaImageResource* LuaProxy::Graphics::loadImage(const char* filename)
{
    int resNumber = gLunaRender.GetAutoImageResourceCode();
    if (resNumber == 0) return NULL;

    if (!gLunaRender.LoadBitmapResource(utf8_decode(std::string(filename)), resNumber)) {
        // If image loading failed, return null
        return NULL;
    }

    // Allocate a LuaImageResource to allow us to automatically garbage collect the image when no longer referenced in Lua
    return new LuaProxy::Graphics::LuaImageResource(resNumber);
}

bool LuaProxy::Graphics::loadImage(const char* filename, int resNumber, int transColor)
{
    return gLunaRender.LoadBitmapResource(utf8_decode(std::string(filename)), resNumber, transColor);
}


void LuaProxy::Graphics::placeSprite(int type, int imgResource, int xPos, int yPos, const char *extra, int time)
{
    CSpriteRequest req;
    req.type = type;
    req.img_resource_code = imgResource;
    req.x = xPos;
    req.y = yPos;
    req.time = time;
    req.str = utf8_decode(std::string(extra));
    gSpriteMan.InstantiateSprite(&req, false);
}

void LuaProxy::Graphics::placeSprite(int type, int imgResource, int xPos, int yPos, const char *extra)
{
    placeSprite(type, imgResource, xPos, yPos, extra, 0);
}


void LuaProxy::Graphics::placeSprite(int type, int imgResource, int xPos, int yPos)
{
    placeSprite(type, imgResource, xPos, yPos, "");
}

void LuaProxy::Graphics::placeSprite(int type, const LuaProxy::Graphics::LuaImageResource& img, int xPos, int yPos, const char *extra, int time)
{
    placeSprite(type, img.imgResource, xPos, yPos, extra, time);
}

void LuaProxy::Graphics::placeSprite(int type, const LuaProxy::Graphics::LuaImageResource& img, int xPos, int yPos, const char *extra)
{
    placeSprite(type, img.imgResource, xPos, yPos, extra, 0);
}

void LuaProxy::Graphics::placeSprite(int type, const LuaProxy::Graphics::LuaImageResource& img, int xPos, int yPos)
{
    placeSprite(type, img.imgResource, xPos, yPos, "");
}

void LuaProxy::Graphics::glSetTexture(const LuaImageResource* img, uint32_t color)
{   
    // Convert RGB to RGBA
    LuaProxy::Graphics::glSetTextureRGBA(img, (color << 8) | 0xFF);
}

void LuaProxy::Graphics::glSetTextureRGBA(const LuaImageResource* img, uint32_t color)
{
    const BMPBox* bmp = NULL;
    if (img) {
        auto it = gLunaRender.LoadedImages.find(img->imgResource);
        if (it != gLunaRender.LoadedImages.end()) {
            bmp = it->second;
        }
    }

    gLunaRender.GLCmd(GLEngineCmd::SetTex(bmp, color));
}

extern "C" {
    __declspec(dllexport) float* __cdecl LunaLuaGlAllocCoords(size_t size) {
        return (float*)malloc(size * sizeof(float));
    }

    __declspec(dllexport) void __cdecl LunaLuaGlDrawTriangles(const float* vert, const float* tex, unsigned int count) {
        gLunaRender.GLCmd(GLEngineCmd::DrawTriangles(vert, tex, count));
    }
}