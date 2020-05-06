#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::DrawModelExecute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix )
{
	static auto oDrawModelExecute = hooks.pModelHook->GetOriginal<DrawModelExecute_t>( vtable_indexes::dme );
	const char* ModelName = g_pModelInfo->GetModelName( (model_t*)info.pModel );
	C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity( info.index );




	oDrawModelExecute( ecx, context, state, info, matrix );
}