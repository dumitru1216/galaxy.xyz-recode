#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "config_system.h"
#pragma comment(lib, "freetype.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_freetype.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>


class c_menu
{
public:
	bool menuOpened = false;
	void initialize( IDirect3DDevice9* pDevice );
	void draw_begin( );
	void draw( );
	void draw_end( );
	void draw_image( );

	bool is_menu_initialized( );
	bool is_menu_opened( );
	void set_menu_opened( bool v );

	IDirect3DTexture9* get_texture_data( );
	ImColor get_accent_color( );

private:
	bool m_bInitialized;
	bool m_bIsOpened;
	IDirect3DTexture9* m_pTexture;

	int m_nCurrentTab;

};
extern c_menu menu;