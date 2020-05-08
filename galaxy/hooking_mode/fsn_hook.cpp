#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __stdcall c_hooks::FrameStageNotify( ClientFrameStage_t curStage )
{
	static auto oFrameStage = hooks.pClientHook->GetOriginal<FrameStageNotify_t>( vtable_indexes::frameStage );

	if (g_pEngine->IsInGame( ) && g_pEngine->IsConnected( ) && g::pLocalEntity)
	{
		//hooking features
		g_Misc.ThirdPerson( curStage );
		g_Resolver.FrameStage( curStage );

		//proper hooking nightmode
		c_other_esp.night_mode( );
	}

	oFrameStage( curStage );
}
