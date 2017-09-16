#include "Skin.h"

unordered_map<int , EconomyItemCfg> g_SkinChangerCfg;
unordered_map<int , const char*> g_ViewModelCfg;
unordered_map<const char* , const char*> g_KillIconCfg;

char* pWeaponData[33] =
{
	// ��������� - 0 - 9
	WEAPON_DEAGLE_STR,WEAPON_ELITE_STR,WEAPON_FIVESEVEN_STR,
	WEAPON_GLOCK_STR,WEAPON_HKP2000_STR,WEAPON_P250_STR,
	WEAPON_USP_S_STR,WEAPON_CZ75A_STR,WEAPON_REVOLVER_STR,
	WEAPON_TEC9_STR,
	// �������� - 10 - 30
	WEAPON_AK47_STR,WEAPON_AUG_STR,WEAPON_FAMAS_STR,WEAPON_GALILAR_STR,
	WEAPON_M249_STR,WEAPON_M4A1_STR,WEAPON_M4A1_S_STR,WEAPON_MAC10_STR,
	WEAPON_P90_STR,WEAPON_UMP45_STR,WEAPON_XM1014_STR,WEAPON_BIZON_STR,
	WEAPON_MAG7_STR,WEAPON_NEGEV_STR,WEAPON_SAWEDOFF_STR,
	WEAPON_MP7_STR,WEAPON_MP9_STR,WEAPON_NOVA_STR,WEAPON_SG553_STR,
	WEAPON_SCAR20_STR,WEAPON_G3SG1_STR,
	// ���������  - 31 - 32
	WEAPON_AWP_STR,WEAPON_SSG08_STR
};

int pWeaponItemIndexData[33] =
{
	// ��������� - 0 - 9
	WEAPON_DEAGLE,WEAPON_ELITE,WEAPON_FIVESEVEN,
	WEAPON_GLOCK,WEAPON_HKP2000,WEAPON_P250,
	WEAPON_USP_SILENCER,WEAPON_CZ75A,WEAPON_REVOLVER,
	WEAPON_TEC9,
	// �������� - 10 - 30
	WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,
	WEAPON_M249,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_MAC10,
	WEAPON_P90,WEAPON_UMP45,WEAPON_XM1014,WEAPON_BIZON,
	WEAPON_MAG7,WEAPON_NEGEV,WEAPON_SAWEDOFF,
	WEAPON_MP7,WEAPON_MP9,WEAPON_NOVA,WEAPON_SG553,
	WEAPON_SCAR20,WEAPON_G3SG1,
	// ���������  - 31 - 32
	WEAPON_AWP,WEAPON_SSG08
};

char* pKnifeData[10] =
{
	"m9_bayonet","knife_flip","knife_gut","knife_karambit" ,"knife_m9_bayonet",
	"knife_tactical","knife_falchion","knife_survival_bowie","knife_butterfly","knife_push"
};

//[enc_string_enable /]
const char* bloodhound = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
const char* handwrap = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
const char* slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
const char* sporty = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
const char* motorcycle = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
const char* specialist = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
//[enc_string_disable /]

WeaponSkins_s WeaponSkins[33];
KnifeSkins_s KnifeSkins[10];

GlovesSkins_s GlovesSkin_Array[25] =
{
	{ 5027,10006,bloodhound },
	{ 5027,10007,bloodhound },
	{ 5027,10008,bloodhound },
	{ 5032,10009,handwrap },
	{ 5032,10010,handwrap },
	{ 5031,10013,slick },
	{ 5031,10015,slick },
	{ 5031,10016,slick },
	{ 5030,10018,sporty },
	{ 5030,10019,sporty },
	{ 5032,10021,handwrap },
	{ 5033,10024,motorcycle },
	{ 5033,10026,motorcycle },
	{ 5033,10027,motorcycle },
	{ 5033,10028,motorcycle },
	{ 5034,10030,specialist },
	{ 5034,10033,specialist },
	{ 5034,10034,specialist },
	{ 5034,10035,specialist },
	{ 5032,10036,handwrap },
	{ 5030,10037,sporty },
	{ 5030,10038,sporty },
	{ 5027,10039,bloodhound },
	{ 5031,10040,slick },
};
//[junk_enable /]
//[enc_string_enable /]
RecvVarProxyFn fnSequenceProxyFn = NULL;

using namespace Client;

int GetWeaponSkinIndexFromPaintKit( int iPaintKit )
{
	for ( size_t iSkinID = 0; iSkinID < WeaponSkins[iWeaponID].SkinPaintKit.size(); iSkinID++ )
	{
		if ( WeaponSkins[iWeaponID].SkinPaintKit[iSkinID] == iPaintKit )
			return iSkinID;
	}

	return 0;
}

