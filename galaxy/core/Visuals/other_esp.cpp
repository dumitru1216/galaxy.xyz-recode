#include "other_esp.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ISurface.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"
#include "../../valve_sdk/ICvar.h"
#include "../../c_hooks.h"

other_esp c_other_esp;

void other_esp::night_mode( )
{
	//starting a proper nighmode
	static bool OldNightmode;

	int OldNightmodeValue = galaxy_vars.cfg.NightModeSlider;

	float nightamount = galaxy_vars.cfg.NightModeSlider / 100.f;

	if (OldNightmode != galaxy_vars.cfg.NightMode || OldNightmodeValue != galaxy_vars.cfg.NightModeSlider)
	{
		ConVar* r_DrawSpecificStaticProp = g_pCvar->FindVar( "r_DrawSpecificStaticProp" );
		*(int*)((DWORD)& r_DrawSpecificStaticProp->fnChangeCallback + 0xC) = 0; // ew
		r_DrawSpecificStaticProp->SetValue( 0 );

		for (auto i = g_pMaterialSys->FirstMaterial( ); i != g_pMaterialSys->InvalidMaterial( ); i = g_pMaterialSys->NextMaterial( i ))
		{
			IMaterial* pMaterial = g_pMaterialSys->GetMaterial( i );

			if (!pMaterial)
				continue;

			if (strstr( pMaterial->GetTextureGroupName( ), "World" ) || strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ))
			{
				if (galaxy_vars.cfg.NightMode)
				{
					if (strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ))
						pMaterial->ColorModulate( nightamount, nightamount, nightamount );
					else
						pMaterial->ColorModulate( nightamount, nightamount, nightamount );
				}
				else
					pMaterial->ColorModulate( 1.0f, 1.0f, 1.0f );
			}

			if (strstr( pMaterial->GetTextureGroupName( ), "SkyBox" )) {
				if (galaxy_vars.cfg.NightMode)
				{
					auto sky_color = galaxy_vars.cfg.sky_box_color;

					pMaterial->ColorModulate( sky_color[0], sky_color[1], sky_color[2] );
				}
			}


			

		}

		OldNightmode = galaxy_vars.cfg.NightMode;
	}
}

void GradientH( int x, int y, int w, int h, Color c1, Color c2 )
{
	g_pSurface->FilledRect( x, y, w, h, c1 );
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		g_pSurface->FilledRect( x + i, y, 1, h, Color( first, second, third, ia ) );
	}
}

void other_esp::watermak( )
{
	int width, height;
	g_pEngine->GetScreenSize( width, height );

	g_pSurface->FilledRect( width - 145, 4, 117, 17, Color(0, 0, 0, 220) );
	GradientH( width - 145, 4, 58, 2, Color( 56, 56, 56, 255), Color( 255, 255, 255, 255 ) );
	GradientH( width - 88, 4, 59, 2, Color( 255, 255, 255, 255 ), Color( 56, 56, 56, 255 ) );
	g_pSurface->DrawT( width - 140, 7, Color(255, 255, 255, 255), g::watermark_font, false, "galaxy.xyz  |  beta  |  0.3" );
	
}