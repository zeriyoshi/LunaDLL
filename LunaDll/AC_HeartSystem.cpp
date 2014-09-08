// Child for autocode.cpp
#include "Autocode.h"
#include "Globals.h"
#include "PlayerMOB.h"

void Autocode::HeartSystem() {

	PlayerMOB* sheath = Player::Get(1);
	if(sheath != 0) {

		// Don't run for demo or iris
		if(!Player::UsesHearts(sheath))
			return;

		// Detect extra heart pickup and hold displayed hearts at 2
		if(sheath->Hearts > 2) {
			gAutoMan.m_Hearts++;
			sheath->Hearts = 2;
			sheath->CurrentPowerup = (sheath->CurrentPowerup > 2 ? sheath->CurrentPowerup : 2);
		}
		if(sheath->Hearts == 2 && gAutoMan.m_Hearts < 2)
			gAutoMan.m_Hearts = 2;

		// Limit tracked max hearts
		if(gAutoMan.m_Hearts > Param2)
			gAutoMan.m_Hearts = (int)Param2;

		// If damaged, take hearts from extra hearts
		if(sheath->Hearts == 1 && gAutoMan.m_Hearts > 2) {
			char* dbg = "HEART SET";
			sheath->Hearts = 2;
			sheath->CurrentPowerup = (sheath->CurrentPowerup > 2 ? sheath->CurrentPowerup : 2);
			gAutoMan.m_Hearts--;
		}
		else if(sheath->Hearts == 1) {
			sheath->CurrentPowerup = 1;
			gAutoMan.m_Hearts = 1;
		}
		else if(sheath->Hearts == 0) {
			gAutoMan.m_Hearts = 0;
		}

		// Display life stuff on screen
		gLunaRender.SafePrint(std::wstring(L"HP: " + std::to_wstring((long long)gAutoMan.m_Hearts)),3, (float)Target, (float)Param1);

	}//if heartuser

}