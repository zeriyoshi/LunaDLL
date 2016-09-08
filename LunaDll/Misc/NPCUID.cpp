#include "NPCUID.h"
#include <vector>
typedef unsigned short WORD; //windef already have these
typedef unsigned long DWORD;

namespace NPCUID {
	int nextUID = 1;
	bool wrapped = false;
	const WORD UID_MAX = 5000;
	const WORD UID_OFFSET = 0xE6;
	const WORD NPC_INDEX_INVALID = 0xFFFF;
	const WORD GEN_FLAG_OFFSET = 0x08;
	WORD unusedUIDList[UID_MAX-1] = { 0 };
	WORD unusedNextUIDIndex = 0;
	WORD unusedUIDListLen = UID_MAX-1;
	
	void init() {
		for (int i = 0; i < unusedUIDListLen; i++) {
			unusedUIDList[i] = i+1;
		}
	}

	void initUnusedUIDList() {

	}

	WORD getNextUID() {
		return unusedUIDList[unusedNextUIDIndex];
	}

	bool isGenerator(WORD NPCIndex) {
		return (*(WORD*)((byte*)GM_NPCS_PTR_CONSTPTR+(NPCIndex+129)*sizeof(::NPCMOB) + 0x64) == 0xFFFF);
	}

	void setGenCheckFlag(WORD NPCIndex,bool isGen) {
		*(bool*)((byte*)GM_NPCS_PTR_CONSTPTR + (NPCIndex + 129)*sizeof(::NPCMOB) + GEN_FLAG_OFFSET) = isGen;
	}

	void writeNPCUID(WORD NPCIndex,WORD )

	void getAllIdentifiedNPCIndex(bool* out) {
		for (int i = 0; i < *GM_NPCS_COUNT_CONSTPTR; i++) {
			out[i] = (*(WORD*)((byte*)GM_NPCS_PTR_CONSTPTR + (i+129)*sizeof(::NPCMOB) + UID_OFFSET) > 0); //no function call
		}
	}

	WORD generateUID(WORD* currentUIDPointer) {
		WORD tmp = getNextUID();
		if (unusedNextUIDIndex>=unusedUIDListLen-1) {
			refreshUIDList();
		}
		else {
			unusedNextUIDIndex++;
		}
		return tmp;
	}

	void refreshUIDList() {
		bool usedFlag[UID_MAX + 1] = { false };
		usedFlag[0] = true; //set invalid zero as used
		WORD usedCount = 1; //zero
		for (int i = 0; i < *GM_NPCS_COUNT_CONSTPTR; i++) {
			WORD* currentUIDPtr = (WORD*)((byte*)GM_NPCS_PTR_CONSTPTR + (i + 129)*sizeof(::NPCMOB) + UID_OFFSET); //no function call
			if (!usedFlag[*currentUIDPtr]) {
				usedFlag[*currentUIDPtr] = true;
				usedCount++;
			}
		}
		WORD unusedCount = 0;
		for (int k = 0; k <= UID_MAX; k++) {
			if (!usedFlag[k]) {
				unusedUIDList[unusedCount] = k;
				unusedCount++;
			}
		}
		unusedUIDListLen = unusedCount;
		unusedNextUIDIndex = 0;
		wrapped = false;
	}
}