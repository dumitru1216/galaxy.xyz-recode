#include "../c_hooks.h"
#include "../core/external_inclues.h"

void modulate( const float* color, IMaterial* material )
{
	if (material)
	{
		material->AlphaModulate( color[3] );
		material->ColorModulate( color[0], color[1], color[2] );
		bool found_tint;
		const auto tint = material->FindVar( "$envmaptint", &found_tint );
		if (found_tint)
			tint->set_vector( Vector( color[0], color[1], color[2] ) );
	}

}

static IMaterial* glowOverlay = nullptr;
static IMaterial* materialRegular = nullptr;
void __fastcall c_hooks::DrawModelExecute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix )
{
	static auto oDrawModelExecute = hooks.pModelHook->GetOriginal<DrawModelExecute_t>( vtable_indexes::dme );
	const char* ModelName = g_pModelInfo->GetModelName( (model_t*)info.pModel );
	C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity( info.index );

	static IMaterial* normal_enemy;

	glowOverlay = g_pMaterialSys->FindMaterial( "glowOverlay.vmt", TEXTURE_GROUP_OTHER );
	materialRegular = g_pMaterialSys->FindMaterial( "textured_virt", TEXTURE_GROUP_MODEL );

	switch (galaxy_vars.cfg.normal_enemy_material)
	{
		case 0:
			normal_enemy = materialRegular;
			break;
		//---------------------------------
		case 1:
			normal_enemy = glowOverlay;
			break;
		//---------------------------------
	}


	if (pPlayerEntity && strstr( ModelName, "models/player" ))
	{

		if (pPlayerEntity != nullptr && pPlayerEntity != g::pLocalEntity && pPlayerEntity->GetTeam( ) != g::pLocalEntity->GetTeam( ) && strstr( ModelName, "models/player" ))
		{
			if (galaxy_vars.cfg.chams_enemy)
			{
				normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.enemy_visible_colors, normal_enemy );
				g_pModelRender->ForcedMaterialOverride( normal_enemy );
				oDrawModelExecute( ecx, context, state, info, matrix );				
			}
		
			
		
		}

	}

	
	oDrawModelExecute( ecx, context, state, info, matrix );
	g_pModelRender->ForcedMaterialOverride( nullptr );
}