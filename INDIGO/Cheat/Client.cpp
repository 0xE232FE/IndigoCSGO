#include "Client.h"

//[enc_string_enable /]
//[junk_enable /]

namespace Client
{
	//[swap_lines]
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";

	vector<string> ConfigList;

	Vector2D	g_vCenterScreen = Vector2D(0.f, 0.f);

	CPlayers*	g_pPlayers = nullptr;
	CRender*	g_pRender = nullptr;
	CGui*		g_pGui = nullptr;

	CAimbot*	g_pAimbot = nullptr;
	CTriggerbot* g_pTriggerbot = nullptr;
	CEsp*		g_pEsp = nullptr;
	CRadar*		g_pRadar = nullptr;
	CKnifebot*	g_pKnifebot = nullptr;
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;
	//[/swap_lines]

	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = BaseDir + "\\*.ini";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{
		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();

		g_pAimbot = new CAimbot();
		g_pTriggerbot = new CTriggerbot();
		g_pEsp = new CEsp();
		g_pRadar = new CRadar();
		g_pKnifebot = new CKnifebot();
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();

		GuiFile = BaseDir + "\\" + "gui.ini";
		IniFile = BaseDir + "\\" + "settings.ini";

		g_pSkin->InitalizeSkins();

		Settings::LoadSettings(IniFile);

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();

		return true;
	}

	void Shutdown()
	{
		DELETE_MOD(g_pPlayers);
		DELETE_MOD(g_pRender);
		DELETE_MOD(g_pGui);

		DELETE_MOD(g_pAimbot);
		DELETE_MOD(g_pTriggerbot);
		DELETE_MOD(g_pEsp);
		DELETE_MOD(g_pRadar);
		DELETE_MOD(g_pKnifebot);
		DELETE_MOD(g_pSkin);
		DELETE_MOD(g_pMisc);
	}

	void OnRender()
	{
		if (g_pRender && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			g_pRender->BeginRender();

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();

			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			g_vCenterScreen.x = iScreenWidth / 2.f;
			g_vCenterScreen.y = iScreenHeight / 2.f;

			if (!Interfaces::Engine()->IsConnected())
				g_pRender->Text(3, 3, false, true, Color::Aqua(), HACK_NAME);

			if (Client::g_pPlayers && Client::g_pPlayers->GetLocal() && Interfaces::Engine()->IsInGame())
			{
				if (g_pEsp)
					g_pEsp->OnRender();

				if (g_pMisc)
				{
					g_pMisc->OnRender();
					g_pMisc->OnRenderSpectatorList();
				}
			}

			g_pRender->EndRender();
		}
	}

