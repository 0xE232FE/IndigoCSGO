#include "Engine.h"

#pragma warning(disable:4244)

//[enc_string_enable /]
//[junk_enable /]

namespace Engine
{
	bool Initialize()
	{
		CLicense License;
		
		/*#if ENABLE_LICENSING == 1
		if ( !License.CheckLicense() )
		{
			return false;
		}
		#endif*/
		
		if ( !CSX::Utils::IsModuleLoad( CLIENT_DLL , 45000 ) )
			return false;

		if ( !CSX::Utils::IsModuleLoad( ENGINE_DLL ) )
			return false;

		if ( !CSX::Utils::IsModuleLoad( VGUI2_DLL ) )
			return false;

		if ( !CSX::Utils::IsModuleLoad( VGUIMAT_DLL ) )
			return false;

		if ( !CSX::Utils::IsModuleLoad( VSTDLIB_DLL ) )
			return false;

		if ( !SDK::Interfaces::Engine() )
		{
			return false;
		}

		if ( !SDK::Interfaces::Client() )
		{
			return false;
		}

		if ( !SDK::Interfaces::EntityList() )
		{
			return false;
		}

		if ( !SDK::Interfaces::GlobalVars() )
		{
			return false;
		}

		/*
		if ( !SDK::Interfaces::Input() )
		{
			return false;
		}
		*/

		if ( !SDK::Interfaces::EngineTrace() )
		{
			return false;
		}

		if ( !SDK::Interfaces::ClientMode() )
		{
			return false;
		}

		if ( !SDK::Interfaces::ModelInfo() )
		{
			return false;
		}

		if ( !SDK::Interfaces::Sound() )
		{
			return false;
		}

		if ( !SDK::Interfaces::ModelRender() )
		{
			return false;
		}

		if ( !SDK::Interfaces::RenderView() )
		{
			return false;
		}

		if ( !SDK::Interfaces::MaterialSystem() )
		{
			return false;
		}

		if ( !SDK::Interfaces::Surface() )
		{
			return false;
		}

		if ( !SDK::Interfaces::GameEvent() )
		{
			return false;
		}

		if ( !SDK::Interfaces::InputSystem() )
		{
			return false;
		}

		if ( !g_NetVar.Init( SDK::Interfaces::Client()->GetAllClasses() ) )
			return false;

		if ( !Engine::Hook::Initialize() || !Engine::Offset::Initialize() )
		{
			return false;
		}

		return true;
	}

	void Shutdown()
	{
		Hook::Shutdown();
		Client::Shutdown();
	}
//[junk_disable /]
	WEAPON_TYPE GetWeaponType( int iItemDefinitionIndex )
	{
		switch ( iItemDefinitionIndex )
		{
			case WEAPON_DEAGLE:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_ELITE:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_FIVESEVEN:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_GLOCK:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_AK47:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_AUG:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_AWP:
				return WEAPON_TYPE_SNIPER;
			case WEAPON_FAMAS:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_G3SG1:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_GALILAR:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_M249:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_M4A1:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_MAC10:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_P90:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_UMP45:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_XM1014:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_BIZON:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_MAG7:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_NEGEV:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_SAWEDOFF:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_TEC9:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_TASER:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_HKP2000:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_MP7:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_MP9:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_NOVA:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_P250:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_SCAR20:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_SG553:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_SSG08:
				return WEAPON_TYPE_SNIPER;
			case WEAPON_KNIFE:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_FLASHBANG:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_HEGRENADE:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_SMOKEGRENADE:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_MOLOTOV:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_DECOY:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_INCGRENADE:
				return WEAPON_TYPE_GRENADE;
			case WEAPON_C4:
				return WEAPON_TYPE_C4;
			case WEAPON_KNIFE_T:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_M4A1_SILENCER:
				return WEAPON_TYPE_SHOTGUN;
			case WEAPON_USP_SILENCER:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_CZ75A:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_REVOLVER:
				return WEAPON_TYPE_PISTOL;
			case WEAPON_KNIFE_BAYONET:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_FLIP:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_GUT:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_KARAMBIT:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_M9_BAYONET:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_TACTICAL:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_FALCHION:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_SURVIVAL_BOWIE:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_BUTTERFLY:
				return WEAPON_TYPE_KNIFE;
			case WEAPON_KNIFE_PUSH:
				return WEAPON_TYPE_KNIFE;
			default:
				return WEAPON_TYPE_UNKNOWN;
		}

		return WEAPON_TYPE_UNKNOWN;
	}
//[junk_enable /]
	bool IsLocalAlive()
	{
		if ( Client::g_pPlayers && 
			 Client::g_pPlayers->GetLocal() && 
			 Client::g_pPlayers->GetLocal()->bAlive &&
			 Client::g_pPlayers->GetLocal()->m_pEntity &&
			 Client::g_pPlayers->GetLocal()->m_pWeaponEntity )
			return true;

		return false;
	}

