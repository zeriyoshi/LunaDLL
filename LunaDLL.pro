TEMPLATE = lib

CONFIG += dll
CONFIG -= app_bundle
CONFIG -= qt

TARGET = LunaDLL
#OBJECTS_DIR = release/obj

DEFINES += LunaDLL_LIBRARY LunaDLL_EXPORTS _USRDLL
DEFINES += DX_EXPORT=Q_DECL_EXPORT NDEBUG _WINDOWS _USRDLL LUNADLL_EXPORTS

INCLUDEPATH += LunaDLL LunaDll/libs/luabind-include

CONFIG += create_prl
CONFIG += link_prl

LIBS += kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib Gdiplus.lib
LIBS += -L$$PWD/LunaDll/libs/luabind-lib/ -llibluabind

INCLUDEPATH += $$PWD/LunaDll/libs/luabind-include
DEPENDPATH += $$PWD/LunaDll/libs/luabind-include

SOURCES += \
    LunaDll/AC_HeartSystem.cpp \
    LunaDll/AC_LunaControl.cpp \
    LunaDll/Autocode.cpp \
    LunaDll/AutocodeManager.cpp \
    LunaDll/Blocks.cpp \
    LunaDll/BMPBox.cpp \
    LunaDll/CellManager.cpp \
    LunaDll/CSprite.cpp \
    LunaDll/CSpriteManager.cpp \
    LunaDll/DeathCounter.cpp \
    LunaDll/DeathRecord.cpp \
    LunaDll/dlltestlvlCode.cpp \
    LunaDll/Docopoper-AbstractAssault.lvl.cpp \
    LunaDll/Docopoper-Calleoca.cpp \
    LunaDll/Docopoper-TheFloorisLava.lvl..cpp \
    LunaDll/EnumCmd.cpp \
    LunaDll/EuroShellRandD.lvl.cpp \
    LunaDll/GenComp.cpp \
    LunaDll/Globals.cpp \
    LunaDll/Hitbox.cpp \
    LunaDll/Input.cpp \
    LunaDll/JosephStaleknight-CurtainTortoise.lvl.cpp \
    LunaDll/Keyboard.cpp \
    LunaDll/Kil-DemosBrain.cpp \
    LunaDll/Layer.cpp \
    LunaDll/Level.cpp \
    LunaDll/Logger.cpp \
    LunaDll/Main.cpp \
    LunaDll/MiscFuncs.cpp \
    LunaDll/NPCs.cpp \
    LunaDll/PerformanceTimer.cpp \
    LunaDll/PlayerMOB.cpp \
    LunaDll/RenderBitmapOp.cpp \
    LunaDll/Rendering.cpp \
    LunaDll/RenderRectOp.cpp \
    LunaDll/SAJewers-QraestoliaCaverns.lvl.cpp \
    LunaDll/SAJewers-Snowboardin.cpp \
    LunaDll/SMBXEvent.cpp \
    LunaDll/SMBXEvents.cpp \
    LunaDll/Sound.cpp \
    LunaDll/SpriteBehaviorFuncs.cpp \
    LunaDll/SpriteComponent.cpp \
    LunaDll/SpriteDrawFuncs.cpp \
    LunaDll/Talkhaus-Science_Final_Battle.cpp \
    LunaDll/LuaMain/LunaLuaMain.cpp

HEADERS += \
    LunaDll/Autocode.h \
    LunaDll/AutocodeManager.h \
    LunaDll/Blocks.h \
    LunaDll/BMPBox.h \
    LunaDll/CellManager.h \
    LunaDll/CSprite.h \
    LunaDll/CSpriteManager.h \
    LunaDll/DeathCounter.h \
    LunaDll/DeathRecord.h \
    LunaDll/Defines.h \
    LunaDll/Globals.h \
    LunaDll/Hitbox.h \
    LunaDll/Input.h \
    LunaDll/Keyboard.h \
    LunaDll/Layer.h \
    LunaDll/Level.h \
    LunaDll/LevelCodes.h \
    LunaDll/Logging.h \
    LunaDll/Main.h \
    LunaDll/MiscFuncs.h \
    LunaDll/NPCs.h \
    LunaDll/PerformanceTimer.h \
    LunaDll/PlayerMOB.h \
    LunaDll/RenderBitmapOp.h \
    LunaDll/Rendering.h \
    LunaDll/RenderOp.h \
    LunaDll/RenderRectOp.h \
    LunaDll/RenderString.h \
    LunaDll/SMBXEvents.h \
    LunaDll/Sound.h \
    LunaDll/SpriteComponent.h \
    LunaDll/SpriteFuncs.h \
    LunaDll/TriggerZone.h \
    LunaDll/LuaMain/LunaLuaMain.h

OTHER_FILES += \
    LunaDll/scratch.txt

