#include "misc.hpp"
#include <unordered_map>
#include "fnv1a.hpp"
#include "unity.hpp"

uintptr_t m_pBaseProjectile = 0;

std::unordered_map<std::string, Vector4> m_vecRecoilProperties;
uintptr_t m_pEffect;

void spoof_admin ( )
{
	if ( var::misc::admin && !GetAsyncKeyState ( var::misc::fly ) )
	{
		int flags = game::m_pLocalplayer->get_flags ( mist::misc.get ( ) );
		if ( flags == 0 )
			return;

		flags |= 4;
		game::m_pLocalplayer->set_flag ( mist::misc.get ( ), flags );
	}
}

void no_recoil ( )
{
	Item* m_pWeapon = game::m_pLocalplayer->get_held_item ( mist::misc.get ( ) ); VASSERT ( m_pWeapon );

	m_pBaseProjectile = m_pWeapon->get_base_projectile ( mist::misc.get ( ) );
	if ( !m_pBaseProjectile || !game::m_rLocalInfo.m_pWeapon->is_weapon ( mist::misc.get ( ) ) )
		return;

	uintptr_t m_pRecoilProperties = mist::misc->read<uintptr_t> ( m_pBaseProjectile + offsets::recoil_properties );
	if ( !m_pRecoilProperties )
		return;

	Vector4 m_vecRecoil = mist::misc->read<Vector4> ( m_pRecoilProperties + 0x18 );
	std::string m_hWeapon = m_pWeapon->get_name ( mist::misc.get ( ) );
	auto pair = std::make_pair ( m_hWeapon, m_vecRecoil );

	if ( m_vecRecoilProperties.find ( m_hWeapon ) == m_vecRecoilProperties.end ( ) )
		m_vecRecoilProperties.emplace ( m_hWeapon, m_vecRecoil );

	const Vector4 m_vecDefault = m_vecRecoilProperties[m_hWeapon];

	if ( m_pWeapon->is_weapon ( mist::misc.get ( ) ) && ( var::misc::recoil_yaw != 100.f && var::misc::recoil_pitch != 100.f ) )
		mist::misc->write<Vector4> ( m_pRecoilProperties + 0x18, { m_vecDefault.x / 100.f * var::misc::recoil_yaw, m_vecDefault.y / 100.f * var::misc::recoil_yaw, m_vecDefault.z / 100.f * var::misc::recoil_pitch, m_vecDefault.w / 100.f * var::misc::recoil_pitch } );
}

void movement ( )
{
	static bool do_once = false;
	uintptr_t movement = mist::misc->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::basemovement ); VASSERT ( movement );
	if ( movement && var::misc::spider )
		mist::misc->write<float> ( movement + offsets::groundangle_new, 0.f );

	if ( movement && var::misc::noslide )
		mist::misc->write<Vector2> ( movement + offsets::maxangle_walk, { 99999.f, 99999.f } ); // walk && climb

	if ( var::misc::bgravity && GetAsyncKeyState ( var::misc::gravity ) )
	{
		mist::misc->write<float> ( movement + offsets::gravity, 1.68f );
		do_once = true;
	}
	else if ( do_once )
	{
		mist::misc->write<float> ( movement + offsets::gravity, 2.5f );
		do_once = false;
	}

	if ( var::misc::omnisprint )
		game::m_rLocalInfo.m_pModelState->set_sprinting ( mist::misc.get ( ), true );
}

