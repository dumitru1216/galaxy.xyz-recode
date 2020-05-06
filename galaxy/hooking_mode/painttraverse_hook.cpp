#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	static auto oPaintTraverse = hooks.pPanelHook->GetOriginal<PaintTraverse_t>( vtable_indexes::paint );
	static unsigned int panelID, panelHudID;

	if (!panelHudID)
		if (!strcmp( "HudZoom", g_pPanel->GetName( vguiPanel ) ))
		{
			panelHudID = vguiPanel;
		}

	if (panelHudID == vguiPanel && g::pLocalEntity && g::pLocalEntity->IsAlive( ) && galaxy_vars.cfg.NoScope)
	{
		if (g::pLocalEntity->IsScoped( ))
			return;
	}
	oPaintTraverse( pPanels, vguiPanel, forceRepaint, allowForce );

	if (!panelID)
		if (!strcmp( "MatSystemTopPanel", g_pPanel->GetName( vguiPanel ) ))
		{
			panelID = vguiPanel;
			hooks.bInitializedDrawManager = true;
		}

	if (panelID == vguiPanel)
	{
		g_ESP.Render( );
		g_Misc.Crosshair( );

	}
}


