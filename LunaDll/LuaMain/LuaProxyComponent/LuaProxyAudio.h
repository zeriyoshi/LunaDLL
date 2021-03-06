//#ifdef LuaProxyAudio_H //Temporary disabled, WIP
#ifndef LuaProxyAudio_H
#define LuaProxyAudio_H

#include <string>
struct Mix_Chunk;
class MciEmulator;

namespace LuaProxy
{
    namespace Audio
    {
        //Music
        void MusicOpen(const std::string& filename);
        void MusicPlay();
        void MusicPlayFadeIn(int ms);
        void MusicStop();
        void MusicStopFadeOut(int ms);
        void MusicVolume(int vlm);
        void MusicPause();
        void MusicResume();
        bool MusicIsPlaying();
        bool MusicIsPaused();
        bool MusicIsFading();
        std::string MusicTitle();
        std::string MusicTitleTag();
        std::string MusicArtistTag();
        std::string MusicAlbumTag();
        std::string MusicCopyrightTag();
        void seizeStream(int section);
        void releaseStream(int section);
        
        //SFX
        Mix_Chunk* newMix_Chunk();
        void clearSFXBuffer();
        void playSFX(const std::string& filename);
        Mix_Chunk* SfxOpen(const std::string& filename);
        int SfxPlayCh(int channel, Mix_Chunk* chunk, int loops);
        int SfxPlayChTimed(int channel, Mix_Chunk *chunk, int loops, int ticks);
        int SfxFadeInCh(int channel, Mix_Chunk *chunk, int loops, int ms);
        int SfxFadeInChTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks);
        void SfxPause(int channel);
        void SfxResume(int channel);
        int SfxStop(int channel);
        int SfxExpire(int channel, int ticks);
        int SfxFadeOut(int channel, int ms);
        int SfxIsPlaying(int channel);
        int SfxIsPaused(int channel);
        int SfxIsFading(int channel);
        int SfxVolume(int channel, int vlm);
        int SfxSetPanning(int channel, int left, int right);
        int SfxSetDistance(int channel, int distance);
        int SfxSet3DPosition(int channel, int angle, int distance);
        int SfxReverseStereo(int channel, int flip);

        std::string getSfxPath(const std::string& path);
        void setMciSection(int section);
        void resetMciSections();

        double AudioClock();
        double MusicClock();

        void __setOverrideForAlias(const std::string& alias, Mix_Chunk* chunk);
        Mix_Chunk* __getChunkForAlias(const std::string& alias);
        void __setMuteForAlias(const std::string& alias, bool muted);
        bool __getMuteForAlias(const std::string& alias);
    }
}
#endif
