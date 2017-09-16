#pragma once

#include "../../Engine/Engine.h"

namespace Engine
{
	class CBaseEntity;
	class CPlayer;
}

using namespace Engine;

struct Sound_s
{
	ULONGLONG dwTime;
	Vector vOrigin;
};

class CSoundEsp
{
public:
//[swap_lines]
	vector<Sound_s> Sound;

	Color SoundColor;

	CSoundEsp();

	void Update();
	void AddSound( Vector vOrigin );
	void DrawSoundEsp();
//[/swap_lines]
};

class CEsp
{
public:

	CEsp();
//[swap_lines]
	CSoundEsp SoundEsp;

	Color CT_HP_ColorM;
	Color TT_HP_ColorM;

	Color CT_AR_ColorM;
	Color TT_AR_ColorM;

	IMaterial* visible_flat;
	IMaterial* visible_tex;
	IMaterial* hidden_flat;
	IMaterial* hidden_tex;

	float fExplodeC4Timer;
	float fC4Timer;

	Color GetPlayerColor( CPlayer* pPlayer );
	Color GetPlayerVisibleColor( CPlayer* pPlayer );

	bool CheckPlayerTeam( CPlayer* pPlayer );

	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void OnReset();
	void OnEvents( IGameEvent* pEvent );
	void OnDrawModelExecute( IMatRenderContext* ctx , const DrawModelState_t &state , const ModelRenderInfo_t &pInfo , matrix3x4_t *pCustomBoneToWorld = NULL );
	//[/swap_lines]
private:
//[swap_lines]
	void DrawPlayerEsp( CPlayer* pPlayer );
	void DrawPlayerSkeleton( CPlayer* pPlayer );
	void DrawPlayerBulletTrace( CPlayer* pPlayer );

	void DrawHitBoxLine( Vector* vHitBoxArray , Color color );
//[/swap_lines]
};