	void OnLostDevice()
	{
		if (g_pRender)
			g_pRender->OnLostDevice();

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void OnResetDevice()
	{
		if (g_pRender)
			g_pRender->OnResetDevice();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void OnCreateMove(CUserCmd* pCmd)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{
			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pTriggerbot)
					g_pTriggerbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pKnifebot)
					g_pKnifebot->OnCreateMove(pCmd);

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);
			}
		}
	}

	void OnFireEventClientSideThink(IGameEvent* pEvent)
	{
		if (!strcmp(pEvent->GetName(), "player_connect_full") ||
			!strcmp(pEvent->GetName(), "round_start") ||
			!strcmp(pEvent->GetName(), "cs_game_disconnected"))
		{
			if (g_pPlayers)
				g_pPlayers->Clear();

			if (g_pEsp)
				g_pEsp->OnReset();
		}

		if (Interfaces::Engine()->IsConnected())
		{
			if (g_pEsp)
				g_pEsp->OnEvents(pEvent);

			if (g_pSkin)
				g_pSkin->OnEvents(pEvent);
		}
	}

	void OnFrameStageNotify(ClientFrameStage_t Stage)
	{
		if (Interfaces::Engine()->IsInGame())
		{
			Skin_OnFrameStageNotify(Stage);
			Gloves_OnFrameStageNotify(Stage);
		}
	}

	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
		const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Interfaces::Engine()->IsInGame() && ctx && pCustomBoneToWorld)
		{
			if (g_pEsp)
				g_pEsp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (g_pMisc)
				g_pMisc->OnDrawModelExecute();
		}
	}

	void OnPlaySound(const Vector* pOrigin, const char* pszSoundName)
	{
		if (!pszSoundName || !Interfaces::Engine()->IsInGame())
			return;

		if (!strstr(pszSoundName, "bulletLtoR") &&
			!strstr(pszSoundName, "rics/ric") &&
			!strstr(pszSoundName, "impact_bullet"))
		{
			if (g_pEsp && IsLocalAlive() && Settings::Esp::esp_Sound && pOrigin)
			{
				if (!GetVisibleOrigin(*pOrigin))
					g_pEsp->SoundEsp.AddSound(*pOrigin);
			}
		}
	}

	void OnPlaySound(const char* pszSoundName)
	{
		if (g_pMisc)
			g_pMisc->OnPlaySound(pszSoundName);
	}

	void OnOverrideView(CViewSetup* pSetup)
	{
		if (g_pMisc)
			g_pMisc->OnOverrideView(pSetup);
	}

	void OnGetViewModelFOV(float& fov)
	{
		if (g_pMisc)
			g_pMisc->OnGetViewModelFOV(fov);
	}

	void OnRenderGUI()
	{
		ImGui::SetNextWindowSize(ImVec2(560.f, 325.f));

		if (ImGui::Begin(HACK_NAME, &bIsGuiVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType > 1)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType = 1;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit > 1)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit = 1;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot > 5)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot = 5;

			const char* tabNames[] = {
				AIMBOT_TEXT , TRIGGER_TEXT , VISUAL_TEXT , RADAR_TEXT ,
				KNIFEBOT_TEXT , SKIN_TEXT , MISC_TEXT , CONFIG_TEXT };

			static int tabOrder[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };
			static int tabSelected = 0;
			const bool tabChanged = ImGui::TabLabels(tabNames,
				sizeof(tabNames) / sizeof(tabNames[0]),
				tabSelected, tabOrder);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float SpaceLineOne = 120.f;
			float SpaceLineTwo = 220.f;
			float SpaceLineThr = 320.f;

			if (tabSelected == 0) // Aimbot
			{
				ImGui::PushItemWidth(110.f);
				ImGui::Text("Current Weapon: ");
				ImGui::SameLine();
				ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Deathmatch", &Settings::Aimbot::aim_Deathmatch);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("WallAttack", &Settings::Aimbot::aim_WallAttack);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("CheckSmoke", &Settings::Aimbot::aim_CheckSmoke);

				ImGui::Checkbox("AntiJump", &Settings::Aimbot::aim_AntiJump);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Draw Fov", &Settings::Aimbot::aim_DrawFov);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("DrawSpot", &Settings::Aimbot::aim_DrawSpot);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

				if (iWeaponID <= 9)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Autopistol", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol);
				}

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Smooth, 1, 300);
				ImGui::SliderInt("Fov", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Fov, 1, 300);
				ImGui::PopItemWidth();

				const char* AimFovType[] = { "Dynamic" , "Static" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("Fov Type", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType, AimFovType, IM_ARRAYSIZE(AimFovType));
				ImGui::PopItemWidth();

				const char* BestHit[] = { "Disable" , "Enable" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("BestHit", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit, BestHit, IM_ARRAYSIZE(BestHit));

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("if disabled then used Aimspot");

				ImGui::PopItemWidth();

				const char* Aimspot[] = { "Head" , "Neck" , "Low Neck" , "Body" , "Thorax" , "Chest" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("Aimspot", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot, Aimspot, IM_ARRAYSIZE(Aimspot));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("ShotDelay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
				ImGui::SliderInt("Rcs", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Rcs, 0, 100);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (iWeaponID >= 10 && iWeaponID <= 30)
				{
					ImGui::PushItemWidth(362.f);
					ImGui::SliderInt("Rcs Fov", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsFov, 1, 300);
					ImGui::SliderInt("Rcs Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsSmooth, 1, 300);
					ImGui::PopItemWidth();

					const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
					ImGui::PushItemWidth(362.f);
					ImGui::Combo("Rcs Clamp", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType, ClampType, IM_ARRAYSIZE(ClampType));
					ImGui::PopItemWidth();

					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
				}
			}
			else if (tabSelected == 1) // Trigger
			{
				const char* TriggerEnable[] = { "Disable" , "Fov" , "Trace" };
				ImGui::PushItemWidth(80.f);
				ImGui::Combo("Enable", &Settings::Triggerbot::trigger_Enable, TriggerEnable, IM_ARRAYSIZE(TriggerEnable));
				ImGui::PopItemWidth();
				ImGui::SameLine();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Deathmatch", &Settings::Triggerbot::trigger_Deathmatch);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("WallAttack", &Settings::Triggerbot::trigger_WallAttack);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("FastZoom", &Settings::Triggerbot::trigger_FastZoom);

				ImGui::Checkbox("SmokCheck", &Settings::Triggerbot::trigger_SmokCheck);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("DrawFov", &Settings::Triggerbot::trigger_DrawFov);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("DrawSpot", &Settings::Triggerbot::trigger_DrawSpot);
				ImGui::SameLine(SpaceLineThr);
				ImGui::Checkbox("DrawFovAssist", &Settings::Triggerbot::trigger_DrawFovAssist);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				const char* items1[] = { CVAR_KEY_MOUSE3 , CVAR_KEY_MOUSE4 , CVAR_KEY_MOUSE5 };
				ImGui::PushItemWidth(80.f);
				ImGui::Combo("Key", &Settings::Triggerbot::trigger_Key, items1, IM_ARRAYSIZE(items1));
				ImGui::PopItemWidth();
				ImGui::SameLine();

				const char* items2[] = { "Hold" , "Press" };
				ImGui::PushItemWidth(80.f);
				ImGui::Combo("Key Mode", &Settings::Triggerbot::trigger_KeyMode, items2, IM_ARRAYSIZE(items2));
				ImGui::PopItemWidth();
				ImGui::SameLine();

				ImGui::PushItemWidth(110.f);
				ImGui::Combo("Weapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Min Disstance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMin, 0, 5000);
				ImGui::SliderInt("Max Disstance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMax, 0, 5000);
				ImGui::SliderInt("Fov", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov, 1, 100);
				ImGui::SliderInt("Delay Before", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayBefore, 0, 200);
				ImGui::SliderInt("Delay After", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayAfter, 0, 1000);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("HeadOnly", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly);
				ImGui::SameLine();

				const char* AssistMode[] = { "Disable" , "One Shot" , "Auto" };
				ImGui::PushItemWidth(80.f);
				ImGui::Combo("Assist", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist, AssistMode, IM_ARRAYSIZE(AssistMode));
				ImGui::PopItemWidth();
				ImGui::SameLine();

				const char* AssistFovType[] = { "Dynamic" , "Static" };
				ImGui::PushItemWidth(80.f);
				ImGui::Combo("Assist Fov Type", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType, AssistFovType, IM_ARRAYSIZE(AssistFovType));
				ImGui::PopItemWidth();

				const char* HitGroup[] = { "All" , "Head + Body" , "Head" };
				ImGui::Combo("HitGroup", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup, HitGroup, IM_ARRAYSIZE(HitGroup));
				//ImGui::PopItemWidth();

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Assist Rcs", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs, 0, 100);
				ImGui::SliderInt("Assist Fov", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov, 1, 300);
				ImGui::SliderInt("Assist Smooth", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistSmooth, 1, 300);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
			}
			else if (tabSelected == 2) // Visuals
			{
				string style_1 = "Box";
				string style_2 = "CoalBox";

				const char* items1[] = { style_1.c_str() , style_2.c_str() };

				ImGui::PushItemWidth(339.f);
				ImGui::Combo("Esp Type", &Settings::Esp::esp_Style, items1, IM_ARRAYSIZE(items1));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Esp Team", &Settings::Esp::esp_Team);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Esp Enemy", &Settings::Esp::esp_Enemy);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("Esp Bomp", &Settings::Esp::esp_Bomb);
				ImGui::SameLine(SpaceLineThr);
				ImGui::Checkbox("Esp Sound", &Settings::Esp::esp_Sound);

				ImGui::Checkbox("Esp Line", &Settings::Esp::esp_Line);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Esp OutLine", &Settings::Esp::esp_Outline);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("Esp Name", &Settings::Esp::esp_Name);
				ImGui::SameLine(SpaceLineThr);
				ImGui::Checkbox("Esp Rank", &Settings::Esp::esp_Rank);

				ImGui::Checkbox("Esp Weapon", &Settings::Esp::esp_Weapon);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Esp Ammo", &Settings::Esp::esp_Ammo);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::Checkbox("Esp Distance", &Settings::Esp::esp_Distance);
				ImGui::SameLine(SpaceLineThr);
				ImGui::Checkbox("Esp Skeleton", &Settings::Esp::esp_Skeleton);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Esp World Weapon", &Settings::Esp::esp_WorldWeapons);
				ImGui::Checkbox("Esp World Grenade", &Settings::Esp::esp_WorldGrenade);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				string visible_1 = "Enemy";
				string visible_2 = "Team";
				string visible_3 = "All";
				string visible_4 = "Only Visible";

				const char* items2[] = { visible_1.c_str() , visible_2.c_str() , visible_3.c_str() , visible_4.c_str() };

				ImGui::PushItemWidth(339.f);
				ImGui::Combo("Esp Visible", &Settings::Esp::esp_Visible, items2, IM_ARRAYSIZE(items2));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::SliderInt("Esp Size", &Settings::Esp::esp_Size, 0, 10);
				ImGui::SliderInt("Esp BombTimer", &Settings::Esp::esp_BombTimer, 0, 65);
				ImGui::SliderInt("Esp BulletTrace", &Settings::Esp::esp_BulletTrace, 0, 3000);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				string hpbar_1 = "None";
				string hpbar_2 = "Number";
				string hpbar_3 = "Bottom";
				string hpbar_4 = "Left";

				const char* items3[] = { hpbar_1.c_str() , hpbar_2.c_str() , hpbar_3.c_str() , hpbar_4.c_str() };
				ImGui::Combo("Esp Health", &Settings::Esp::esp_Health, items3, IM_ARRAYSIZE(items3));

				string arbar_1 = "None";
				string arbar_2 = "Number";
				string arbar_3 = "Bottom";
				string arbar_4 = "Right";

				const char* items4[] = { arbar_1.c_str() , arbar_2.c_str() , arbar_3.c_str() , arbar_4.c_str() };
				ImGui::Combo("Esp Armor", &Settings::Esp::esp_Armor, items4, IM_ARRAYSIZE(items4));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				string chams_1 = "None";
				string chams_2 = "Flat";
				string chams_3 = "Texture";

				const char* items5[] = { chams_1.c_str() , chams_2.c_str() , chams_3.c_str() };
				ImGui::Combo("Chams", &Settings::Esp::esp_Chams, items5, IM_ARRAYSIZE(items5));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::ColorEdit3("Esp Color CT", Settings::Esp::esp_Color_CT);
				ImGui::ColorEdit3("Esp Color TT", Settings::Esp::esp_Color_TT);
				ImGui::ColorEdit3("Esp Color Visible CT", Settings::Esp::esp_Color_VCT);
				ImGui::ColorEdit3("Esp Color Visible TT", Settings::Esp::esp_Color_VTT);

				ImGui::ColorEdit3("Chams Color CT", Settings::Esp::chams_Color_CT);
				ImGui::ColorEdit3("Chams Color TT", Settings::Esp::chams_Color_TT);
				ImGui::ColorEdit3("Chams Color Visible CT", Settings::Esp::chams_Color_VCT);
				ImGui::ColorEdit3("Chams Color Visible TT", Settings::Esp::chams_Color_VTT);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
			}
			else if (tabSelected == 3) // Radar
			{
				ImGui::Checkbox("Active", &Settings::Radar::rad_Active);
				ImGui::SameLine();
				ImGui::Checkbox("Team", &Settings::Radar::rad_Team);
				ImGui::SameLine();
				ImGui::Checkbox("Enemy", &Settings::Radar::rad_Enemy);
				ImGui::SameLine();
				ImGui::Checkbox("Sound", &Settings::Radar::rad_Sound);
				ImGui::SameLine();
				ImGui::Checkbox("InGame", &Settings::Radar::rad_InGame);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(339.f);
				ImGui::SliderInt("Range", &Settings::Radar::rad_Range, 1, 5000);
				ImGui::SliderInt("Alpha", &Settings::Radar::rad_Alpha, 1, 255);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::ColorEdit3("Color CT", Settings::Radar::rad_Color_CT);
				ImGui::ColorEdit3("Color TT", Settings::Radar::rad_Color_TT);
				ImGui::ColorEdit3("Color Visible CT", Settings::Radar::rad_Color_VCT);
				ImGui::ColorEdit3("Color Visible TT", Settings::Radar::rad_Color_VTT);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
			}
			else if (tabSelected == 4) // knifebot
			{
				ImGui::Checkbox("Active", &Settings::Knifebot::knf_Active);
				ImGui::Checkbox("Attack Team", &Settings::Knifebot::knf_Team);

				ImGui::Separator();

				string attack_1 = "Attack 1";
				string attack_2 = "Attack 2";
				string attack_3 = "Attack 1 + Attack 2";

				const char* items[] = { attack_1.c_str() , attack_2.c_str() , attack_3.c_str() };
				ImGui::Combo("Attack type", &Settings::Knifebot::knf_Attack, items, IM_ARRAYSIZE(items));

				ImGui::Separator();

				ImGui::SliderInt("Dist to attack 1", &Settings::Knifebot::knf_DistAttack, 1, 100);
				ImGui::SliderInt("Dist to attack 2", &Settings::Knifebot::knf_DistAttack2, 1, 100);
			}
			else if (tabSelected == 5) // Skins
			{
				//[enc_string_disable /]
				const char* knife_models_items[] =
				{
					"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
					"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers"
				};

				const char* quality_items[] =
				{
					"Normal","Genuine","Vintage","Unusual","Community","Developer",
					"Self-Made","Customized","Strange","Completed","Tournament"
				};

				const char* gloves_listbox_items[25] =
				{
					"default",
					"bloodhound_black_silver","bloodhound_snakeskin_brass","bloodhound_metallic","handwrap_leathery",
					"handwrap_camo_grey","slick_black","slick_military","slick_red","sporty_light_blue","sporty_military",
					"handwrap_red_slaughter","motorcycle_basic_black","motorcycle_mint_triangle","motorcycle_mono_boom",
					"motorcycle_triangle_blue","specialist_ddpat_green_camo","specialist_kimono_diamonds_red",
					"specialist_emerald_web","specialist_orange_white","handwrap_fabric_orange_camo","sporty_purple",
					"sporty_green","bloodhound_guerrilla","slick_snakeskin_yellow"
				};
				//[enc_string_enable /]

			
				ImGui::Text("Current Weapon: %s", pWeaponData[iWeaponID]);

				ImGui::PushItemWidth(362.f);

				ImGui::Separator();

				ImGui::Combo("Knife CT Model", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
				ImGui::Combo("Knife TT Model", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));

				ImGui::Separator();

				static int iSelectKnifeCTSkinIndex = -1;
				static int iSelectKnifeTTSkinIndex = -1;

				int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
				int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

				static int iOldKnifeCTModelIndex = -1;
				static int iOldKnifeTTModelIndex = -1;

				if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
					iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

				if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
					iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

				iOldKnifeCTModelIndex = iKnifeCTModelIndex;
				iOldKnifeTTModelIndex = iKnifeTTModelIndex;

				string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
				string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

				KnifeCTModel += " Skin##KCT";
				KnifeTTModel += " Skin##KTT";

				ImGui::SliderFloat("Knife CT Wear", &g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, 0.001f, 1.f);
				ImGui::Combo("Knife CT Qality", &g_SkinChangerCfg[WEAPON_KNIFE].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
				ImGui::ComboBoxArray(KnifeCTModel.c_str(), &iSelectKnifeCTSkinIndex, KnifeSkins[iKnifeCTModelIndex].SkinNames);

				ImGui::Separator();

				ImGui::SliderFloat("Knife TT Wear", &g_SkinChangerCfg[WEAPON_KNIFE_T].flFallbackWear, 0.001f, 1.f);
				ImGui::Combo("Knife TT Qality", &g_SkinChangerCfg[WEAPON_KNIFE_T].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
				ImGui::ComboBoxArray(KnifeTTModel.c_str(), &iSelectKnifeTTSkinIndex, KnifeSkins[iKnifeTTModelIndex].SkinNames);

				ImGui::Separator();

				static int iOldWeaponID = -1;

				ImGui::Combo("Weapon##WeaponSelect", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));

				iWeaponSelectIndex = pWeaponItemIndexData[iWeaponID];

				if (iOldWeaponID != iWeaponID)
					iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

				iOldWeaponID = iWeaponID;

				string WeaponSkin = pWeaponData[iWeaponID];
				WeaponSkin += " Skin";

				ImGui::ComboBoxArray(WeaponSkin.c_str(), &iWeaponSelectSkinIndex, WeaponSkins[iWeaponID].SkinNames);

				ImGui::Combo("Weapon Qality", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
				ImGui::SliderFloat("Weapon Wear", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].flFallbackWear, 0.001f, 1.f);
				ImGui::InputInt("Weapon StatTrak", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackStatTrak, 1, 100, ImGuiInputTextFlags_CharsDecimal);

				ImGui::Separator();

				ImGui::Combo("Gloves Skin", &Settings::Skin::gloves_skin, gloves_listbox_items,
					IM_ARRAYSIZE(gloves_listbox_items));

				ImGui::Separator();

				ImGui::PopItemWidth();

				if (ImGui::Button("Apply##Skin"))
				{
					if (iWeaponSelectSkinIndex >= 0) {
						g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit = WeaponSkins[iWeaponID].SkinPaintKit[iWeaponSelectSkinIndex];
					}
					if (iSelectKnifeCTSkinIndex >= 0) {
						Settings::Skin::knf_ct_skin = KnifeSkins[iKnifeCTModelIndex].SkinPaintKit[iSelectKnifeCTSkinIndex];
					}
					if (iSelectKnifeTTSkinIndex >= 0) {
						Settings::Skin::knf_tt_skin = KnifeSkins[iKnifeTTModelIndex].SkinPaintKit[iSelectKnifeTTSkinIndex];
					}

					ForceFullUpdate();
				}
			}
			else if (tabSelected == 6) // Misc
			{
				ImGui::Checkbox("Bhop", &Settings::Misc::misc_Bhop);
				ImGui::Checkbox("Punch", &Settings::Misc::misc_Punch);
				ImGui::Checkbox("AwpAim", &Settings::Misc::misc_AwpAim);
				ImGui::Checkbox("NoFlash", &Settings::Misc::misc_NoFlash);
				ImGui::Checkbox("AutoStrafe", &Settings::Misc::misc_AutoStrafe);
				ImGui::Checkbox("AutoAccept", &Settings::Misc::misc_AutoAccept);
				ImGui::Checkbox("Spectators", &Settings::Misc::misc_Spectators);
				ImGui::Checkbox("Fov Changer", &Settings::Misc::misc_FovChanger);
				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Fov View", &Settings::Misc::misc_FovView, 1, 190);
				ImGui::SliderInt("Fov Model View", &Settings::Misc::misc_FovModelView, 1, 190);
				ImGui::Separator();
				ImGui::ColorEdit3("Awp Aim Color", Settings::Misc::misc_AwpAimColor);
				ImGui::PopItemWidth();
			}
			else if (tabSelected == 7) // Config
			{
				static int iConfigSelect = 0;
				static int iMenuTheme = 1;
				static char ConfigName[64] = { 0 };

				ImGui::ComboBoxArray("Select Config", &iConfigSelect, ConfigList);

				ImGui::Separator();

				if (ImGui::Button("Load Config"))
				{
					Settings::LoadSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save Config"))
				{
					Settings::SaveSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
				}
				ImGui::SameLine();
				if (ImGui::Button("Refresh Config List"))
				{
					RefreshConfigs();
				}

				ImGui::Separator();

				ImGui::InputText("Config Name", ConfigName, 64);

				if (ImGui::Button("Create & Save new Config"))
				{
					string ConfigFileName = ConfigName;

					if (ConfigFileName.size() < 1)
					{
						ConfigFileName = "settings";
					}

					Settings::SaveSettings(BaseDir + "\\" + ConfigFileName + ".ini");
					RefreshConfigs();
				}

				ImGui::Separator();

				const char* ThemesList[] = { "Default" , "Red" };

				ImGui::Combo("Menu Color Theme", &iMenuTheme, ThemesList, IM_ARRAYSIZE(ThemesList));

				ImGui::Separator();

				if (ImGui::Button("Apply Theme"))
				{
					if (iMenuTheme == 0)
					{
						g_pGui->DefaultTheme();
					}
					else if (iMenuTheme == 1)
					{
						g_pGui->RedTheme();
					}
				}
			}

			ImGui::End();
		}
	}
}