int GetKnifeSkinIndexFromPaintKit( int iPaintKit , bool tt )
{
	int iKnifeModelID = ( tt ? Settings::Skin::knf_tt_model - 1 : Settings::Skin::knf_ct_model - 1 );

	for ( size_t iSkinID = 0; iSkinID < KnifeSkins[iKnifeModelID].SkinPaintKit.size(); iSkinID++ )
	{
		if ( KnifeSkins[iKnifeModelID].SkinPaintKit[iSkinID] == iPaintKit )
			return iSkinID;
	}

	return 0;
}
void CSkin::OnEvents( IGameEvent* pEvent )
{
	const char* szEventName = pEvent->GetName();

	if ( !strcmp( szEventName , "player_death" ) )
		ApplyCustomKillIcon( pEvent );

	if ( !strcmp( szEventName , "game_newmap" ) )
	{
		if ( g_ViewModelCfg.size() >= 1 )
			g_ViewModelCfg.clear();

		SetSkinConfig();
		SetModelConfig();
		SetKillIconCfg();
	}
}

void CSkin::SetSkinConfig()
{
	int KnifeModelsType[10] =
	{
		WEAPON_KNIFE_BAYONET,WEAPON_KNIFE_FLIP,WEAPON_KNIFE_GUT,WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,WEAPON_KNIFE_TACTICAL,WEAPON_KNIFE_FALCHION,
		WEAPON_KNIFE_SURVIVAL_BOWIE,WEAPON_KNIFE_BUTTERFLY,WEAPON_KNIFE_PUSH
	};

	if ( Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10 )
	{
		g_SkinChangerCfg[WEAPON_KNIFE].iItemDefinitionIndex = KnifeModelsType[Settings::Skin::knf_ct_model - 1];
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE].iItemDefinitionIndex = WEAPON_KNIFE;
	}
	
	if ( Settings::Skin::knf_ct_skin)
	{
		g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = Settings::Skin::knf_ct_skin;
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = 0;
	}
	
	if ( Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10 )
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].iItemDefinitionIndex = KnifeModelsType[Settings::Skin::knf_tt_model - 1];
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].iItemDefinitionIndex = WEAPON_KNIFE_T;
	}
	
	if ( Settings::Skin::knf_tt_skin)
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = Settings::Skin::knf_tt_skin;
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = 0;
	}
}

void CSkin::SetModelConfig()
{
	char* pszDefaultCtModel = "models/weapons/v_knife_default_ct.mdl";
	char* pszDefaultTtModel = "models/weapons/v_knife_default_t.mdl";

	char* pszKnifeBayonet = "models/weapons/v_knife_bayonet.mdl";
	char* pszKnifeFlip = "models/weapons/v_knife_flip.mdl";
	char* pszKnifeGut = "models/weapons/v_knife_gut.mdl";
	char* pszKnifeKarambit = "models/weapons/v_knife_karam.mdl";
	char* pszKnifeM9Bay = "models/weapons/v_knife_m9_bay.mdl";
	char* pszKnifeHuntsman = "models/weapons/v_knife_tactical.mdl";
	char* pszKnifeFalchion = "models/weapons/v_knife_falchion_advanced.mdl";
	char* pszKnifeBowie = "models/weapons/v_knife_survival_bowie.mdl";
	char* pszKnifeButterfly = "models/weapons/v_knife_butterfly.mdl";
	char* pszKnifeShadow = "models/weapons/v_knife_push.mdl";

	char* pszKnifeModels[10] =
	{
		pszKnifeBayonet,pszKnifeFlip,pszKnifeGut,pszKnifeKarambit,
		pszKnifeM9Bay,pszKnifeHuntsman,pszKnifeFalchion,pszKnifeBowie,
		pszKnifeButterfly,pszKnifeShadow
	};

	int nOriginalKnifeCT = Interfaces::ModelInfo()->GetModelIndex( pszDefaultCtModel );
	int nOriginalKnifeT = Interfaces::ModelInfo()->GetModelIndex( pszDefaultTtModel );

	if ( Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10 )
	{
		char* mdl_ct = pszKnifeModels[Settings::Skin::knf_ct_model - 1];
		g_ViewModelCfg[nOriginalKnifeCT] = mdl_ct;
	}
	else
	{
		g_ViewModelCfg[nOriginalKnifeCT] = pszDefaultCtModel;
	}

	if ( Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10 )
	{
		char* mdl_tt = pszKnifeModels[Settings::Skin::knf_tt_model - 1];
		g_ViewModelCfg[nOriginalKnifeT] = mdl_tt;
	}
	else
	{
		g_ViewModelCfg[nOriginalKnifeT] = pszDefaultTtModel;
	}
}

