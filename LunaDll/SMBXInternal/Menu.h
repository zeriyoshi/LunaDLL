#ifndef Menu_hhhhh
#define Menu_hhhhh

#include <vector>
#include "../Defines.h"
#include "BaseItemArray.h"

struct EpisodeListItem : SMBX_FullBaseItemArray<EpisodeListItem, -1, GM_EP_LIST_COUNT_CONSTPTR, GM_EP_LIST_PTR_CONSTPTR>
{
    VB6StrPtr episodeName;
    VB6StrPtr episodePath;
    VB6StrPtr episodeWorldFile;
    DWORD unknown_C;
    DWORD unknown_10;
    VB6StrPtr unknown_14;

    static inline std::vector<EpisodeListItem*> GetAll(){
        std::vector<EpisodeListItem*> allEpisodes;
        for (int i = 0; i < GM_EP_LIST_COUNT; ++i){
            allEpisodes.push_back(Get(i));
        }
        return allEpisodes;
    }
};

static_assert(sizeof(EpisodeListItem) == 0x18, "sizeof(SMBX_BGO) must be 0x24");


#endif