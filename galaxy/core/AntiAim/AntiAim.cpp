#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"
#include <array>
#include "../../valve_sdk/ICvar.h"
#include "../Aimbot/LagComp.h"

AntiAim g_AntiAim;

bool Swtich = false;

bool m_should_update_fake = false;
std::array< AnimationLayer, 15 > m_fake_layers; // 13
std::array< float, 24 > m_fake_poses; // 20
CBasePlayerAnimState* m_fake_state = nullptr;
bool init_fake_anim = false;
float m_fake_spawntime = 0.f;
matrix3x4_t m_fake_position_matrix[128];


void AntiAim::desyncchams( )
{
	if (!g::pLocalEntity || !g::pLocalEntity->IsAlive( )) {
		m_should_update_fake = true;
		return;
	}

	if (m_fake_spawntime != g::pLocalEntity->m_flSpawnTime( ) || m_should_update_fake)
	{
		init_fake_anim = false;
		m_fake_spawntime = g::pLocalEntity->m_flSpawnTime( );
		m_should_update_fake = false;
	}

	if (!init_fake_anim)
	{
		m_fake_state = reinterpret_cast<CBasePlayerAnimState*> (g_pMemAlloc->Alloc( sizeof( CBasePlayerAnimState ) ));

		if (m_fake_state != nullptr)
			g::pLocalEntity->create_animation_state( m_fake_state );

		init_fake_anim = true;
	}

	if (g::bSendPacket)
	{
		int OldFrameCount = g_pGlobalVars->framecount;
		int OldTickCount = g_pGlobalVars->tickcount;
		static auto host_timescale = g_pCvar->FindVar( ("host_timescale") );

		g_pGlobalVars->framecount = TIME_TO_TICKS( g::pLocalEntity->GetSimulationTime( ) );
		g_pGlobalVars->tickcount = TIME_TO_TICKS( g::pLocalEntity->GetSimulationTime( ) );

		std::memcpy( m_fake_layers.data( ), g::pLocalEntity->GetAnimOverlays( ), sizeof( m_fake_layers ) );
		std::memcpy( m_fake_poses.data( ), g::pLocalEntity->m_flPoseParameter( ).data( ), sizeof( m_fake_poses ) );

		g::pLocalEntity->UpdateAnimationState( m_fake_state, g::pCmd->viewangles );

		const auto backup_absangles = g::pLocalEntity->GetAbsAngles( );

		/* invalidate bone cache */
		g::pLocalEntity->GetMostRecentModelBoneCounter( ) = 0;
		g::pLocalEntity->GetLastBoneSetupTime( ) = -FLT_MAX;

		g::m_got_fake_matrix = g::pLocalEntity->SetupBones( g::m_fake_matrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING & ~BONE_USED_BY_ATTACHMENT, g_pGlobalVars->curtime );

		memcpy( m_fake_position_matrix, g::m_fake_matrix, sizeof( m_fake_position_matrix ) );

		const auto org_tmp = g::pLocalEntity->GetRenderOrigin( );

		if (g::m_got_fake_matrix)
		{
			for (auto& i : g::m_fake_matrix)
			{
				i[0][3] -= org_tmp.x;
				i[1][3] -= org_tmp.y;
				i[2][3] -= org_tmp.z;
			}
		}

		g::pLocalEntity->SetAbsAngles( backup_absangles ); // restore real abs rotation

		/* invalidate bone cache */
		g::pLocalEntity->GetMostRecentModelBoneCounter( ) = 0;
		g::pLocalEntity->GetLastBoneSetupTime( ) = -FLT_MAX;

		std::memcpy( g::pLocalEntity->GetAnimOverlays( ), m_fake_layers.data( ), sizeof( m_fake_layers ) );
		std::memcpy( g::pLocalEntity->m_flPoseParameter( ).data( ), m_fake_poses.data( ), sizeof( m_fake_poses ) );

		g_pGlobalVars->framecount = OldFrameCount;
		g_pGlobalVars->tickcount = OldTickCount;
	}
}

void FreeStanding() // cancer v1
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y += 180;
	else
		g::pCmd->viewangles.y = FinalAngle;

	if (galaxy_vars.cfg.JitterRange != 0)
	{
		float Offset = galaxy_vars.cfg.JitterRange / 2.f;

		if (!galaxy_vars.cfg.RandJitterInRange)
		{
			Swtich ? g::pCmd->viewangles.y -= Offset : g::pCmd->viewangles.y += Offset;
		}
		else
		{
			static bool oldSwtich = Swtich;

			g::pCmd->viewangles.y -= Offset;

			static int Add = 0;

			if (oldSwtich != Swtich)
			{
				Add = rand() % galaxy_vars.cfg.JitterRange;
				oldSwtich = Swtich;
			}

			g::pCmd->viewangles.y += Add;
		}
	}
}

void Real()
{
	static bool Swtich2 = false;
	Swtich2 = !Swtich2;

	static float test = 0.f;
	if (Swtich2)
		test += 90.f;

	test = g_Math.NormalizeYaw(test);

	if (!g::bSendPacket && g::pLocalEntity->AnimState() && galaxy_vars.cfg.DesyncAngle)
		g::pCmd->viewangles.y = g_Math.NormalizeYaw(g::RealAngle.y + 90 + test);
	else
		FreeStanding();
}

void AntiAim::OnCreateMove()
{
	if (!g_pEngine->IsInGame() || !galaxy_vars.cfg.Antiaim || galaxy_vars.cfg.LegitBacktrack)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
		return;

	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (g::pLocalEntity->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime);

	if (canShoot && (g::pCmd->buttons & IN_ATTACK))
		return;

	if (g::bSendPacket)
		Swtich = !Swtich;

	g::pCmd->viewangles.x = 89.9f;

	Real();
}