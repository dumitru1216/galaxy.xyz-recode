#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"

AntiAim g_AntiAim;

bool Swtich = false;

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