void fast ( )
{
	Item* m_pLastWeapon = 0;
	while ( true )
	{
		Item* m_pCurrentWeapon = game::m_pLocalplayer->get_held_item ( mist::wep.get ( ) ); CASSERT ( m_pCurrentWeapon );
		if ( m_pCurrentWeapon != m_pLastWeapon || m_pCurrentWeapon != game::m_rLocalInfo.m_pWeapon )
		{
			game::m_rLocalInfo.m_pWeapon = m_pCurrentWeapon;
			game::m_rLocalInfo.m_szWeaponName = m_pCurrentWeapon->get_name ( mist::wep.get ( ) );
			game::m_rLocalInfo.m_szBulletType = m_pCurrentWeapon->get_bullet_name ( mist::wep.get ( ) );
			game::m_rLocalInfo.m_hWeaponName = hash_constfnv64_string ( game::m_rLocalInfo.m_szWeaponName.c_str ( ) );
			game::m_rLocalInfo.m_hBulletType = hash_constfnv64_string ( game::m_rLocalInfo.m_szBulletType.c_str ( ) );
			game::m_rLocalInfo.m_vInfo.m_bPrediction = rust::use_weapon_prediction ( );
			game::m_rLocalInfo.m_vInfo.m_flGravity = rust::get_bullet_gravity ( );
			game::m_rLocalInfo.m_vInfo.m_flSpeed = rust::get_bullet_speed ( );

			m_pLastWeapon = m_pCurrentWeapon;
			m_pBaseProjectile = m_pCurrentWeapon->get_base_projectile ( mist::wep.get ( ) );
		}

		if ( !m_pBaseProjectile || !m_pCurrentWeapon->is_weapon ( mist::wep.get ( ) ) )
		{
			sleep_for ( milliseconds ( 300 ) );
			continue;
		}

		if ( game::m_rLocalInfo.m_szWeaponName == _ ( "pistol.eoka" ) && var::misc::eoka )
		{
			mist::wep->write<float> ( m_pBaseProjectile + 0x348, 1.f ); // flint->successfraction
			if ( GetAsyncKeyState ( VK_LBUTTON ) & 0x8000 )
			{
				mist::wep->write<uint8_t> ( m_pBaseProjectile + 0x358, 1 ); //_didSparkThisFrame
				mist::wep->write<uint8_t> ( m_pBaseProjectile + 0x359, 1 ); //_isStriking
			}
			else
				mist::wep->write<uint8_t> ( m_pBaseProjectile + 0x359, 0 ); //_isStriking
		}

		if ( game::m_rLocalInfo.m_szWeaponName == _ ( "bow.compound" ) && var::misc::compound )
			mist::wep->write<float> ( m_pBaseProjectile + 0x360, 0.f ); // compound->stringmaxduration

		if ( var::misc::nospread )
		{
			mist::wep->write<float> ( m_pBaseProjectile + 0x2D4, 0 );//hipAimCone
			mist::wep->write<float> ( m_pBaseProjectile + 0x2D0, 0 );//aimCone
			mist::wep->write<float> ( m_pBaseProjectile + 0x308, 0 );//aimconePenalty
			mist::wep->write<float> ( m_pBaseProjectile + 0x304, 0 );//stancePenalty
			mist::wep->write<float> ( m_pBaseProjectile + 0x2E8, 0 );//stancePenaltyScale
		}

		mist::wep->write<float> ( (uintptr_t)game::m_pLocalplayer + 0x5C8, var::misc::fakelag ? 0.4 : 0.05f );
		sleep_for ( milliseconds ( 150 ) );
	}
}

void atmosphere ( )
{/*
	while ( true )
	{
		uintptr_t m_pAdminSettings = mist::misc->read<uintptr_t> ( addr::gameassembly + offsets::admin_offset );
		uintptr_t m_pAdminInstance = mist::misc->read<uintptr_t> ( m_pAdminSettings + 0xB8 );
		if ( var::misc::alwaysday || var::misc::alwaysnight )
		{
			auto time = var::misc::alwaysday ? 12.f : 0.f;
			mist::other->write<float> ( mist::other->read<float> ( m_pAdminSettings ), time );
		}
		else
		{
			mist::other->write<float> ( mist::other->read<float> ( m_pAdminSettings), -1.f );

			sleep_for ( seconds ( 1 ) );
		}
		sleep_for ( milliseconds ( 500 ) );
	}*/
}

Vector3 m_vecEyes {};
uintptr_t m_pMovement = 0;
bool stop = false;
bool go_back = false, go_forward = false, go_up = false;

