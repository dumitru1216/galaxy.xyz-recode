#include "../c_hooks.h"
#include "../core/external_inclues.h"

bool __fastcall c_hooks::CreateMove( IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd )
{
	// Call original createmove before we start screwing with it
	static auto oCreateMove = hooks.pClientModeHook->GetOriginal<CreateMove_t>( vtable_indexes::createMove );
	oCreateMove( thisptr, edx, sample_frametime, pCmd );

	if (!pCmd || !pCmd->command_number)
		return oCreateMove;

	// Get globals
	g::pCmd = pCmd;
	g::pLocalEntity = g_pEntityList->GetClientEntity( g_pEngine->GetLocalPlayer( ) );
	g::bSendPacket = true;
	if (!g::pLocalEntity) return oCreateMove;

	uintptr_t *framePtr;
	__asm mov framePtr, ebp;

	g::OriginalView = g::pCmd->viewangles;

	g_Misc.OnCreateMove( );
	g_Resolver.OnCreateMove( );
	engine_prediction::RunEnginePred( );
	g_AntiAim.OnCreateMove( );
	g_Aimbot.OnCreateMove( );
	g_Legitbot.OnCreateMove( );
	engine_prediction::EndEnginePred( );
	g_Misc.MovementFix( g::OriginalView );
	g_Math.Clamp( g::pCmd->viewangles );

	if (g::bSendPacket)
		g::RealAngle = g::pCmd->viewangles;

	*(bool*)(*framePtr - 0x1C) = g::bSendPacket;

	g::pCmd->buttons |= IN_BULLRUSH; // hehe

	return false;
}