#include "../c_hooks.h"
#include "../core/external_inclues.h"

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT c_hooks::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN)
		if (wParam == VK_INSERT)
			menu.set_menu_opened( !menu.is_menu_opened( ) );

	if (menu.is_menu_opened( ))
	{
		ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam );

		if (wParam != 'W' && wParam != 'A' && wParam != 'S' && wParam != 'D' && wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_TAB && wParam != VK_SPACE || ui::GetIO( ).WantTextInput)
			return true;
	}


	return CallWindowProcA( hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam );
}
