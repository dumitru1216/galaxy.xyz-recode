#include "menu_system.h"
#include "config_system.h"
#include "..\valve_sdk\Vector.h"
#include "..\valve_sdk\ISurface.h"
#include "..\valve_utils\Color.h"
#include "..\valve_utils\GlobalVars.h"
#include "..\valve_utils\XorStr.h"
#include "../c_hooks.h"
#include "../hooking_mode/directx_font.h"

c_menu menu;

ImFont* Main;
ImFont* MainCaps;

void c_menu::apply_fonts( )
{
	ImGui::CreateContext( );

	Main = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( SmallestPixel_compressed_data, SmallestPixel_compressed_size, 10 );
	MainCaps = ImGui::GetIO( ).Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuil.ttf", 24 );

}

void legit_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
	{
	



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void rage_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
	{
		



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
	{




	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void visuals_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "visuals [ players ]", ImVec2( 269, 201 ), true );
	{
	
		ImGui::Checkbox( "enable visuals", &galaxy_vars.cfg.Esp );
		ImGui::Checkbox( "bounding box", &galaxy_vars.cfg.Box );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###box_color", galaxy_vars.cfg.box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "name", &galaxy_vars.cfg.name );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###name_color", galaxy_vars.cfg.name_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "health bar", &galaxy_vars.cfg.HealthBar );
		ImGui::Checkbox( "weapon", &galaxy_vars.cfg.weapon_name );
		ImGui::Checkbox( "ammo bar", &galaxy_vars.cfg.ammo_bar );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###ammo_color", galaxy_vars.cfg.ammo_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "visuals [ chams ]", ImVec2( 269, 152 ), true );
	{
		static int chams_sub = 0;
		if (ImGui::Button( "enemy", ImVec2( 50, 15 ) ) )
		{
			chams_sub = 0;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "local", ImVec2( 50, 15 ) ))
		{
			chams_sub = 1;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "other", ImVec2( 50, 15 ) ))
		{
			chams_sub = 2;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "glow", ImVec2( 50, 15 ) ))
		{
			chams_sub = 3;
		}

		const char* chams_material[] = { "textured", "eso", "glow" };

		if (chams_sub == 0)
		{
			ImGui::Checkbox( "enemy chams", &galaxy_vars.cfg.chams_enemy );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###normal_chamsenem", galaxy_vars.cfg.enemy_visible_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Checkbox( "enemy chams invisible", &galaxy_vars.cfg.enemy_invisible );
			ImGui::Combo( "enemy materials [ normal ]", &galaxy_vars.cfg.normal_enemy_material, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "enemy chams overide", &galaxy_vars.cfg.enemy_overide_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###overidecolorenemy", galaxy_vars.cfg.enemy_overide_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "enemy materials [ overide ]", &galaxy_vars.cfg.overide_enemy_material, chams_material, IM_ARRAYSIZE( chams_material ) );


		}

		if (chams_sub == 1)
		{
			ImGui::Checkbox( "local chams", &galaxy_vars.cfg.local_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###noremal_locachams", galaxy_vars.cfg.local_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "local materials [ normal ]", &galaxy_vars.cfg.local_materials, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "local chams overide", &galaxy_vars.cfg.local_chams_overide );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###localcamsoveride", galaxy_vars.cfg.local_overide_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "local materials [ overide ]", &galaxy_vars.cfg.local_overide_materials, chams_material, IM_ARRAYSIZE( chams_material ) );

		}

		if (chams_sub == 2)
		{

		}

		if (chams_sub == 3)
		{

		}



	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "visuals [ other ]", ImVec2( 289, 152 ), true );
	{
		ImGui::Checkbox( "world modulation", &galaxy_vars.cfg.NightMode );
		ImGui::SliderInt( "nighmode scale", &galaxy_vars.cfg.NightModeSlider, 0, 100, "%.f%%" );
		ImGui::Text( "sky color" );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###skycolor", galaxy_vars.cfg.sky_box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );

	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "Effects", ImVec2( 289, 201 ), true );
	{
		
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void misc_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 361 ), true );
	{
	
	}
	ImGui::EndChild( true );
	ImGui::NextColumn( );
	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 2", ImVec2( 289, 152 ), true );
	{

	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 201 ), true );
	{

	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void config_tab( ) {

}

void c_menu::Render( )
{
	ImGui::GetIO( ).MouseDrawCursor = menuOpened;

	static int page = 0;

	if (menuOpened) {

		

		ImGui::GetStyle( ).Colors[ImGuiCol_CheckMark] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );
		ImGui::GetStyle( ).Colors[ImGuiCol_SliderGrab] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );
		ImGui::GetStyle( ).Colors[ImGuiCol_SliderGrabActive] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );

		ImGui::SetNextWindowSize( ImVec2( 600, 455 ), ImGuiSetCond_FirstUseEver );

		ImGui::Begin( "angeldust", &menuOpened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ); {
			ImVec2 p = ImGui::GetCursorScreenPos( );
			ImColor c = ImColor( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2] );

			ImGui::GetWindowDrawList( )->AddRectFilled( ImVec2( p.x, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + -3 ), ImColor( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2] ) );
			ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ) / 2, p.y + -3 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 125 ) );
			ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x + ImGui::GetWindowWidth( ) / 2, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + -3 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 15 ) );
			ImGui::GetWindowDrawList( )->AddLine( ImVec2( p.x, p.y + 2 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + 2 ), ImColor( 0, 0, 0, 150 ) );

		

			ImGui::PushFont( MainCaps );
			ImGui::Dummy( ImVec2( 250, 0 ) ); ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 7 );
			ImGui::TextColored( ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f ), "galaxy.xyz" );
			ImGui::PopFont( );

			ImGui::PushFont( Main );
			ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
			ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

			ImGui::BeginChild( "tabs", ImVec2( 568, 30 ), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );
			{
				if (ImGui::ButtonT( "ragebot", ImVec2( 95, -1 ), page, 0, c, false )) page = 0; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "legitbot", ImVec2( 95, -1 ), page, 1, c, false )) page = 1; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "visuals", ImVec2( 95, -1 ), page, 2, c, false )) page = 2; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "misc", ImVec2( 95, -1 ), page, 3, c, false )) page = 3; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "config & lua", ImVec2( 95, -1 ), page, 4, c, false )) page = 4; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "skins", ImVec2( 95, -1 ), page, 5, c, false )) page = 5; ImGui::SameLine( 0, 0 );
			}
			ImGui::EndChild( );
			ImGui::PopFont( );

			ImGui::PushFont( Main );

			switch (page) {
			case 0: legit_tab( );
				break;
			case 1: rage_tab( );
				break;
			case 2: visuals_tab( );
				break;
			case 3: misc_tab( );
				break;
			case 4: config_tab( );
				break;
			}

			ImGui::PopFont( );
		}
		ImGui::End( );
	}
}