void picus1 ( )
{
	while ( !m_pMovement )
		sleep_for ( milliseconds ( 50 ) );

	while ( true )
	{
		if ( m_pMovement )

			if ( var::misc::bclimb && GetAsyncKeyState ( var::misc::assist ) ) {
				go_forward = true;
				sleep_for ( milliseconds ( 600 ) );
				go_forward = false; go_back = true;
				sleep_for ( milliseconds ( 65 ) );
				go_back = false; go_up = true;
				sleep_for ( milliseconds ( 500 ) );
				go_up = false;
			}

		sleep_for ( milliseconds ( 50 ) );
	}
}
bool nigger = false;

void flyhack ( )
{
	while ( true )
	{
		uintptr_t m_pLocalPlayer = (uintptr_t)game::m_pLocalplayer;
		if ( !m_pLocalPlayer )
		{
			sleep_for ( 400ms );
			continue;
		}

		uintptr_t m_pEyes = mist::misc->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + 0x600 );
		if ( !m_pEyes )
		{
			sleep_for ( 400ms );
			continue;
		}
		m_pMovement = mist::misc->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::basemovement );
		if ( !m_pMovement )
		{
			sleep_for ( 400ms );
			continue;
		}

		if ( ( var::misc::bfly && GetAsyncKeyState ( var::misc::fly ) ) || ( var::misc::bclimb && GetAsyncKeyState ( var::misc::assist ) ) )
		{
			//mist::misc->write<uint8_t> ( m_pMovement + 0x13C, 1 ); // flying BOOL
			ModelState* model_state = (ModelState*)mist::misc->read<uintptr_t> ( reinterpret_cast<uintptr_t>( game::m_pLocalplayer ) + offsets::modelState );
			mist::misc->write<float> ( (uintptr_t)model_state + 0x14, 1.f );
			mist::misc->write<float> ( m_pMovement + 0x64, -999.f ); // noclip ;)
			mist::misc->write<float> ( m_pMovement + 0x6C, -999.f );

			Vector3 targetmovement {};
			PlayerEyes* eyes = (PlayerEyes*)m_pEyes;

			Vector4 rotation = mist::misc->read<Vector4> ( m_pEyes + 0x44 );

			model_state->set_sprinting ( mist::misc.get ( ), true );

			if ( GetAsyncKeyState ( 'W' ) || go_forward )
				targetmovement += quatmult ( &vecforward, &rotation );
			if ( GetAsyncKeyState ( 'S' ) || go_back )
				targetmovement -= quatmult ( &vecforward, &rotation );
			if ( GetAsyncKeyState ( 'A' ) )
				targetmovement -= quatmult ( &vecright, &rotation );
			if ( GetAsyncKeyState ( 'D' ) )
				targetmovement += quatmult ( &vecright, &rotation );

			if ( GetAsyncKeyState ( VK_SPACE ) || go_up )
				targetmovement.y += 1.f;

			float d = 5.f;
			if ( GetAsyncKeyState ( VK_CONTROL ) )
				d = 2.5f;
			if ( GetAsyncKeyState ( VK_SHIFT ) )
				d = var::misc::noclip_speed;

			if ( !( targetmovement == Vector3 { 0.f, 0.f, 0.f } ) )
				mist::misc->write<Vector3> ( m_pMovement + offsets::target_movement, targetmovement * d );

			model_state->set_sprinting ( mist::misc.get ( ), true );

			nigger = true;
		}
		else
		{
			if ( nigger )
			{
				mist::misc->write<float> ( m_pMovement + 0x64, 1.f );
				mist::misc->write<float> ( m_pMovement + 0x6C, 0.55f );
				//mist::misc->write<float> ( m_pMovement + 0x74, 2.5f );
				nigger = false;
			}
		}
		mist::misc->write<bool> ( m_pMovement + 0x18, 0 );
		sleep_for ( 1ms );
	}
}

