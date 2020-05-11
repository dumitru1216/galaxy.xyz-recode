#include "menu_system.h"
#include "config_system.h"
#include "..\valve_sdk\Vector.h"
#include "..\valve_sdk\ISurface.h"
#include "..\valve_utils\Color.h"
#include "..\valve_utils\GlobalVars.h"
#include "..\valve_utils\XorStr.h"
#include "../c_hooks.h"
#include "bgtexture.h"
#include "std.h"
#include <d3dx9tex.h>
#include "bindsfont.h"
#include "../hooking_mode/directx_font.h"
IDirect3DTexture9* skeet_texture = nullptr;
c_menu menu;
IDirect3DDevice9* dev;

ImFont* tabs;
ImFont* def;
ImFont* Normal;
bool save_config = false;
bool load_config = false;


void c_menu::initialize( IDirect3DDevice9* pDevice ) {
	if (this->m_bInitialized)
		return;

	ui::CreateContext( );
	auto io = ui::GetIO( );
	auto style = &ui::GetStyle( );

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 6.f;
	style->WindowPadding = ImVec2( 0, 0 );
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4( 45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f );
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4( 65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f );

	this->m_bIsOpened = false;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters( &param );	pDevice->GetSwapChain( 0, &pChain );


	if (pChain)
		pChain->GetPresentParameters( &pp );

	ImGui_ImplWin32_Init( param.hFocusWindow );
	ImGui_ImplDX9_Init( pDevice );
	dev = pDevice;

	ImFontConfig cfg;
	io.Fonts->AddFontFromFileTTF( "C:/windows/fonts/verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromFileTTF( "C:/windows/fonts/verdanab.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromMemoryTTF( keybinds_font, 25600, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	tabs = io.Fonts->AddFontFromMemoryTTF( skeet, 15400, 50.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	def = io.Fonts->AddFontFromMemoryTTF( skeet, 15400, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontDefault( );




	ImGuiFreeType::BuildFontAtlas( io.Fonts, 0x00 );

	this->m_bInitialized = true;
}

void c_menu::draw_begin( ) {
	if (!this->m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ui::NewFrame( );
}

void c_menu::draw( )
{


	if (!this->m_bIsOpened && ui::GetStyle( ).Alpha > 0.f) {
		float fc = 255.f / 0.2f * ui::GetIO( ).DeltaTime;
		ui::GetStyle( ).Alpha = std::clamp( ui::GetStyle( ).Alpha - fc / 255.f, 0.f, 1.f );
	}

	if (this->m_bIsOpened && ui::GetStyle( ).Alpha < 1.f) {
		float fc = 255.f / 0.2f * ui::GetIO( ).DeltaTime;
		ui::GetStyle( ).Alpha = std::clamp( ui::GetStyle( ).Alpha + fc / 255.f, 0.f, 1.f );
	}

	if (!this->m_bIsOpened && ui::GetStyle( ).Alpha == 0.f)
		return;

	static auto child_pos_backwend = []( int num ) -> ImVec2 {
		return ImVec2( ui::GetWindowPos( ).x + 20 + (ui::GetWindowSize( ).x / 2 - 70) * num + 20 * num, ui::GetWindowPos( ).y + 45 );
	};

	ui::GetIO( ).MouseDrawCursor = menuOpened;

	ui::SetNextWindowSizeConstraints( ImVec2( 580, 645 ), ImVec2( 4096, 4096 ) );
	ui::Begin( "0x8C2", 0, ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoScrollbar );

	ui::TabButton( "ragebot", &this->m_nCurrentTab, 0, 7 );
	ui::TabButton( "legitbot", &this->m_nCurrentTab, 1, 7 );
	ui::TabButton( "anti-aim", &this->m_nCurrentTab, 2, 7 );
	ui::TabButton( "visuals", &this->m_nCurrentTab, 3, 7 );
	ui::TabButton( "skins", &this->m_nCurrentTab, 4, 7 );
	ui::TabButton( "misc", &this->m_nCurrentTab, 5, 7 );
	ui::TabButton( "settings", &this->m_nCurrentTab, 6, 7 );

	if (this->m_nCurrentTab == 0)
	{
		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Config", ImVec2( 265, 150 ) );
		{
			ui::Checkbox( "enable aimbot", &galaxy_vars.cfg.Aimbot );
			ui::Checkbox( "automatic stop", &galaxy_vars.cfg.Autostop );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2(20, 205) );
		ui::BeginChild( "Aimbot", ImVec2( 265, 440 ) );
		{
			ui::SliderInt( "hitchance", &galaxy_vars.cfg.HitchanceValue, 0, 100, "%.f%%" );
			ui::SliderInt( "minimum damage", &galaxy_vars.cfg.Mindmg, 0, 100, "%.f%%" );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 45 ) );
		ui::BeginChild( "Hitscan", ImVec2( 265, 350 ) );
		{
			ui::Checkbox( "multipoint", &galaxy_vars.cfg.MultiPoint );
			if (galaxy_vars.cfg.MultiPoint)
			{
				ui::Checkbox( "head", &galaxy_vars.cfg.head );
				//ui::SameLine( );
				ui::Checkbox( "body", &galaxy_vars.cfg.body );
				//ui::SameLine( );
				ui::Checkbox( "neck", &galaxy_vars.cfg.neck );
				ui::Checkbox( "legs", &galaxy_vars.cfg.legs );
				//ui::SameLine( );
				ui::Checkbox( "arms", &galaxy_vars.cfg.arms );
			}
			ui::Text( "multipoint scale" );
			ui::SliderInt( "head scale", &galaxy_vars.cfg.HeadScale, 0, 100, "%.f%%" );
			ui::SliderInt( "body scale", &galaxy_vars.cfg.BodyScale, 0, 100, "%.f%%" );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 405 ) );
		ui::BeginChild( "Exploits", ImVec2( 265, 240 ) );
		{
			ui::Checkbox( "airstuck exploit", &galaxy_vars.cfg.air_stuck );
			if (galaxy_vars.cfg.air_stuck)
			{
				ui::Keybind( "airstuck key", &galaxy_vars.cfg.air_key );
			}
			ui::Checkbox( "air desync exploit", &galaxy_vars.cfg.air_desync );
		}
		ui::EndChild( );

	}

	if (this->m_nCurrentTab == 1)
	{

	}

	if (this->m_nCurrentTab == 2)
	{

		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Config", ImVec2( 265, 150 ) );
		{
			ui::Checkbox( "enable anti-aim", &galaxy_vars.cfg.Antiaim );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 20, 205 ) );
		ui::BeginChild( "anti-hit angles", ImVec2( 265, 440 ) );
		{
			
			ui::SingleSelect( "yaw/desync", &galaxy_vars.cfg.yaw_desync, { "off", "backwars", "static desync" } );
			if (galaxy_vars.cfg.yaw_desync == 2)
			{
				ui::Keybind( "desync swap", &galaxy_vars.cfg.desync_swap );
				ui::SliderInt( "body lean", &galaxy_vars.cfg.body_lean, 0, 300, "%.f%%" );
				ui::SliderInt( "body invert", &galaxy_vars.cfg.body_invert, 0, 300, "%.f%%" );
			}
			ui::SliderInt( "fakelag ticks", &galaxy_vars.cfg.Fakelag, 0, 15, "%.f%%" );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 45 ) );
		ui::BeginChild( "Other", ImVec2( 265, 600 ) );
		{

		}
		ui::EndChild( );


	}

	if (this->m_nCurrentTab == 3)
	{
		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Player esp", ImVec2( 265, 350 ) );
		{
			ui::Checkbox( "enable visuals", &galaxy_vars.cfg.Esp );
			ui::Checkbox( "bounding box", &galaxy_vars.cfg.Box );
			ui::SameLine( );
			ui::ColorEdit4( "###box_color", galaxy_vars.cfg.box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ui::Checkbox( "name", &galaxy_vars.cfg.name );
			ui::SameLine( );
			ui::ColorEdit4( "###name_color", galaxy_vars.cfg.name_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ui::Checkbox( "health bar", &galaxy_vars.cfg.HealthBar );
			ui::Checkbox( "weapon", &galaxy_vars.cfg.weapon_name );
			ui::Checkbox( "ammo bar", &galaxy_vars.cfg.ammo_bar );
			ui::SameLine( );
			ui::ColorEdit4( "###ammo_color", galaxy_vars.cfg.ammo_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ui::Checkbox( "flags", &galaxy_vars.cfg.flags );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 20, 400 ) );
		ui::BeginChild( "Chams", ImVec2( 265, 250 ) );
		{
			static int chams_mode = 0;
			ui::SingleSelect( "chams on:", &chams_mode, { "entity", "local", "other" } );
			switch (chams_mode)
			{
				case 0:
				{
					ui::Checkbox( "enemy chams", &galaxy_vars.cfg.chams_enemy );
					ui::SameLine( );
					ui::ColorEdit4( "###normal_chamsenem", galaxy_vars.cfg.enemy_visible_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::Checkbox( "enemy chams invisible", &galaxy_vars.cfg.enemy_invisible );
					ui::SingleSelect( "enemy materials [ normal ]", &galaxy_vars.cfg.normal_enemy_material, { "textured", "eso", "glow" } );
					ui::Checkbox( "enemy chams overide", &galaxy_vars.cfg.enemy_overide_chams );
					ui::SameLine( );
					ui::ColorEdit4( "###overidecolorenemy", galaxy_vars.cfg.enemy_overide_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "enemy materials [ overide ]", &galaxy_vars.cfg.overide_enemy_material, { "textured", "eso", "glow" } );

				}
				break;
				case 1:
				{
					ui::Checkbox( "local chams", &galaxy_vars.cfg.local_chams );
					ui::SameLine( );
					ui::ColorEdit4( "###noremal_locachams", galaxy_vars.cfg.local_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "local materials [ normal ]", &galaxy_vars.cfg.local_materials, { "textured", "eso", "glow" } );
					ui::Checkbox( "local chams overide", &galaxy_vars.cfg.local_chams_overide );
					ui::SameLine( );
					ui::ColorEdit4( "###localcamsoveride", galaxy_vars.cfg.local_overide_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "local materials [ overide ]", &galaxy_vars.cfg.local_overide_materials, { "textured", "eso", "glow" } );

					ui::Checkbox( "desync chams", &galaxy_vars.cfg.desync_chams );
					ui::SameLine( );
					ui::ColorEdit4( "###desyncncamayte", galaxy_vars.cfg.desync_chams_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "desync materials [ normal ]", &galaxy_vars.cfg.desync_chams_material, { "textured", "eso", "glow" } );
					ui::Checkbox( "desync chams overide", &galaxy_vars.cfg.deysnc_Overid );
					ui::SameLine( );
					ui::ColorEdit4( "###desocverdidechams", galaxy_vars.cfg.desync_chams_overide, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "desync materials [ overide ]", &galaxy_vars.cfg.desync_chams_overide_material, { "textured", "eso", "glow" } );

				}
				break;
				case 2:
				{
					ui::Checkbox( "hand chams", &galaxy_vars.cfg.hand_chams );
					ui::SameLine( );
					ui::ColorEdit4( "###handnormalcaolor", galaxy_vars.cfg.hand_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "hand materials [ normal ]", &galaxy_vars.cfg.hand_materials, { "textured", "eso", "glow" } );
					ui::Checkbox( "hand chams overide", &galaxy_vars.cfg.hand_chams_overide );
					ui::SameLine( );
					ui::ColorEdit4( "###handoveridecolor", galaxy_vars.cfg.hand_overide_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
					ui::SingleSelect( "hand materials [ overide ]", &galaxy_vars.cfg.hand_materials_overide, { "textured", "eso", "glow" } );

				}
				break;

			}

		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 45 ) );
		ui::BeginChild( "Other", ImVec2( 265, 350 ) );
		{
			ui::Checkbox( "world modulation", &galaxy_vars.cfg.NightMode );
			ui::SliderInt( "nighmode scale", &galaxy_vars.cfg.NightModeSlider, 0, 100, "%.f%%" );
			ui::Text( "sky color" );
			ui::SameLine( );
			ui::ColorEdit4( "###skycolor", galaxy_vars.cfg.sky_box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ui::Checkbox( "top bar", &galaxy_vars.cfg.top_bar_enable );
			ui::Checkbox( "spectators list", &galaxy_vars.cfg.specator_list );

		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 405 ) );
		ui::BeginChild( "Effects", ImVec2( 265, 240 ) );
		{
			ui::Checkbox( "remove scope", &galaxy_vars.cfg.NoScope );
			ui::Checkbox( "remove visual recoil", &galaxy_vars.cfg.NoRecoil );
			ui::Checkbox( "remove zoom", &galaxy_vars.cfg.NoZoom );
			ui::Checkbox( "remove smoke", &galaxy_vars.cfg.no_smoke );
		}
		ui::EndChild( );
	}

	if (this->m_nCurrentTab == 4) // skins
	{
		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Config", ImVec2( 265, 150 ) );
		{
			
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 20, 205 ) );
		ui::BeginChild( "Knifes", ImVec2( 265, 440 ) );
		{

		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 45 ) );
		ui::BeginChild( "Gloves", ImVec2( 265, 350 ) );
		{

		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 405 ) );
		ui::BeginChild( "Weapons", ImVec2( 265, 240 ) );
		{

		}
		ui::EndChild( );
	}

	if (this->m_nCurrentTab == 5)
	{
		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Movement", ImVec2( 265, 150 ) );
		{
			ui::Checkbox( "automatic bhop", &galaxy_vars.cfg.Bhop );
			ui::Checkbox( "automatic strafe", &galaxy_vars.cfg.AutoStrafe );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 20, 205 ) );
		ui::BeginChild( "Other", ImVec2( 265, 440 ) );
		{
			ui::Text( "menu color" );
			ui::SameLine( );
			ui::ColorEdit4( "###mmen0", galaxy_vars.cfg.menu_color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs );
			ui::Checkbox( "slowwalk", &galaxy_vars.cfg.slowwalk );
			if (galaxy_vars.cfg.slowwalk)
			{
				ui::SliderInt( "slowwalk speed", &galaxy_vars.cfg.slowspeed, 0, 100, "%.f%%" );
			}
			ui::Checkbox( "server hitboxes", &galaxy_vars.cfg.server_hitobx );
		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 45 ) );
		ui::BeginChild( "Premium", ImVec2( 265, 350 ) );
		{

		}
		ui::EndChild( );
		ui::SetCursorPos( ImVec2( 295, 405 ) );
		ui::BeginChild( "Dev mode", ImVec2( 265, 240 ) );
		{

		}
		ui::EndChild( );
	}

	if (this->m_nCurrentTab == 6)
	{
		ui::SetNextWindowPos( child_pos_backwend( 0 ) );
		ui::BeginChild( "Config", ImVec2( 265, 600 ) );
		{
			constexpr auto& config_items = galaxy_vars.get_configs( );
			static int current_config = -1;

			if (static_cast<size_t>(current_config) >= config_items.size( ))
				current_config = -1;

			static char buffer[16];

			if (ui::ListBox( "", &current_config, []( void* data, int idx, const char** out_text ) {
				auto& vector = *static_cast<std::vector<std::string>*>(data);
				*out_text = vector[idx].c_str( );
				return true;
			}, &config_items, config_items.size( ), 5 ) && current_config != -1)
				strcpy( buffer, config_items[current_config].c_str( ) );


			ui::PushID( 0 );
			ui::PushItemWidth( 178 );
			if (ui::InputText( "", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue )) {
				if (current_config != -1)
					galaxy_vars.rename( current_config, buffer );
			}
			ui::PopID( );
			ui::NextColumn( );


			if (ui::Button( ("create"), ImVec2( 85, 20 ) )) {
				galaxy_vars.add( buffer );
			}

			ui::SameLine( );

			if (ui::Button( ("reset"), ImVec2( 85, 20 ) )) {
				galaxy_vars.reset( );
			}


			if (current_config != -1) {
				if (ui::Button( ("load"), ImVec2( 85, 20 ) )) {
					galaxy_vars.load( current_config );

					load_config = true;

				}

				ui::SameLine( );

				if (ui::Button( ("save"), ImVec2( 85, 20 ) )) {
					galaxy_vars.save( current_config );

					save_config = true;

				}


				if (ui::Button( ("remove"), ImVec2( 85, 20 ) )) {
					galaxy_vars.remove( current_config );
				}
			}
		}
		ui::EndChild( );
		ui::SameLine( );
		ui::BeginChild( "Lua", ImVec2( 265, 600 ) );
		{

		}
		ui::EndChild( );
	}

	ui::End( );
}

void c_menu::draw_end( ) {
	if (!this->m_bInitialized)
		return;

	ui::EndFrame( );
	ui::Render( );
	ImGui_ImplDX9_RenderDrawData( ui::GetDrawData( ) );
}

bool c_menu::is_menu_initialized( ) {
	return this->m_bInitialized;
}

bool c_menu::is_menu_opened( ) {
	return this->m_bIsOpened;
}

void c_menu::set_menu_opened( bool v ) {
	this->m_bIsOpened = v;
}

IDirect3DTexture9* c_menu::get_texture_data( ) {
	return this->m_pTexture;
}

ImColor c_menu::get_accent_color( ) {
	return ImColor( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], ui::GetStyle( ).Alpha );
}