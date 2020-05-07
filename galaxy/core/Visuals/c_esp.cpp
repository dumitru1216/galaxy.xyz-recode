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

c_esp esp;

void c_esp::RenderBox()
{
	g_pSurface->OutlinedRect( Box.left, Box.top, Box.right, Box.bottom, Color(galaxy_vars.cfg.box_color[0] * 255, galaxy_vars.cfg.box_color[1] * 255, galaxy_vars.cfg.box_color[2] * 255, 240 )); //main part
	g_pSurface->OutlinedRect( Box.left + 1, Box.top + 1, Box.right - 2, Box.bottom - 2, Color(0, 0, 0, 240)); //outlkine
	g_pSurface->OutlinedRect( Box.left - 1, Box.top - 1, Box.right + 2, Box.bottom + 2, Color(0, 0, 0, 240)); //outline
}

void c_esp::RenderName(C_BaseEntity* pEnt, int iterator)
{
    PlayerInfo_t pInfo;
    g_pEngine->GetPlayerInfo(iterator, &pInfo);

	auto name_color = galaxy_vars.cfg.name_color;

	if (galaxy_vars.cfg.name)
		g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top - 9, Color(name_color[0] * 255, name_color[1] * 255, name_color[2] * 255, 240), font, true, pInfo.szName);
	
}

std::string clean_name( std::string name ) {
	std::string wep = name;
	if (wep[0] == 'W') wep.erase( wep.begin( ) );

	auto end_of_weapon = wep.find( "EAPON_" );
	if (end_of_weapon != std::string::npos)
		wep.erase( wep.begin( ) + end_of_weapon, wep.begin( ) + end_of_weapon + 6 );

	return wep;
}

void c_esp::RenderWeaponName(C_BaseEntity* pEnt)
{
    auto weapon = pEnt->GetActiveWeapon();
    if (!weapon) return;
     
	std::string wep_str = weapon->GetName( );

	std::transform( wep_str.begin( ), wep_str.end( ), wep_str.begin( ), ::toupper );

	char ammo[519];
	sprintf_s( ammo, " - [ %d  /  %d ]", weapon->GetAmmo( ), weapon->m_iPrimaryReserveAmmoCount( ) );

	std::string strWeaponName = std::string( weapon->GetCSWpnData( )->weapon_name ); //weapon->GetName();

    strWeaponName.erase(0, 7);

	if (galaxy_vars.cfg.weapon_name)
		g_pSurface->DrawT( Box.left + (Box.right / 2), Box.top + Box.bottom + 7, Color( 255, 255, 255, 240 ), font, true, (clean_name( wep_str ) + (galaxy_vars.cfg.weapon_name ? ammo : "")).c_str( ) );
}

void c_esp::RenderAmmo( C_BaseEntity* pEnt )
{
	if (!pEnt->GetActiveWeapon( )->GetCSWpnData_2( )) return;
	int
		max_in_clip = pEnt->GetActiveWeapon( )->GetCSWpnData_2( )->iMaxClip1,
		ammo_in_clip = pEnt->GetActiveWeapon( )->GetAmmo( ),
		scaled_ammo = ammo_in_clip * Box.right / max_in_clip,
		shit = std::clamp( scaled_ammo, 0, Box.right );

	auto ammo_color = galaxy_vars.cfg.ammo_color;

	g_pSurface->FilledRect( Box.left + 2, Box.top + Box.bottom + 3, Box.right - 5, 2, Color( 20, 20, 20, 240 ) );
	g_pSurface->OutlinedRect( Box.left + 1, Box.top + Box.bottom + 2, Box.right - 3, 4, Color( 35, 35, 35, 240 ) );
	g_pSurface->FilledRect( Box.left + 2, Box.top + Box.bottom + 3, shit - 5, 2, Color( ammo_color[0] * 255, ammo_color[1] * 255, ammo_color[2] * 255, 240 ) );
}
 //+ Box.right
void c_esp::RenderHealth( C_BaseEntity* pEnt )
{
	if (galaxy_vars.cfg.HealthBar)
	{
		//hp number
		std::string Health = "" + std::to_string( pEnt->GetHealth( ) );
		g_pSurface->DrawT( Box.left - 20, Box.top + (offsetY * 5), Color( 255, 255, 255, 255 ), font, false, Health.c_str( ) );
		offsetY += 1;



		g_pSurface->FilledRect( Box.left - 6, Box.top - 1, 4, Box.bottom + 2, Color( 0, 0, 0, 240 ) );
		int pixelValue = pEnt->GetHealth( ) * Box.bottom / 100;
		g_pSurface->FilledRect( Box.left - 5, Box.top + Box.bottom - pixelValue, 2, pixelValue, Color( 0, 255, 0, 250 ) );


	}
}

void c_esp::RenderSkeleton(C_BaseEntity* pEnt) // the best
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

void c_esp::BoundBox(C_BaseEntity* pEnt)
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

void c_esp::Render()
{
    if (!g::pLocalEntity || !g_pEngine->IsInGame() || !galaxy_vars.cfg.Esp)  return;
       
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

        if (galaxy_vars.cfg.name)
            RenderName(pPlayerEntity, i);

        if (galaxy_vars.cfg.weapon_name)
            RenderWeaponName(pPlayerEntity);

		if (galaxy_vars.cfg.ammo_bar)
			RenderAmmo( pPlayerEntity );

		if (galaxy_vars.cfg.HealthBar)
			RenderHealth(pPlayerEntity);
    }
}