#include "c_esp.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ISurface.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"

ESP g_ESP;

void ESP::RenderBox()
{
	g_pSurface->OutlinedRect(Box.left, Box.top, Box.right, Box.bottom, color);
	g_pSurface->OutlinedRect(Box.left + 1, Box.top + 1, Box.right - 2, Box.bottom - 2, Color(0, 0, 0, 240));
	g_pSurface->OutlinedRect(Box.left - 1, Box.top - 1, Box.right + 2, Box.bottom + 2, Color(0, 0, 0, 240));
}

void ESP::RenderName(C_BaseEntity* pEnt, int iterator)
{
    PlayerInfo_t pInfo;
    g_pEngine->GetPlayerInfo(iterator, &pInfo);

	if (galaxy_vars.cfg.Name == 1)
		g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top - 16, textcolor, font, true, pInfo.szName);
	else if (galaxy_vars.cfg.Name == 2)
	{
		g_pSurface->DrawT(Box.left + Box.right + 5, Box.top, textcolor, font, false, pInfo.szName);
		offsetY += 1;
	}
}

void ESP::RenderWeaponName(C_BaseEntity* pEnt)
{
    auto weapon = pEnt->GetActiveWeapon();

    if (!weapon)
        return;

    auto strWeaponName = weapon->GetName();

    strWeaponName.erase(0, 7);

	if (galaxy_vars.cfg.Weapon == 1)
		g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top + Box.bottom, textcolor, font, true, strWeaponName.c_str());
	else if (galaxy_vars.cfg.Weapon == 2)
	{
		g_pSurface->DrawT(Box.left + Box.right + 5, Box.top + (offsetY * 11), textcolor, font, false, strWeaponName.c_str());
		offsetY += 1;
	}
}

void ESP::RenderHealth(C_BaseEntity* pEnt)
{
	if (galaxy_vars.cfg.HealthBar)
	{
		g_pSurface->FilledRect(Box.left - 6, Box.top - 1, 4, Box.bottom + 2, Color(0, 0, 0, 240));
		int pixelValue = pEnt->GetHealth() * Box.bottom / 100;
		g_pSurface->FilledRect(Box.left - 5, Box.top + Box.bottom - pixelValue, 2, pixelValue, Color(0, 255, 0, 250));
	}

	if (galaxy_vars.cfg.HealthVal == 1)
	{
		std::string Health = "HP " + std::to_string(pEnt->GetHealth());
		g_pSurface->DrawT(Box.left + Box.right + 5, Box.top + (offsetY * 11), textcolor, font, false, Health.c_str());
		offsetY += 1;
	}
}

void ESP::RenderHitboxPoints(C_BaseEntity* pEnt)
{
	for (int hitbox = 0; hitbox < 28; hitbox++)
	{
		Vector2D w2sHitbox;
		Utils::WorldToScreen(g::AimbotHitbox[pEnt->EntIndex()][hitbox], w2sHitbox);
		g_pSurface->FilledRect(w2sHitbox.x - 2, w2sHitbox.y - 2, 4, 4, color);
	}
}

void ESP::RenderSkeleton(C_BaseEntity* pEnt) // the best
{
	if (g_LagComp.PlayerRecord[pEnt->EntIndex()].size() == 0)
		return;
	int size = 0;

	if (galaxy_vars.cfg.Skeleton[0])
		size++;
	if (galaxy_vars.cfg.Skeleton[1])
		size++;

	for (int mode = 0; mode < size; mode++)
	{
		Vector Hitbox[19];
		Vector2D Hitboxw2s[19];

		int Record = 0;

		if (mode == 0 && galaxy_vars.cfg.Skeleton[0])
		{
			Record = g_LagComp.PlayerRecord[pEnt->EntIndex()].size() - 1;

			if (g_LagComp.ShotTick[pEnt->EntIndex()] != -1)
				Record = g_LagComp.ShotTick[pEnt->EntIndex()];
		}

		for (int hitbox = 0; hitbox < 19; hitbox++)
		{
			Hitbox[hitbox] = pEnt->GetHitboxPosition(hitbox, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(Record).Matrix);
			Utils::WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]);
		}

		//spine
		g_pSurface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, skelecolor);

		//right leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, skelecolor);

		//right arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, skelecolor);

		//left leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, skelecolor);

		//left arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, skelecolor);

		
	}
}

void ESP::BoundBox(C_BaseEntity* pEnt)
{
	Box.bottom = 0;
	Box.top = 0;
	Box.left = 0;
	Box.right = 0;

	Vector2D w2sBottom, w2sTop;

	Utils::WorldToScreen(pEnt->GetOrigin() - Vector(0, 0, 8), w2sBottom); 
	Utils::WorldToScreen(pEnt->GetHitboxPosition(0, g_Aimbot.Matrix[pEnt->EntIndex()]) + Vector(0, 0, 10), w2sTop);
	
	int Middle = w2sBottom.y - w2sTop.y;
	int Width = Middle / 3.f;

	Box.bottom = Middle;
	Box.top = w2sTop.y;
	Box.left = w2sBottom.x - Width;
	Box.right = Width * 2;
}

