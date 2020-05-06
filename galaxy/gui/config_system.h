#pragma once
#include <filesystem>

class c_config final
{
public:
	void run( ) noexcept;
	void load( size_t ) noexcept;
	void save( size_t ) const noexcept;
	void add( const char* ) noexcept;
	void remove( size_t ) noexcept;
	void rename( size_t, const char* ) noexcept;
	void reset( ) noexcept;

	constexpr auto& get_configs( ) noexcept {
		return configs;
	}

	struct
	{

		bool Aimbot;
		int Hitchance;
		int HitchanceValue;
		int Mindmg;
		bool Resolver;
		int BodyScale;
		int HeadScale;
		bool MultiPoint;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		bool FixShotPitch;
		bool PosBacktrack;
		bool ShotBacktrack;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;

		bool Antiaim;
		bool DesyncAngle;
		bool RandJitterInRange;
		int	JitterRange;
		int	Fakelag;
		bool FakeLagOnPeek;
		bool ChokeShotOnPeek;

		bool Esp;
		int Font;
		
		int Name;
		int HealthVal;
		int Weapon;
		bool Box;
		
		bool HealthBar;
		bool Skeleton[2] = { false,false };
		
		bool HitboxPoints;
		bool Chams;
		
		bool NoZoom;
		bool NoScope;
		bool NoRecoil;
		int Fov;
		bool Crosshair;

		bool Bhop;
		bool AutoStrafe;
		bool LegitBacktrack;
		bool Ak47meme;
		int	Test;

		float menu_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	} 
	cfg;



private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};
extern c_config galaxy_vars;