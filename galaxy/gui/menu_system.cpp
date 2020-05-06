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


	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "Colored models", ImVec2( 269, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "Other ESP", ImVec2( 289, 152 ), true );
	{
		

		float old_x = ImGui::GetCursorPosX( );
		float old_y = ImGui::GetCursorPosY( );

		ImGui::SetCursorPosX( old_x + 18.0f );
		ImGui::SetCursorPosY( old_y - 3.0f );
		
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