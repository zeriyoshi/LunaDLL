#include "LuaProxyAudio.h"

#include "../../SdlMusic/MusicManager.h"
#include "../../SdlMusic/SdlMusPlayer.h"
#include "../LuaProxy.h"
#include "../../Misc/MiscFuncs.h"
#include "../../GlobalFuncs.h"
#include "../../MciEmulator/mciEmulator.h"

#include "../../Level/Level.h"

void LuaProxy::Audio::MusicOpen(const char *filename)
{
#ifndef NO_SDL
    std::string full_paths = getSfxPath(std::string(filename));
    PGE_MusPlayer::MUS_openFile(full_paths.c_str());
#endif
}


void LuaProxy::Audio::MusicPlay()
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_playMusic();
#endif
}


void LuaProxy::Audio::MusicPlayFadeIn(int ms)
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_playMusicFadeIn(ms);
#endif
}


void LuaProxy::Audio::MusicStop()
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_stopMusic();
#endif
}


void LuaProxy::Audio::MusicStopFadeOut(int ms)
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_stopMusicFadeOut(ms);
#endif
}


void LuaProxy::Audio::MusicVolume(int vlm)
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_changeVolume(vlm);
#endif
}


void LuaProxy::Audio::MusicPause()
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_pauseMusic();
#endif
}


void LuaProxy::Audio::MusicResume()
{
#ifndef NO_SDL
    PGE_MusPlayer::MUS_playMusic();
#endif
}


bool LuaProxy::Audio::MusicIsPlaying()
{
#ifndef NO_SDL
    return PGE_MusPlayer::MUS_IsPlaying();
#else
    return false;
#endif
}


bool LuaProxy::Audio::MusicIsPaused()
{
#ifndef NO_SDL
    return PGE_MusPlayer::MUS_IsPaused();
#endif
}


bool LuaProxy::Audio::MusicIsFading()
{
#ifndef NO_SDL
	return PGE_MusPlayer::MUS_IsFading();
#endif
}




void LuaProxy::Audio::seizeStream(int section)
{
    if(mciEngine)
        mciEngine->setSeized(section, true);

}

void LuaProxy::Audio::releaseStream(int section)
{
    if(mciEngine)
        mciEngine->setSeized(section, false);
}







void LuaProxy::Audio::clearSFXBuffer()
{
    PGE_Sounds::clearSoundBuffer();
}


void LuaProxy::Audio::playSFX(const char *filename)
{
#ifndef NO_SDL
    std::string full_paths = getSfxPath(std::string(filename));
    PGE_Sounds::SND_PlaySnd(full_paths.c_str());
#else
    ::LuaProxy::playSFX(filename);
#endif
}


Mix_Chunk *LuaProxy::Audio::SfxOpen(const char *filename)
{
    std::string full_paths = getSfxPath(std::string(filename));
    return PGE_Sounds::SND_OpenSnd(full_paths.c_str());
}


int LuaProxy::Audio::SfxPlayCh(int channel, Mix_Chunk *chunk, int loops)
{
    return Mix_PlayChannel(channel, chunk, loops);
}


int LuaProxy::Audio::SfxPlayChTimed(int channel, Mix_Chunk *chunk, int loops, int ticks)
{
    return Mix_PlayChannelTimed(channel, chunk, loops, ticks);
}


int LuaProxy::Audio::SfxFadeInCh(int channel, Mix_Chunk *chunk, int loops, int ms)
{
    return Mix_FadeInChannel(channel, chunk, loops, ms);
}


int LuaProxy::Audio::SfxFadeInChTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks)
{
    return Mix_FadeInChannelTimed(channel, chunk, loops, ms, ticks);
}


void LuaProxy::Audio::SfxPause(int channel)
{
    Mix_Pause(channel);
}


void LuaProxy::Audio::SfxResume(int channel)
{
    Mix_Resume(channel);
}


int LuaProxy::Audio::SfxStop(int channel)
{
    return Mix_HaltChannel(channel);
}


int LuaProxy::Audio::SfxExpire(int channel, int ticks)
{
    return Mix_ExpireChannel(channel, ticks);
}


int LuaProxy::Audio::SfxFadeOut(int channel, int ms)
{
    return Mix_FadeOutChannel(channel, ms);
}


int LuaProxy::Audio::SfxIsPlaying(int channel)
{
    return Mix_Playing(channel);
}

int LuaProxy::Audio::SfxIsPaused(int channel)
{
    return Mix_Paused(channel);
}



string LuaProxy::Audio::getSfxPath(string filename)
{
    std::string full_paths;
    if(!isAbsolutePath(filename))
    {
        std::wstring world_dir = wstring((wchar_t*)GM_FULLDIR);
        std::wstring full_path = world_dir.append(Level::GetName());
        full_path = removeExtension(full_path);
        full_path = full_path.append(L"\\"); // < path into level folder
        full_paths = wstr2str(full_path) + filename;
    }
    else
    {
        full_paths = filename;
    }
    return full_paths;
}

MciEmulator *LuaProxy::Audio::mciEngine=NULL;
void LuaProxy::Audio::setMciEngine(MciEmulator *_engine)
{
    mciEngine=_engine;
}


void LuaProxy::Audio::setMciSection(int section)
{
    if(mciEngine)
        mciEngine->setCurrentSection(section);
}


void LuaProxy::Audio::resetMciSections()
{
    if(mciEngine)
        mciEngine->resetSeizes();
}