void slow ( )
{
	std::thread pica ( picus1 );
	pica.detach ( );

	bool pico = false;
	bool nigger = false;
	while ( true )
	{
		uintptr_t m_pTime = mist::misc->read<uintptr_t> ( addr::unityplayer + offsets::timescale + 7 * 8 );
		if ( !m_pTime )
		{
			sleep_for ( milliseconds ( 700 ) );
			continue;
		}

		m_pMovement = mist::misc->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::basemovement );
		if ( !m_pMovement )
		{
			sleep_for ( milliseconds ( 400 ) );
			continue;
		}

		if ( var::misc::fakelag )
			mist::misc->write<float> ( (uintptr_t)game::m_pLocalplayer + 0x5C8, 0.15f );
		else
			mist::misc->write<float> ( (uintptr_t)game::m_pLocalplayer + 0x5C8, 0.05f );

		if ( var::misc::btimescale && GetAsyncKeyState ( var::misc::timescale ) )
		{
			mist::misc->write<float> ( m_pTime + 0xFC, 3.f );
			mist::misc->write<float> ( (uintptr_t)game::m_pLocalplayer + 0x5C8, 0.05f );
			pico = true;
		}
		else if ( pico == true )
		{
			mist::misc->write<float> ( m_pTime + 0xFC, 1.f );

			pico = false;
		}
		uintptr_t m_pEyes = mist::misc->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + 0x600 );
		if ( var::misc::bview_offset && GetAsyncKeyState ( var::misc::view_offset ) )
			mist::misc->write<Vector3> ( m_pEyes + 0x38, { m_vecEyes.x + var::misc::x, m_vecEyes.y + var::misc::z, m_vecEyes.z + var::misc::y } );
		else
			m_vecEyes = mist::misc->read<Vector3> ( m_pEyes + 0x38 );

		if ( var::misc::fov != 70.f )
		{
			uintptr_t m_pGraphics = mist::misc->read<uintptr_t> ( addr::gameassembly + offsets::fov_offset );
			uintptr_t m_pGraphicsInstance = mist::misc->read<uintptr_t> ( m_pGraphics + 0xB8 );
			mist::misc->write<float> ( m_pGraphicsInstance + 0x18, var::misc::fov );
		}

		if ( var::misc::omnisprint )
			game::m_rLocalInfo.m_pModelState->set_sprinting ( mist::misc.get ( ), true );

		sleep_for ( milliseconds ( 5 ) );
	}
}

void experimental ( )
{
	uintptr_t m_pMetabolism = mist::other->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + 0x618 );
	uintptr_t m_pMetabolismAttributeRadiation = mist::other->read<uintptr_t> ( m_pMetabolism + 0x50 );
	mist::other->write<float> ( m_pMetabolismAttributeRadiation + 0x20, 0.f );
	//logging::print ( TYPE_DEBUG, "%f", m_pRadiationValue );
}

namespace keybinds {
	bool menu = true;
};

int misc::thread ( )
{
	uintptr_t m_pAmbientParams = mist::misc->read<uintptr_t> ( addr::tod_sky + 0x90 );
	mist::misc->write<float> ( m_pAmbientParams + 0x18, 5.f );

	std::thread nosleep ( fast );
	nosleep.detach ( );

	std::thread bigsleep ( slow );
	bigsleep.detach ( );

	std::thread atmo ( atmosphere );
	atmo.detach ( );

	std::thread fly ( flyhack );
	fly.detach ( );

	while ( true )
	{
		spoof_admin ( );
		movement ( );
		no_recoil ( );
		//experimental ( );

		sleep_for ( milliseconds ( 15 ) );
	}
	return 1;
}

void misc::keybindh ( )
{
	while ( true )
	{

		if ( GetAsyncKeyState ( VK_INSERT ) & 1 )
		{
			keybinds::menu = !keybinds::menu;
		}

		if ( GetAsyncKeyState ( VK_END ) & 1 )
		{
			mist::other.release ( );
			mist::aim.release ( );
			mist::esp.release ( );
			mist::misc.release ( );
			mist::obj.release ( );

			ExitProcess ( 0 );
		}

		sleep_for ( milliseconds ( 120 ) );
	}
}