#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __stdcall c_hooks::FrameStageNotify( ClientFrameStage_t curStage )
{
	static auto oFrameStage = hooks.pClientHook->GetOriginal<FrameStageNotify_t>( vtable_indexes::frameStage );

	//hooking features
	g_Misc.ThirdPerson( curStage );
	g_Resolver.FrameStage( curStage );

	oFrameStage( curStage );
}
