#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::DoExtraBonesProcessing( void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6 )
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity || pPlayerEntity == nullptr) return;
	if (!pPlayerEntity->IsAlive( ) || pPlayerEntity->IsDormant( )) return;
	if (!pPlayerEntity->AnimState( )) return;

	auto oDoExtraBonesProcessing = hooks.pPlayerHook[pPlayerEntity->EntIndex( )]->GetOriginal<ExtraBoneProcess_t>( vtable_indexes::extraBonePro );

	float Backup = pPlayerEntity->AnimState( )->m_flUnknownFraction;
	pPlayerEntity->AnimState( )->m_flUnknownFraction = 0;
	oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );

	pPlayerEntity->AnimState( )->m_flUnknownFraction = Backup;
}