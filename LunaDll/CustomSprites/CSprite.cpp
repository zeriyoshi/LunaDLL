#include "CSprite.h"
#include "../Rendering/Rendering.h"
#include "../Globals.h"
#include "../Rendering/RenderOps/RenderOp.h"
#include "../Rendering/RenderOps/RenderRectOp.h"
#include "../CustomSprites/SpritesFuncs/SpriteFuncs.h"

// CTOR
CSprite::CSprite() {
	Init();
}

// INIT
void CSprite::Init() {
	m_ImgResCode = -1;
	m_CollisionCode = -1;
	m_StaticScreenPos = false;
	m_Visible = true;
	m_Birthed = false;
	m_Died = false;
	m_Invalidated = false;
	m_LimitedFrameLife = false;
	m_AnimationSet = false;
	m_AlwaysProcess = false;
	m_FramesLeft = 0;	
	m_DrawPriorityLevel = 1;
	m_OffscreenCount = 0;
	m_FrameCounter = 0;
	m_GfxXOffset = 0;
	m_GfxYOffset = 0;
	m_Xpos = 0;
	m_Ypos = 0;
	m_Ht = 0;
	m_Wd = 0;
	m_Xspd = 0;
	m_Yspd = 0;	
	m_AnimationPhase = 0;
	m_AnimationFrame = 0;
	m_AnimationTimer = 0;
	m_Hitbox.pParent = this;
}

// CLEAR EXPIRED COMPONENTS
void CSprite::ClearExpiredComponents() {
	std::list<SpriteComponent>::iterator iter = m_BehavComponents.begin();
	std::list<SpriteComponent>::iterator end  = m_BehavComponents.end();

	while(iter != m_BehavComponents.end()) {
		SpriteComponent comp = *iter;
		if((*iter).expired) {			
			iter = m_BehavComponents.erase(iter);
		} else {
			++iter;
		}
	}
}

#ifndef __MINGW32__
#pragma region Add functions
#endif

// ADD BIRTH -- Add a birth function to sprite
void CSprite::AddBirthComponent(SpriteComponent comp) {
	if(comp.func != NULL)
		m_BirthComponents.push_back(comp);
}

// ADD BEHAVIOR -- Add a behavior component to sprite
void CSprite::AddBehaviorComponent(SpriteComponent comp) {
	if(comp.func != NULL)
		m_BehavComponents.push_back(comp);
}

// ADD DRAW -- Add a draw component to sprite. Updates m_StaticScreenPos if static drawing detected.
void CSprite::AddDrawComponent(pfnSprDraw func) {
	if(func != NULL)
		m_DrawFuncs.push_back(func);
	if(func == SpriteFunc::StaticDraw) {
		m_StaticScreenPos = true;
	}	
}

// ADD DEATH -- Add a death function to sprite
void CSprite::AddDeathComponent(SpriteComponent comp) {
	if(comp.func != NULL)
		m_DeathComponents.push_back(comp);
}

#ifndef __MINGW32__
#pragma endregion
#endif

// SET IMAGE RESOURCE
void CSprite::SetImageResource(int _resource_code) {
	m_ImgResCode = _resource_code;
}
void CSprite::SetImage(const std::shared_ptr<BMPBox>& _img) {
    m_directImg = _img;
}

// MAKE LIMITED LIFETIME
void CSprite::MakeLimitedLifetime(int new_lifetime) {
	this->m_LimitedFrameLife = true;
	this->m_FramesLeft = new_lifetime;
}

// BIRTH -- Run the birth components
void CSprite::Birth() {	
	if(!m_Birthed) {
		for (std::list<SpriteComponent>::iterator iter = m_BirthComponents.begin(); iter != m_BirthComponents.end(); ++iter) {
			(*iter).func(this, &(*iter));
		}
	}
	m_Birthed = true;
}

// PROCESS -- Process 1 frame of lifetime. Call all of the running behavior components
void CSprite::Process() {
	ClearExpiredComponents();

	if(m_Birthed == false)
		Birth();

	m_FrameCounter++;

	for (std::list<SpriteComponent>::iterator iter = m_BehavComponents.begin(); iter != m_BehavComponents.end(); ++iter) {
		(*iter).func(this, &(*iter));
		(*iter).Tick();		
	}

	// Die?
	if(this->m_LimitedFrameLife == true) {
		this->m_FramesLeft--;
		if(m_FramesLeft <= 0 && !m_Died) {
			Die();
		}
	}
}

// DRAW -- Call all of the registered draw components
void CSprite::Draw() {
	for (std::list<pfnSprDraw>::iterator iter = m_DrawFuncs.begin(); iter != m_DrawFuncs.end(); ++iter) {
		(*iter)(this);
	}
	if(false) { //debug
		RenderRectOp op;
		op.x1 = m_Xpos; op.y1 = m_Ypos;
		op.x2 = m_Xpos + m_Wd; op.y2 = m_Xpos + m_Ht; 
		op.m_FramesLeft = 1;
		op.Draw(&gLunaRender);
	}
}

// DIE -- Run the death components
void CSprite::Die() {	
	for (std::list<SpriteComponent>::iterator iter = m_DeathComponents.begin(); iter != m_DeathComponents.end(); ++iter) {
		(*iter).func(this, &(*iter));
	}
	m_Died = true;
	m_Invalidated = true;
}

// SET CUSTOM VAR
void CSprite::SetCustomVar(std::wstring var_name, OPTYPE operation_to_do, double value) {
	if(var_name.length() > 0) {
		// Create var if doesn't exist
		if(m_CustomVars.find(var_name) == m_CustomVars.end()) {
			m_CustomVars[var_name] = 0;
		}

		double var_val = m_CustomVars[var_name];

		// Do the operation
		OPTYPE oper = operation_to_do;
		switch(oper) {
		case OP_Assign: 			
			m_CustomVars[var_name] = value;
			break;
		case OP_Add:			
			m_CustomVars[var_name] = var_val + value;
			break;
		case OP_Sub:
			m_CustomVars[var_name] = var_val - value;
			break;
		case OP_Mult:
			m_CustomVars[var_name] = var_val * value;
			break;
		case OP_Div:
			if(value == 0)
				break;
			m_CustomVars[var_name] = var_val / value;
			break;
		case OP_XOR:
			m_CustomVars[var_name] = (int)var_val ^ (int)value;
			break;
		default:
			break;
		}
	}		
}

// CUSTOM VAR EXISTS
bool CSprite::CustomVarExists(std::wstring var_name) {
	return m_CustomVars.find(var_name) == m_CustomVars.end() ? false : true;
}

// GET CUSTOM VAR
double CSprite::GetCustomVar(std::wstring var_name) {
	if(m_CustomVars.find(var_name) == m_CustomVars.end()) {
		return 0;
	}
	return m_CustomVars[var_name];
}