void ESP::Render()
{
    if (!g::pLocalEntity || !g_pEngine->IsInGame() || !galaxy_vars.cfg.Esp)
        return;

//	color = Color(galaxy_vars.cfg.BoxColor.red, galaxy_vars.cfg.BoxColor.green, galaxy_vars.cfg.BoxColor.blue, 240);
	//textcolor = Color(galaxy_vars.cfg.FontColor.red, galaxy_vars.cfg.FontColor.green, galaxy_vars.cfg.FontColor.blue, 240);
	//skelecolor = Color(galaxy_vars.cfg.SkeletonColor.red, galaxy_vars.cfg.SkeletonColor.green, galaxy_vars.cfg.SkeletonColor.blue, 240);

	(galaxy_vars.cfg.Font == 0) ? font = g::CourierNew : font = g::Tahoma;

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
    {
        C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

        if (!pPlayerEntity
            || !pPlayerEntity->IsAlive()
            || pPlayerEntity->IsDormant()
            || pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
            continue;

		offsetY = 0;

		BoundBox(pPlayerEntity);

		if (Box.bottom == 0)
			continue;

		if (galaxy_vars.cfg.Skeleton[0] || galaxy_vars.cfg.Skeleton[1])
			RenderSkeleton(pPlayerEntity);

        if (galaxy_vars.cfg.Box)
            RenderBox();

		if (galaxy_vars.cfg.HitboxPoints)
			RenderHitboxPoints(pPlayerEntity);

        if (galaxy_vars.cfg.Name > 0)
            RenderName(pPlayerEntity, i);

        if (galaxy_vars.cfg.Weapon)
            RenderWeaponName(pPlayerEntity);

		if (galaxy_vars.cfg.HealthBar || galaxy_vars.cfg.HealthVal > 0)
			RenderHealth(pPlayerEntity);
    }
/* i was working on exptrapolation 4ever ago i did in here because i just wanted to see it visualy
	if (g::pLocalEntity->IsAlive())
	{
		float angDelta;
		float angExtrap;
		Vector originExtrap;
		Vector2D angCalc, w2sBottom, w2sTop;

		static float oldSimtime;
		static float storedSimtime;

		static std::deque<TestPos> oldPos;

		if (storedSimtime != g::pLocalEntity->GetSimulationTime())
		{
			oldPos.push_back(TestPos{ g::pLocalEntity->GetOrigin(), g::pLocalEntity->GetSimulationTime() });
			oldSimtime = storedSimtime;
			storedSimtime = g::pLocalEntity->GetSimulationTime();
		}

		float simDelta = storedSimtime - oldSimtime;

		originExtrap = g::pLocalEntity->GetOrigin();

		Utils::WorldToScreen(g::pLocalEntity->GetOrigin() + (g::pLocalEntity->GetVelocity() * simDelta), w2sTop);
		g_pSurface->FilledRect(w2sTop.x - 3, w2sTop.y - 3, 6, 6, Color(255, 0, 0, 255));

		if (oldPos.size() > 3)
			oldPos.erase(oldPos.begin());

		if (oldPos.size() == 3)
		{
			angDelta = g_Math.CalcAngle(oldPos.at(1).Pos, oldPos.at(2).Pos).y - g_Math.CalcAngle(oldPos.at(0).Pos, oldPos.at(1).Pos).y;
			angExtrap = g_Math.CalcAngle(Vector(0,0,0), g::pLocalEntity->GetVelocity()).y + angDelta;
			angExtrap += 90;

			float tempExtrap = (g::pLocalEntity->GetVelocity().Length2D() * simDelta);

			angCalc = { (tempExtrap * sin(g_Math.GRD_TO_BOG(angExtrap))), (tempExtrap * cos(g_Math.GRD_TO_BOG(angExtrap))) };

			originExtrap = { originExtrap.x + angCalc.x, originExtrap.y - angCalc.y, g::pLocalEntity->GetVelocity().z * simDelta };			
		}

		if ((g::pLocalEntity->GetFlags() & FL_ONGROUND))
		{
			originExtrap.z = g::pLocalEntity->GetOrigin().x;
		}
		else
		{

		}

		Utils::WorldToScreen(originExtrap, w2sBottom);
		g_pSurface->FilledRect(w2sBottom.x - 3, w2sBottom.y - 3, 6, 6, Color(0, 255, 0, 255));
	}
	*/
}