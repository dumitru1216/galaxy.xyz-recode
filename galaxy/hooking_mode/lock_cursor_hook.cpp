#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::LockCursor( ISurface* thisptr, void* edx )
{
	static auto oLockCursor = hooks.pSurfaceHook->GetOriginal<LockCursor_t>( vtable_indexes::lockCursor );

	if (!menu.menuOpened)
	{
		g_pSurface->UnLockCursor( ); 
	}
		
	return oLockCursor( thisptr, edx );
}