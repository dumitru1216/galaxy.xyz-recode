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

//get hand
bool isHand( std::string modelName ) {
	if ((modelName.find( "arms" ) != std::string::npos || modelName.find( "v_models" ) != std::string::npos) && !(modelName.find( "uid" ) != std::string::npos || modelName.find( "stattrack" ) != std::string::npos)) {
		return true;
	}

	return false;
}

static IMaterial* glowOverlay = nullptr;
static IMaterial* materialRegular = nullptr;
static IMaterial* rifk_glow = nullptr;
void __fastcall c_hooks::DrawModelExecute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix )
{
	static auto oDrawModelExecute = hooks.pModelHook->GetOriginal<DrawModelExecute_t>( vtable_indexes::dme );
	const char* ModelName = g_pModelInfo->GetModelName( (model_t*)info.pModel );
	C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity( info.index );

	//enemy struct
	static IMaterial* normal_enemy;
	static IMaterial* overide_enemy;

	//local struct
	static IMaterial* normal_local;
	static IMaterial* overide_local;

	glowOverlay = g_pMaterialSys->FindMaterial( "glowOverlay.vmt", TEXTURE_GROUP_OTHER );
	materialRegular = g_pMaterialSys->FindMaterial( "textured_virt", TEXTURE_GROUP_MODEL );
	rifk_glow = g_pMaterialSys->FindMaterial( "dev/glow_armsrace", TEXTURE_GROUP_OTHER );

	//enemy normal
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
		case 2:
			normal_enemy = rifk_glow;
			break;
		//---------------------------------
	}
	//enemy overide
	switch (galaxy_vars.cfg.overide_enemy_material)
	{
	case 0:
		overide_enemy = materialRegular;
		break;
		//---------------------------------
	case 1:
		overide_enemy = glowOverlay;
		break;
		//---------------------------------
	case 2:
		overide_enemy = rifk_glow;
		break;
		//---------------------------------
	}
	//local normal
	switch (galaxy_vars.cfg.local_materials)
	{
	case 0:
		normal_local = materialRegular;
		break;
		//---------------------------------
	case 1:
		normal_local = glowOverlay;
		break;
		//---------------------------------
	case 2:
		normal_local = rifk_glow;
		break;
		//---------------------------------
	}
	//local overide
	switch (galaxy_vars.cfg.local_overide_materials)
	{
	case 0:
		overide_local = materialRegular;
		break;
		//---------------------------------
	case 1:
		overide_local = glowOverlay;
		break;
		//---------------------------------
	case 2:
		overide_local = rifk_glow;
		break;
		//---------------------------------
	}

	//entity modelname
	if (pPlayerEntity && strstr( ModelName, "models/player" ))
	{
		if (info.index == g_pEngine->GetLocalPlayer( ) && strstr( ModelName, "models/player" ))
		{
			if (galaxy_vars.cfg.local_chams)
			{
				normal_local->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.local_color, normal_local );
				g_pModelRender->ForcedMaterialOverride( normal_local );
				oDrawModelExecute( ecx, context, state, info, matrix );

				if (galaxy_vars.cfg.local_chams_overide)
				{
					overide_local->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					modulate( galaxy_vars.cfg.local_overide_color, overide_local );
					g_pModelRender->ForcedMaterialOverride( overide_local );
					oDrawModelExecute( ecx, context, state, info, matrix );
				}
			}
		}

		if (pPlayerEntity != nullptr && pPlayerEntity != g::pLocalEntity && pPlayerEntity->GetTeam( ) != g::pLocalEntity->GetTeam( ) && strstr( ModelName, "models/player" ))
		{
			if (galaxy_vars.cfg.chams_enemy)
			{
				normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.enemy_visible_colors, normal_enemy );
				g_pModelRender->ForcedMaterialOverride( normal_enemy );
				oDrawModelExecute( ecx, context, state, info, matrix );	

				if (galaxy_vars.cfg.enemy_invisible)
				{
					normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					modulate( galaxy_vars.cfg.enemy_visible_colors, normal_enemy );
					g_pModelRender->ForcedMaterialOverride( normal_enemy );
					oDrawModelExecute( ecx, context, state, info, matrix );
				}

				if (galaxy_vars.cfg.enemy_overide_chams)
				{
					overide_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					overide_enemy->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					modulate( galaxy_vars.cfg.enemy_overide_colors, overide_enemy );
					g_pModelRender->ForcedMaterialOverride( overide_enemy );
					oDrawModelExecute( ecx, context, state, info, matrix );

				}
			}

			
		
			
		
		}

	}

	//other modelname
//	if (isHand)
//	{
//		if (galaxy_vars.cfg.hand_chams)
//		{

//		}
//	}


	
	oDrawModelExecute( ecx, context, state, info, matrix );
	g_pModelRender->ForcedMaterialOverride( nullptr );
}