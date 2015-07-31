#include "../LuaProxy.h"
#include "../../SMBXInternal/Overworld.h"
#include "../../Misc/MiscFuncs.h"

LuaProxy::World::World()
{}

double LuaProxy::World::playerX() const
{
	return SMBXOverworld::get()->XPos;
}

void LuaProxy::World::setPlayerX(double playerX)
{
	SMBXOverworld::get()->XPos = playerX;
}

double LuaProxy::World::playerY() const
{
	return SMBXOverworld::get()->YPos;
}

void LuaProxy::World::setPlayerY(double playerY)
{
	SMBXOverworld::get()->YPos = playerY;
}

void LuaProxy::World::mem(int offset, L_FIELDTYPE ftype, const luabind::object &value, lua_State* L)
{
	Overworld* pOverworld = ::SMBXOverworld::get();
	void* ptr = ((&(*(byte*)pOverworld)) + offset);
	LuaProxy::mem((int)ptr, ftype, value, L);
}

luabind::object LuaProxy::World::mem(int offset, L_FIELDTYPE ftype, lua_State* L) const
{
	Overworld* pOverworld = ::SMBXOverworld::get();
	void* ptr = ((&(*(byte*)pOverworld)) + offset);
	return LuaProxy::mem((int)ptr, ftype, L);
}

short LuaProxy::World::currentWalkingDirection() const
{
	return SMBXOverworld::get()->currentWalkingDirection;
}

void LuaProxy::World::setCurrentWalkingDirection(short currentWalkingDirection)
{
	SMBXOverworld::get()->currentWalkingDirection = currentWalkingDirection;
}

short LuaProxy::World::currentWalkingFrame() const
{
	return SMBXOverworld::get()->currentWalkingFrame;
}

void LuaProxy::World::setCurrentWalkingFrame(short currentWalkingFrame)
{
	SMBXOverworld::get()->currentWalkingFrame = currentWalkingFrame;
}

short LuaProxy::World::currentWalkingFrameTimer() const
{
	return SMBXOverworld::get()->currentWalkingFrameTimer;
}

void LuaProxy::World::setCurrentWalkingFrameTimer(short currentWalkingFrameTimer)
{
	SMBXOverworld::get()->currentWalkingFrameTimer = currentWalkingFrameTimer;
}

short LuaProxy::World::currentWalkingTimer() const
{
	return SMBXOverworld::get()->currentWalkingTimer;
}

void LuaProxy::World::setCurrentWalkingTimer(short currentWalkingTimer)
{
	SMBXOverworld::get()->currentWalkingTimer = currentWalkingTimer;
}

bool LuaProxy::World::playerIsCurrentWalking() const
{
	return (SMBXOverworld::get()->currentWalkingDirection || SMBXOverworld::get()->isCurrentlyWalking ? true : false);
}

luabind::object LuaProxy::World::levelTitle(lua_State* L)
{
	if(SMBXOverworld::get()->currentLevelTitle.ptr[0] && SMBXOverworld::get()->currentLevelTitle.ptr != (wchar_t*)0x423D00){
		return luabind::object(L, VBStr(SMBXOverworld::get()->currentLevelTitle.ptr));
	}
	return luabind::object();
}

short LuaProxy::World::getCurrentDirection() const
{
	short val = SMBXOverworld::get()->currentWalkingFrame;
	if(val == 0 || val == 1)
		return 3;
	if(val == 2 || val == 3)
		return 4;
	if(val == 4 || val == 5)
		return 2;
	if(val == 6 || val == 7)
		return 1;

	return 0;
}

short LuaProxy::World::playerPowerup() const
{
    return SMBXOverworld::get()->currentPowerup;
}

void LuaProxy::World::setPlayerPowerup(short playerPowerup)
{
    SMBXOverworld::get()->currentPowerup = playerPowerup;
}

