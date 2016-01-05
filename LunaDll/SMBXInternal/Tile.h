#ifndef Tile_hhhhh
#define Tile_hhhhh

#include "../Defines.h"
#include "BaseItemArray.h"

#pragma pack(push, 1)
struct SMBXTile : SMBX_FullBaseItemArray<SMBXTile, 328, GM_TILE_COUNT_CONSTPTR, GM_TILE_PTR_CONSTPTR>
{
    Momentum momentum;
    short id;
    short field_32;
};
#pragma pack(pop)

/* Verify struct is correctly sized */
#ifndef __INTELLISENSE__
static_assert(sizeof(SMBXTile) == 0x34, "sizeof(SMBXTile) must be 0x34");
#endif

#endif