	void ForceFullUpdate()
	{
		if (Client::g_pSkin)
		{
			Client::g_pSkin->SetSkinConfig();
			Client::g_pSkin->SetModelConfig();
			Client::g_pSkin->SetKillIconCfg();
		}

		typedef void(*ForceUpdate) (void);
		ForceUpdate FullUpdate = (ForceUpdate)CSX::Memory::FindSignature("engine.dll", "FullUpdate", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
		FullUpdate();
	}

	int GetWeaponSettingsSelectID()
	{
		for ( size_t i = 0; i < WEAPON_DATA_SIZE; i++ )
		{
			if ( pWeaponItemIndexData[i] == Client::g_pPlayers->GetLocal()->WeaponIndex )
				return i;
		}

		return -1;
	}

	bool ScanColorFromCvar( const char* str , float* colors )
	{
		int len = strlen( str );
		if ( len>11 )return false;
		int clrnum = 0;
		int clrsum = 0;
		for ( int i = 0; i<len; i++ )
		{
			if ( clrnum >= 3 )return false;
			if ( str[i] >= '0'&&str[i] <= '9' )
			{
				clrsum = clrsum * 10 + str[i] - ( int )'0';
			}
			else
			{
				colors[clrnum++] = (float)clrsum / 255.f;
				clrsum = 0;
			}
		}
		colors[clrnum] = (float)clrsum / 255.f;
		return true;
	}

	bool WorldToScreen( const Vector& vOrigin , Vector& vScreen )
	{
		static float* ViewMatrixOld = nullptr;
		float* ViewMatrix = nullptr;

		if ( !ViewMatrixOld )
		{
			ViewMatrixOld = Offset::FindW2Matrix();
		}
		else
		{
			ViewMatrix = (float*)( *(PDWORD)( ViewMatrixOld ) + 0x3DC );
		}
		
		if ( ViewMatrix && *ViewMatrix )
		{
			vScreen.x = ViewMatrix[0] * vOrigin.x + ViewMatrix[1] * vOrigin.y + ViewMatrix[2] * vOrigin.z + ViewMatrix[3];
			vScreen.y = ViewMatrix[4] * vOrigin.x + ViewMatrix[5] * vOrigin.y + ViewMatrix[6] * vOrigin.z + ViewMatrix[7];
			float w = ViewMatrix[12] * vOrigin.x + ViewMatrix[13] * vOrigin.y + ViewMatrix[14] * vOrigin.z + ViewMatrix[15];

			if ( w < 0.01f )
				return false;

			float invw = 1.0f / w;
			vScreen.x *= invw;
			vScreen.y *= invw;

			float x = (float)Client::iScreenWidth / 2.f;
			float y = (float)Client::iScreenHeight / 2.f;

			x += 0.5f * vScreen.x * Client::iScreenWidth + 0.5f;
			y -= 0.5f * vScreen.y * Client::iScreenHeight + 0.5f;

			vScreen.x = x;
			vScreen.y = y;

			return true;
		}

		return false;
	}

	bool GetVisibleOrigin( const Vector& vOrigin )
	{
		if ( Client::g_pEsp && IsLocalAlive() )
		{
			trace_t tr;
			Ray_t ray;
			CTraceFilter Filter;
			Filter.pSkip = Client::g_pPlayers->GetLocal()->m_pEntity;

			ray.Init( Client::g_pPlayers->GetLocal()->m_pEntity->GetEyePosition() , vOrigin );

			Interfaces::EngineTrace()->TraceRay( ray , MASK_VISIBLE , &Filter , &tr );

			return tr.IsVisible();
		}

		return false;
	}

	void AngleVectors( const Vector &vAngles , Vector& vForward )
	{
		float	sp , sy , cp , cy;

		sy = sin( DEG2RAD( vAngles[1] ) );
		cy = cos( DEG2RAD( vAngles[1] ) );

		sp = sin( DEG2RAD( vAngles[0] ) );
		cp = cos( DEG2RAD( vAngles[0] ) );

		vForward.x = cp*cy;
		vForward.y = cp*sy;
		vForward.z = -sp;
	}

	void VectorAngles( const Vector vForward , Vector& vAngle )
	{
		float tmp , yaw , pitch;

		if ( vForward[1] == 0 && vForward[0] == 0 )
		{
			yaw = 0;
			if ( vForward[2] > 0 )
			{
				pitch = 270;
			}
			else
			{
				pitch = 90;
			}
		}
		else
		{
			yaw = ( atan2( vForward[1] , vForward[0] ) * 180 / M_PI );
			if ( yaw < 0 )
			{
				yaw += 360;
			}
			tmp = sqrt( vForward[0] * vForward[0] + vForward[1] * vForward[1] );
			pitch = ( atan2( -vForward[2] , tmp ) * 180 / M_PI );

			if ( pitch < 0 )
			{
				pitch += 360;
			}
		}

		vAngle[0] = pitch;
		vAngle[1] = yaw;
		vAngle[2] = 0;
	}

	void AngleNormalize( Vector& vAngles )
	{
		if ( vAngles.x > 89.0f && vAngles.x <= 180.0f )
			vAngles.x = 89.0f;
		while ( vAngles.x > 180.f )
			vAngles.x -= 360.f;
		while ( vAngles.x < -89.0f )
			vAngles.x = -89.0f;
		while ( vAngles.y > 180.f )
			vAngles.y -= 360.f;
		while ( vAngles.y < -180.f )
			vAngles.y += 360.f;
		vAngles.z = 0;
	}

	void SmoothAngles( Vector MyViewAngles , Vector AimAngles , Vector &OutAngles , float Smoothing )
	{
		OutAngles = AimAngles - MyViewAngles;

		AngleNormalize( OutAngles );

		OutAngles.x = OutAngles.x / Smoothing + MyViewAngles.x;
		OutAngles.y = OutAngles.y / Smoothing + MyViewAngles.y;

		AngleNormalize( OutAngles );
	}

	float DistanceScreen( Vector2D vDstPos , Vector2D vSrcPos )
	{
		return ( sqrt( pow( vSrcPos.x - vDstPos.x , 2 ) + pow( vSrcPos.y - vDstPos.y , 2 ) ) );
	}

	bool LineGoesThroughSmoke( Vector vStartPos , Vector vEndPos )
	{
		typedef bool( __cdecl* _LineGoesThroughSmoke ) ( Vector , Vector );

		static _LineGoesThroughSmoke LineGoesThroughSmokeFn = 0;
		static bool SearchFunction = false;

		if ( !SearchFunction )
		{
			DWORD dwFunctionAddress = CSX::Memory::FindPattern( CLIENT_DLL , SMOK_PATTERN , "xxxxxxxx????xxx" , 0 );

			if ( dwFunctionAddress )
			{
				LineGoesThroughSmokeFn = (_LineGoesThroughSmoke)dwFunctionAddress;
				SearchFunction = true;
			}
		}

		if ( LineGoesThroughSmokeFn && SearchFunction )
		{
			return LineGoesThroughSmokeFn( vStartPos , vEndPos );
		}

		return false;
	}

	bool CheckPlayerFov( Vector vScreen , int iFov )
	{
		int	iFovX = (int)Client::g_vCenterScreen.x - (int)vScreen.x;
		int	iFovY = (int)Client::g_vCenterScreen.y - (int)vScreen.y;

		if ( iFovX < iFov && iFovX > -iFov && iFovY < iFov && iFovY > -iFov )
			return true;

		return false;
	}

	bool CheckPlayerFov( Vector vScreen , int iFovX , int iFovY )
	{
		int	FovX = (int)Client::g_vCenterScreen.x - vScreen.x;
		int	FovY = (int)Client::g_vCenterScreen.y - vScreen.y;

		if ( FovX < iFovX && FovX > -iFovX && FovY < iFovX && FovY > -iFovX )
			return true;

		return false;
	}
//[enc_string_disable /]
	IMaterial* CreateMaterial( bool bFlat , bool bShouldIgnoreZ )
	{
		static int iCreated = 0;

		static const char szTmp[] =
		{
			"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"VGUI/white_additive\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"1\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"1\"\
		\n\t\"halflambert\" \"1\"\
		\n\t\"$wireframe\" \"%i\"\
		\n}\n"
		};

		char szMaterial[256];
		char szBaseType[18];

		if ( bFlat )
		{
			static const char pszBaseType[] = "UnlitGeneric";
			sprintf_s( szMaterial , sizeof( szMaterial ) , szTmp , pszBaseType , ( bShouldIgnoreZ ) ? 1 : 0 , 0 );
			strcpy_s( szBaseType , pszBaseType );
		}
		else
		{
			static const char pszBaseType[] = "VertexLitGeneric";
			sprintf_s( szMaterial , sizeof( szMaterial ) , szTmp , pszBaseType , ( bShouldIgnoreZ ) ? 1 : 0 , 0 );
			sprintf_s( szBaseType , pszBaseType );
		}

		char szName[25];
		sprintf_s( szName , sizeof( szName ) , "custom_material_%i.vmt" , iCreated );

		++iCreated;

		auto pKeyValues = new KeyValues( szBaseType );
		pKeyValues->LoadFromBuffer( pKeyValues , szName , szMaterial );
		auto pCreatedMaterial = Interfaces::MaterialSystem()->CreateMaterial( szName , pKeyValues );
		pCreatedMaterial->IncrementReferenceCount();
		return pCreatedMaterial;
	}
//[enc_string_enable /]
	void ForceMaterial( Color color , IMaterial* material , bool useColor , bool forceMaterial )
	{
		if ( useColor )
		{
			float blend[3] = { (float)color.r() / 255.f, (float)color.g() / 255.f, (float)color.b() / 255.f };
			float alpha = (float)color.a() / 255.f;

			Interfaces::RenderView()->SetBlend( alpha );
			Interfaces::RenderView()->SetColorModulation( blend );
		}

		if ( forceMaterial )
			Interfaces::ModelRender()->ForcedMaterialOverride( material );
	}
//[enc_string_disable /]
	BOOL SearchFiles( LPCTSTR lpszFileName , LPSEARCHFUNC lpSearchFunc , BOOL bInnerFolders = FALSE )
	{
		LPTSTR part;
		char tmp[MAX_PATH];
		char name[MAX_PATH];

		HANDLE hSearch = NULL;
		WIN32_FIND_DATA wfd;
		memset( &wfd , 0 , sizeof( WIN32_FIND_DATA ) );

		if ( bInnerFolders )
		{
			if ( GetFullPathName( lpszFileName , MAX_PATH , tmp , &part ) == 0 ) return FALSE;
			strcpy( name , part );
			strcpy( part , "*.*" );
			wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
			if ( !( ( hSearch = FindFirstFile( tmp , &wfd ) ) == INVALID_HANDLE_VALUE ) )
				do
				{
					if ( !strncmp( wfd.cFileName , "." , 1 ) || !strncmp( wfd.cFileName , ".." , 2 ) )
						continue;

					if ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						char next[MAX_PATH];
						if ( GetFullPathName( lpszFileName , MAX_PATH , next , &part ) == 0 ) return FALSE;
						strcpy( part , wfd.cFileName );
						strcat( next , "\\" );
						strcat( next , name );

						SearchFiles( next , lpSearchFunc , TRUE );
					}
				} while ( FindNextFile( hSearch , &wfd ) );
				FindClose( hSearch );
		}

		if ( ( hSearch = FindFirstFile( lpszFileName , &wfd ) ) == INVALID_HANDLE_VALUE )
			return TRUE; 
		do
			if ( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				char file[MAX_PATH];
				if ( GetFullPathName( lpszFileName , MAX_PATH , file , &part ) == 0 ) return FALSE;
				strcpy( part , wfd.cFileName );

				lpSearchFunc( wfd.cFileName );
			}
		while ( FindNextFile( hSearch , &wfd ) );
		FindClose( hSearch );
		return TRUE;
	}
}

bool CTimer::delay( DWORD dwMsec )
{
	if ( !bEnable )
		return true;

	if ( !dwTime )
		dwTime = GetTickCount64();

	if ( dwTime + dwMsec < GetTickCount64() )
	{
		//dwTime = 0;
		bEnable = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CTimer::reset()
{
	dwTime = 0;
	bEnable = true;
}

void CTimer::stop()
{
	bEnable = false;
}