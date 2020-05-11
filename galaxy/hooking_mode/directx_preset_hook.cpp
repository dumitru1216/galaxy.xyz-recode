#include "../c_hooks.h"
#include "../core/external_inclues.h"
#include "directx_font.h"
#include <intrin.h>

IDirect3DStateBlock9* m_pStateBlockDraw;
IDirect3DStateBlock9* m_pStateBlockText;
IDirect3DStateBlock9 *state_block;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
static bool initialized = false;

void __stdcall create_objects( IDirect3DDevice9* device )  //creating imgui objects
{
	if (hooks.hCSGOWindow)
		ImGui_ImplDX9_CreateDeviceObjects( );
}

void __stdcall invalidate_objects( ) //invalidate imgui objects
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}



void __stdcall pre_render( IDirect3DDevice9* device ) {
	D3DVIEWPORT9 d3d_viewport;
	device->GetViewport( &d3d_viewport );

	device->CreateStateBlock( D3DSBT_ALL, &state_block );
	state_block->Capture( );

	device->SetVertexShader( nullptr );
	device->SetPixelShader( nullptr );
	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	device->SetRenderState( D3DRS_LIGHTING, FALSE );
	device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, TRUE );

	device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
}

void __stdcall post_render( ) {
	ImGui_ImplDX9_NewFrame( );
}

void __stdcall c_hooks::Hooked_EndScene( IDirect3DDevice9* pDevice )
{
	static auto oEndScene = hooks.D3DHook->GetOriginal<EndSceneFn>( vtable_indexes::end_scene );

	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
	pDevice->CreateStateBlock( D3DSBT_PIXELSTATE, &pixel_state );
	pDevice->GetVertexDeclaration( &vertDec );
	pDevice->GetVertexShader( &vertShader );
	
	static auto wanted_ret_address = _ReturnAddress( );
	if (_ReturnAddress( ) == wanted_ret_address)
	{

		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState( D3DRS_COLORWRITEENABLE, &colorwrite );
		pDevice->GetRenderState( D3DRS_SRGBWRITEENABLE, &srgbwrite );
		pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
		pDevice->SetRenderState( D3DRS_SRGBWRITEENABLE, false );

		static bool once{ false };
		if (!once) {
			menu.initialize( pDevice );
			once = true;
		}

		menu.draw_begin( );

		int w, h = 0;
		g_pEngine->GetScreenSize( w, h );

		ui::SetNextWindowPos( ImVec2( 0, 0 ) );
		ui::SetNextWindowSize( ImVec2( w, h ) );
		ui::Begin( "##visuals", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );

		ui::End( );

		menu.draw( );
		menu.draw_end( );

		pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, colorwrite );
		pDevice->SetRenderState( D3DRS_SRGBWRITEENABLE, srgbwrite );
	}

	pixel_state->Apply( );
	pixel_state->Release( );
	pDevice->SetVertexDeclaration( vertDec );
	pDevice->SetVertexShader( vertShader );

	oEndScene( pDevice );
}

void __stdcall c_hooks::Hooked_EndScene_Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	static auto oEndSceneReset = hooks.D3DHook->GetOriginal<EndSceneResetFn>( vtable_indexes::end_scene_reset );

	if (!initialized)
		oEndSceneReset( pDevice, pPresentationParameters );

	invalidate_objects( );
	auto hr = oEndSceneReset;
	create_objects( pDevice );


	return oEndSceneReset( pDevice, pPresentationParameters );
}