void CSkin::SetKillIconCfg()
{
//[enc_string_disable /]
	char* pszKnifeModelsIcon[10] =
	{
		"bayonet","knife_flip","knife_gut","knife_karambit",
		"knife_m9_bayonet","knife_tactical","knife_falchion",
		"knife_survival_bowie","knife_butterfly","knife_push"
	};
//[enc_string_enable /]
	if ( Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10 )
	{
		g_KillIconCfg["knife_default_ct"] = pszKnifeModelsIcon[Settings::Skin::knf_ct_model - 1];
	}
	else
	{
		g_KillIconCfg["knife_default_ct"] = "knife_default_ct";
	}
	if ( Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10 )
	{
		g_KillIconCfg["knife_t"] = pszKnifeModelsIcon[Settings::Skin::knf_tt_model - 1];
	}
	else
	{
		g_KillIconCfg["knife_t"] = "knife_t";
	}
}

void CSkin::InitalizeSkins()
{
	auto pSkins = new DynSkin::Skins::CSkins();

	string CsgoPatch = CSX::Utils::GetModuleBaseDir( 0 ) + "\\csgo";

	if ( pSkins->Load( CsgoPatch , "csgo" ) )
	{
		// Load Weapon Skins
		for ( int iWeaponIndex = 0; iWeaponIndex < WEAPON_DATA_SIZE; iWeaponIndex++ )
		{
			string WeaponSearch = "weapon_" + string( pWeaponData[iWeaponIndex] );
			
			DynSkin::Skins::vecSkinInfo SkinInfoWeapon = pSkins->GetSkinInfoByWeapon( WeaponSearch );

			for ( size_t iSkinWeaponIndex = 0; iSkinWeaponIndex < SkinInfoWeapon.size(); iSkinWeaponIndex++ )
			{
				if ( SkinInfoWeapon[iSkinWeaponIndex].nFallbackPaintKit >= 10000 )
					continue;

				string DragonKing = "Dragon King";
				string FullName = SkinInfoWeapon[iSkinWeaponIndex]._name;

				if ( SkinInfoWeapon[iSkinWeaponIndex].nFallbackPaintKit == 400 && SkinInfoWeapon[iSkinWeaponIndex]._name.find( DragonKing ) != string::npos )
					FullName = DragonKing;

				FullName += " (" + SkinInfoWeapon[iSkinWeaponIndex]._shortname + ") " + to_string( SkinInfoWeapon[iSkinWeaponIndex].nFallbackPaintKit );

				WeaponSkins[iWeaponIndex].SkinPaintKit.push_back( SkinInfoWeapon[iSkinWeaponIndex].nFallbackPaintKit );
				WeaponSkins[iWeaponIndex].SkinNames.push_back( FullName );
			}
		}

		// Load Knife Skins
		for ( int iKnifeIndex = 0; iKnifeIndex < KNIFE_DATA_SIZE; iKnifeIndex++ )
		{
			string KnifeSearch = "weapon_" + string( pKnifeData[iKnifeIndex] );

			DynSkin::Skins::vecSkinInfo SkinInfoKnife = pSkins->GetSkinInfoByWeapon( KnifeSearch );

			for ( size_t iSkinKnifeIndex = 0; iSkinKnifeIndex < SkinInfoKnife.size(); iSkinKnifeIndex++ )
			{
				KnifeSkins[iKnifeIndex].SkinPaintKit.push_back( SkinInfoKnife[iSkinKnifeIndex].nFallbackPaintKit );
				KnifeSkins[iKnifeIndex].SkinNames.push_back( SkinInfoKnife[iSkinKnifeIndex]._name + " (" + SkinInfoKnife[iSkinKnifeIndex]._shortname + ") " + to_string( SkinInfoKnife[iSkinKnifeIndex].nFallbackPaintKit ) );
			}
		}

		pSkins->Release();
	}

	delete pSkins;

	SetSkinConfig();
	SetKillIconCfg();

	for ( ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext )
	{
		if ( !strcmp( pClass->m_pNetworkName , "CBaseViewModel" ) )
		{
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for ( int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++ )
			{
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if ( !pProp || strcmp( pProp->m_pVarName , "m_nSequence" ) )
					continue;

				fnSequenceProxyFn = pProp->m_ProxyFn;
				pProp->m_ProxyFn = (RecvVarProxyFn)Hook_SetViewModelSequence;

				break;
			}

			break;
		}
	}
}

