#pragma once

#include "Defines.h"
#include <string>

using namespace std;

struct SpriteComponent;
class CSprite;

typedef void (*pfnSprFunc)(CSprite*, SpriteComponent* obj);

// Class for wrapping a sprite component.
// Sprite manager will run it (calling func) until run_time == 0
struct SpriteComponent {

	SpriteComponent() : data1(0), data2(0), data3(0), data4(0), func(NULL), lookup_code(0),
						run_time(0), org_time(0), expired(0) {  }

	void Init(int set_time) {
		func = NULL; lookup_code = 0; data1 = 0; data2 = 0; data3 = 0; data4 = 0;
		run_time = set_time;
		org_time = set_time;
		expired = false;
	}

	void Tick();			// Tick down the timer of this component. Expires self when run_time reaches 1

	pfnSprFunc func;
	int lookup_code;		// Which "bucket" of custom components this component is a part of
	int run_time;			// Run time frame countdown. 0 = infinite lifetime, always run
	int org_time;			// Frame count to begin countdown at
	double data1;
	double data2;
	double data3;
	double data4;
	wstring data5;
	bool expired;			// If this component should be deleted at the next clean up cycle
};