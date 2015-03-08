#ifndef Animations_hhhh
#define Animations_hhhh
#include "../Defines.h"

enum EffectID : short
{
    EFFECTID_BRICK_FRAGMENTS = 1,
    EFFECTID_SQUASHED_GOOMBA = 2,
    EFFECTID_MARIO = 3,
    EFFECTID_GOOMBA = 4,
    EFFECTID_LUIGI = 5,
    EFFECTID_SQUASHED_RED_GOOMBA = 6,
    EFFECTID_RED_GOOMBA = 7,
    EFFECTID_GREEN_SHELL = 8,
    EFFECTID_RED_SHELL = 9,
    EFFECTID_SMOKE = 10,
    EFFECTID_KICKED_COIN = 11,
    EFFECTID_PODOBOO_JUMPING = 12,
    EFFECTID_LAVA_SPLASH = 13,
    EFFECTID_BOOM__BOOM_KICKED = 14,
    EFFECTID_BULLET_BILL = 15,
    EFFECTID_BANZAI_BILL = 16,
    EFFECTID_RED_SHY_GUY = 17,
    EFFECTID_BLUE_SHY_GUY = 18,
    EFFECTID_BUZZY_BEETLES_SHELL = 19,
    EFFECTID_NINJI = 20,
    EFFECTID_BRICK_FRAGMENTS_2 = 21,
    EFFECTID_BLUE_GOOMBA = 22,
    EFFECTID_SQUASHED_BLUE_GOOMBA = 23,
    EFFECTID_RED_CHEEP_CHEEP = 24,
    EFFECTID_HAMMER_BROS = 25,
    EFFECTID_KURIBOS_SHOE = 26,
    EFFECTID_SPINIY = 27,
    EFFECTID_BIRDOS_EGG = 28,
    EFFECTID_BIRDOS_DEATH = 29,
    EFFECTID_ICE_BLOCKS_FRAGMENTS = 30,
    EFFECTID_LAKITUS_BALL = 31,
    EFFECTID_LAKITU_SMB3 = 32,
    EFFECTID_CRAB = 33,
    EFFECTID_FIGHTER_FLY = 34,
    EFFECTID_SQUASHED_SMW_BEACH_KOOPA_ = 35,
    EFFECTID_SMW_BEACH_KOOPA_ = 36,
    EFFECTID_SQUASHED_YELLOW_SWITH_GOOMBA = 37,
    EFFECTID_YELLOW_SWITH_GOOMBA = 38,
    EFFECTID_SQUASHED_BLUE_SWITH_GOOMBA = 39,
    EFFECTID_BLUE_SWITH_GOOMBA = 40,
    EFFECTID_SQUASHED_GREEN_SWITH_GOOMBA = 41,
    EFFECTID_GREEN_SWITH_GOOMBA = 42,
    EFFECTID_SQUASHED_RED_SWITH_GOOMBA = 43,
    EFFECTID_RED_SWITH_GOOMBA = 44,
    EFFECTID_SQUASHED_GRAND_GOOMBA = 45,
    EFFECTID_GRAND_GOOMBA = 46,
    EFFECTID_GRAND_KOOPAS_SHELL = 47,
    EFFECTID_MUSHROOM = 48,
    EFFECTID_NINJI_2 = 49,
    EFFECTID_SMB3_BOWSER = 50,
    EFFECTID_BRICK_FRAGMENTS_3 = 51,
    EFFECTID_SQUASHED_SMB1_BROWN_GOOMBA = 52,
    EFFECTID_SMB1_BROWN_GOOMBA = 53,
    EFFECTID_SMB2_DOOR_DROPABLE = 54,
    EFFECTID_SMB2_DOOR = 55,
    EFFECTID_YOSHIS_EGGS = 56,
    EFFECTID_YOSHIS_EGGS_FRAGMENTS = 57,
    EFFECTID_APPEARANCE_YOSHIS = 58,
    EFFECTID_SMB3_DOOR = 59,
    EFFECTID_LYING_KOOPAS_SHELLS = 60,
    EFFECTID_KICKED_SMW_BEACH_KOOPA = 61,
    EFFECTID_SQUASHED_SMW_BEACH_KOOPAS = 62,
    EFFECTID_FAIRY_MAGIC_SMOKE = 63,
    EFFECTID_TWITTER = 64,
    EFFECTID_RED_SNIFIT = 65,
    EFFECTID_BLUE_SNIFIT = 66,
    EFFECTID_GRAY_SNIFIT = 67,
    EFFECTID_SNIFITS_BULLET = 68,
    EFFECTID_SMB2_EXPLOSION = 69,
    EFFECTID_EMPTY_EFFECT = 70,
    EFFECTID_SMB3_BOB_OMB_EXPLOSION = 71,
    EFFECTID_KICKED_SMB3_BOB_OMB = 72,
    EFFECTID_TAIL_BEATING = 73,
    EFFECTID_PLAYERS_FRICTION = 74,
    EFFECTID_BEAT = 75,
    EFFECTID_SPIN_SPARK = 76,
    EFFECTID_FIREBALLS_PLAYER = 77,
    EFFECTID_GLOSS_OF_COINS = 78,
    EFFECTID_POINTS = 79,
    EFFECTID_ICE_GLOSS = 80,
    EFFECTID_PUSHED_P_SWITCH = 81,
    EFFECTID_SMW_ROTATING_BLOCK = 82,
    EFFECTID_MR_SATURN = 83,
    EFFECTID_SQUASHED_KICKED_REX = 84,
    EFFECTID_KICKED_REX = 85,
    EFFECTID_REX = 86,
    EFFECTID_MEGA_MOLE = 87,
    EFFECTID_GALOOMBA = 88,
    EFFECTID_BULLY = 89,
    EFFECTID_THWOMP = 90,
    EFFECTID_SMW_BIG_BOO = 91,
    EFFECTID_SMW_BOO = 92,
    EFFECTID_EERIE = 93,
    EFFECTID_SMB3_BOO = 94,
    EFFECTID_SMB1_LYING_GREEN_KOOPAS_SHELL = 95,
    EFFECTID_SMB1_LYING_RED_KOOPAS_SHELL = 96,
    EFFECTID_DRY_BONES = 97,
    EFFECTID_SMW_THWOMP = 98,
    EFFECTID_CIRCULAR_SAW = 99,
    EFFECTID_BRICK_FRAGMENTS_5 = 100,
    EFFECTID_PODOBOOS_SHOE = 101,
    EFFECTID_LAKITUS_SHOE = 102,
    EFFECTID_SMW_GHOST_HOUSE_DOOR = 103,
    EFFECTID_BLARGGS_EYES = 104,
    EFFECTID_SMB1_BOWSER = 105,
    EFFECTID_WART = 106,
    EFFECTID_BREAKED_BLOCK = 107,
    EFFECTID_RINKA_EXPLOSION = 108,
    EFFECTID_SPIKE_TOP = 109,
    EFFECTID_SPARK = 110,
    EFFECTID_GLASS_SPLINTERS = 111,
    EFFECTID_MOTHER_BRAIN = 112,
    EFFECTID_UNDERWATER_BUBLES = 113,
    EFFECTID_WATER_SPLASH = 114,
    EFFECTID_SMB3_GREEN_CHEEP_CHEEP = 115,
    EFFECTID_SMB3_RED_CHEEP_CHEEP = 116,
    EFFECTID_SMB3_BLOOPER = 117,
    EFFECTID_BLURP = 118,
    EFFECTID_SMB1_GREEN_CHEEP_CHEEP = 119,
    EFFECTID_BONE_FISH = 120,
    EFFECTID_SMB1_BLOOPER = 121,
    EFFECTID_BLURP_2 = 122,
    EFFECTID_GREEN_P_SWITCH_PUSHED = 123,
    EFFECTID_DYNAMIT_SWITCHER = 124,
    EFFECTID_POW_BLOCK = 125,
    EFFECTID_SQUASHED_MISC_GOOMBA = 126,
    EFFECTID_MISC_GOOMBA = 127,
    EFFECTID_POKEY = 128,
    EFFECTID_PEACH = 129,
    EFFECTID_TOAD = 130,
    EFFECTID_TANOOKI__RACOON_TRANSFORM = 131,
    EFFECTID_SHELL_KICK_SPARK = 132,
    EFFECTID_SHELL_KICK_SPARK_2 = 133,
    EFFECTID_LINK = 134,
    EFFECTID_BRICK_FRAGMENTS_4 = 135,
    EFFECTID_ROTO_DISK = 136,
    EFFECTID_NIPPER_PLANT = 137,
    EFFECTID_MOUSER = 138,
    EFFECTID_LINKS_ICEBALL_GLOSS = 139,
    EFFECTID_LARRY_KOOPA = 140,
    EFFECTID_SWOOPER = 141,
    EFFECTID_HOOPSTER = 142,
    EFFECTID_LUDWIG_VON_KOOPA = 143,
    EFFECTID_BUBBLE_POP = 144,
    EFFECTID_SMW_LAKITU = 145,
    EFFECTID_SPINY = 146,
    EFFECTID_POTION_TO_DOOR_TRANSFORM = 147,
    EFFECTID_PEACHS_BOMB_EXPLOSION = 148
};


#pragma pack(push, 4)
struct SMBXAnimation{
    EffectID AnimationID;
    short unknown02;
    Momentum momentum;
    short unknown34; //Subtimer?
    short unknown36;
    short unknown38;
    short unknown3A;
    short Timer;
    short unknown3E;
    short unknown40;
    short unknown42;
};
#pragma pack(pop)


namespace Animations {

    SMBXAnimation* Get(int AnimationIndex);

}

#endif
