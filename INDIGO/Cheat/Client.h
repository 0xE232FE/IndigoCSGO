#pragma once

#define DELETE_MOD(name) delete name; name = nullptr

#include "../Gui/Gui.h"
#include "../Engine/Engine.h"
#include "../License/License.h"

#include "Aimbot/Aimbot.h"
#include "TriggerBot/Triggerbot.h"
#include "Esp/Esp.h"
#include "Radar/Radar.h"
#include "Knifebot/Knifebot.h"
#include "Misc/Misc.h"
#include "Skin/Skin.h"

class CGui;

class CAimbot;
class CTriggerbot;
class CEsp;
class CKnifebot;
class CRadar;
class CSkin;
class CMisc;

namespace Engine
{
	class CRender;
	class CPlayers;
}

using namespace Engine;

namespace Client
{
//[swap_lines]
	extern int iScreenWidth;
	extern int iScreenHeight;

	extern string BaseDir;
	extern string LogFile;
	extern string GuiFile;
	extern string IniFile;

	extern Vector2D		g_vCenterScreen;

	extern CPlayers*	g_pPlayers;
	extern CRender*		g_pRender;
	extern CGui*		g_pGui;

	extern CAimbot*		g_pAimbot;
	extern CTriggerbot* g_pTriggerbot;
	extern CEsp*		g_pEsp;
	extern CRadar*		g_pRadar;
	extern CKnifebot*	g_pKnifebot;
	extern CSkin*		g_pSkin;
	extern CMisc*		g_pMisc;

	extern bool			bC4Timer;
	extern int			iC4Timer;

	extern int			iWeaponID;
	extern int			iWeaponSelectSkinIndex;
	extern int			iWeaponSelectIndex;

	bool Initialize( IDirect3DDevice9* pDevice );
	void Shutdown();

	void OnRender();
	void OnLostDevice();
	void OnResetDevice();
	void OnCreateMove( CUserCmd* pCmd );
	void OnFireEventClientSideThink( IGameEvent* pEvent );
	void OnFrameStageNotify( ClientFrameStage_t Stage );
	void OnDrawModelExecute( IMatRenderContext* ctx , const DrawModelState_t &state , const ModelRenderInfo_t &pInfo , matrix3x4_t *pCustomBoneToWorld = NULL );
	void OnPlaySound( const Vector* pOrigin , const char* pszSoundName );
	void OnPlaySound( const char* pszSoundName );
	void OnOverrideView( CViewSetup* pSetup );
	void OnGetViewModelFOV( float& fov );
	void OnRenderGUI();
//[/swap_lines]
}