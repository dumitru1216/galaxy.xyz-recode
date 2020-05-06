#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "config_system.h"

class c_menu
{
public:
	bool menuOpened = false;
	void Render( );
	void apply_fonts( );

};
extern c_menu menu;