bool CSkin::ApplyCustomSkin( CBaseAttributableItem* pWeapon , int nWeaponIndex )
{
	if ( g_SkinChangerCfg.find( nWeaponIndex ) == g_SkinChangerCfg.end() )
		return false;

	*pWeapon->GetFallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
	*pWeapon->GetEntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
	*pWeapon->GetFallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
	*pWeapon->GetFallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;

	if ( g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex )
	{
		*pWeapon->GetItemDefinitionIndex() = g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex;
	}

	*pWeapon->GetItemIDHigh() = -1;

	return true;
}

bool CSkin::ApplyCustomModel( CBaseEntity* pLocal , CBaseAttributableItem* pWeapon )
{
	CBaseViewModel* pViewModel = pLocal->GetViewModel();

	if ( !pViewModel )
		return false;

	DWORD hViewModelWeapon = pViewModel->GetWeapon();

	CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle( (PVOID)hViewModelWeapon );

	if ( pViewModelWeapon != pWeapon )
		return false;

	int nViewModelIndex = pViewModel->GetModelIndex();

	if ( !nViewModelIndex )
		return false;

	if ( g_ViewModelCfg.find( nViewModelIndex ) == g_ViewModelCfg.end() )
		return false;

	pViewModel->SetModelIndex( Interfaces::ModelInfo()->GetModelIndex( g_ViewModelCfg[nViewModelIndex] ) );

	return true;
}

bool CSkin::ApplyCustomKillIcon( IGameEvent* pEvent )
{
	int nUserID = pEvent->GetInt( "attacker" );

	if ( !nUserID )
		return false;

	if ( Interfaces::Engine()->GetPlayerForUserID( nUserID ) != Interfaces::Engine()->GetLocalPlayer() )
		return false;

	const char* szWeapon = pEvent->GetString( "weapon" );

	for ( auto ReplacementIcon : g_KillIconCfg )
	{
		if ( !strcmp( szWeapon , ReplacementIcon.first ) )
		{
			pEvent->SetString( "weapon" , ReplacementIcon.second );
			break;
		}
	}

	return true;
}

void Skin_OnFrameStageNotify( ClientFrameStage_t Stage )
{
	if ( Stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START )
	{
		int nLocalPlayerID = Interfaces::Engine()->GetLocalPlayer();

		CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( nLocalPlayerID );

		if ( !pLocal || pLocal->IsDead() )
			return;

		UINT* hWeapons = pLocal->GetWeapons();

		if ( !hWeapons )
			return;

		PlayerInfo LocalPlayerInfo;
		Interfaces::Engine()->GetPlayerInfo( nLocalPlayerID , &LocalPlayerInfo );

		for ( int nIndex = 0; hWeapons[nIndex]; nIndex++ )
		{
			CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle(
				(PVOID)hWeapons[nIndex] );

			CBaseViewModel* pView = pWeapon->GetViewModel();

			if ( !pWeapon )
				continue;

			int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();

			if ( g_ViewModelCfg.find( pView->GetModelIndex() ) != g_ViewModelCfg.end() )
				pView->SetModelIndex( Interfaces::ModelInfo()->GetModelIndex( g_ViewModelCfg[pView->GetModelIndex()] ) );

			if ( Client::g_pSkin )
				Client::g_pSkin->ApplyCustomModel( pLocal , pWeapon );

			if ( LocalPlayerInfo.m_nXuidLow != *pWeapon->GetOriginalOwnerXuidLow() )
				continue;

			if ( LocalPlayerInfo.m_nXuidHigh != *pWeapon->GetOriginalOwnerXuidHigh() )
				continue;

			if ( Client::g_pSkin )
				Client::g_pSkin->ApplyCustomSkin( pWeapon , nWeaponIndex );

			*pWeapon->GetAccountID() = LocalPlayerInfo.m_nXuidLow;
		}
	}
}

void Gloves_OnFrameStageNotify( ClientFrameStage_t Stage )
{
	if ( !Settings::Skin::gloves_skin )
		return;

	if ( Stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	if ( !IsLocalAlive() )
		return;

	CBaseEntity* pLocal = g_pPlayers->GetLocal()->m_pEntity;

	UINT* pWeapons = pLocal->GetWeapons();

	if ( !pWeapons )
		return;

	UINT* pWareables = pLocal->GetWearables();

	if ( !Interfaces::EntityList()->GetClientEntityFromHandle( (PVOID)pWareables[0] ) )
	{
		for ( ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext )
		{
			if ( pClass->m_ClassID == (int)CLIENT_CLASS_ID::CEconWearable )
			{
				int iEntry = Interfaces::EntityList()->GetHighestEntityIndex() + 1;
				int iSerial = CSX::Utils::RandomIntRange( 0x0 , 0xFFF );

				if ( pClass->m_pCreateFn( iEntry , iSerial ) )
				{
					pWareables[0] = iEntry | ( iSerial << 16 );
					break;
				}
			}
		}

		if ( !pWareables[0] )
			return;

		CBaseAttributableItem* pGlove = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle( (PVOID)pWareables[0] );

		if ( !pGlove )
			return;

		PlayerInfo LocalPlayerInfo;

		if ( !Interfaces::Engine()->GetPlayerInfo( Interfaces::Engine()->GetLocalPlayer() , &LocalPlayerInfo ) )
			return;

		*pGlove->GetItemDefinitionIndex() = GlovesSkin_Array[Settings::Skin::gloves_skin - 1].ItemIndex;
		*pGlove->GetItemIDHigh() = -1;
		*pGlove->GetEntityQuality() = 4;
		*pGlove->GetAccountID() = LocalPlayerInfo.m_nXuidLow;
		*pGlove->GetFallbackWear() = 0.00000001f;
		*pGlove->GetFallbackSeed() = 0;
		*pGlove->GetFallbackStatTrak() = -1;
		*pGlove->GetFallbackPaintKit() = GlovesSkin_Array[Settings::Skin::gloves_skin - 1].PaintKit;

		int ModelIndex = Interfaces::ModelInfo()->GetModelIndex(
			GlovesSkin_Array[Settings::Skin::gloves_skin - 1].szModel );

		if ( !ModelIndex )
			return;

		if ( pGlove->GetViewModel() )
			pGlove->GetViewModel()->SetModelIndex( ModelIndex );

		if ( pGlove->GetClientNetworkable() )
			pGlove->GetClientNetworkable()->PreDataUpdate( DATA_UPDATE_CREATED );
	}
}

void Hook_SetViewModelSequence( const CRecvProxyData *pDataConst , void *pStruct , void *pOut )
{
	CRecvProxyData* pData = const_cast<CRecvProxyData*>( pDataConst );
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;

	if ( pViewModel )
	{
		IClientEntity* pOwner = Interfaces::EntityList()->GetClientEntityFromHandle( (PVOID)pViewModel->GetOwner() );

		if ( pOwner && pOwner->EntIndex() == Interfaces::Engine()->GetLocalPlayer() )
		{
			const model_t* pModel = Interfaces::ModelInfo()->GetModel( pViewModel->GetModelIndex() );
			const char* szModel = Interfaces::ModelInfo()->GetModelName( pModel );

			int m_nSequence = pData->m_Value.m_Int;
//[junk_disable /]
			if ( !strcmp( szModel , "models/weapons/v_knife_butterfly.mdl" ) )
			{
				switch ( m_nSequence )
				{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_BUTTERFLY_DRAW , SEQUENCE_BUTTERFLY_DRAW2 ); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_BUTTERFLY_LOOKAT01 , SEQUENCE_BUTTERFLY_LOOKAT03 ); break;
					default:
						m_nSequence++;
				}
			}
			else if ( !strcmp( szModel , "models/weapons/v_knife_falchion_advanced.mdl" ) )
			{
				// Fix animations for the Falchion Knife.
				switch ( m_nSequence )
				{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_FALCHION_HEAVY_MISS1 , SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP ); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_FALCHION_LOOKAT01 , SEQUENCE_FALCHION_LOOKAT02 ); break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						m_nSequence--;
				}
			}
			else if ( !strcmp( szModel , "models/weapons/v_knife_push.mdl" ) )
			{
				// Fix animations for the Shadow Daggers.
				switch ( m_nSequence )
				{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
					case SEQUENCE_DEFAULT_LIGHT_MISS1:
					case SEQUENCE_DEFAULT_LIGHT_MISS2:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_DAGGERS_LIGHT_MISS1 , SEQUENCE_DAGGERS_LIGHT_MISS5 ); break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = CSX::Utils::RandomIntRange( SEQUENCE_DAGGERS_HEAVY_MISS2 , SEQUENCE_DAGGERS_HEAVY_MISS1 ); break;
					case SEQUENCE_DEFAULT_HEAVY_HIT1:
					case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence += 3; break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						m_nSequence += 2;
				}
			}
			else if ( !strcmp( szModel , "models/weapons/v_knife_survival_bowie.mdl" ) )
			{
				switch ( m_nSequence )
				{
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
					default:
						m_nSequence--;
				}
			}
//[junk_enable /]
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	fnSequenceProxyFn( pData , pStruct , pOut );
}