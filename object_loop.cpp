#include "object_loop.hpp"
#include "include.hpp"
#include "sdk.hpp"
#include "baseplayer.hpp"
#include "cheat.hpp"

#define FERR_HASH_IMPL 1
#define HE(x, y) (x == y)
#include "fnv1a.hpp"
#include "overlay.hpp"

ULONG64 SteamID3To64 ( ULONG64 id )
{
	return ( ( 1ULL << 56 ) | ( 1ULL << 52 ) | ( 1ULL << 32 ) | id );
}

namespace addr {
	uintptr_t unityplayer;
	uintptr_t gameassembly;

	uintptr_t base_networkable;
	uintptr_t client_entities;
	uintptr_t entity_realm;
	uintptr_t buffer_list;
	uintptr_t object_list;
	uintptr_t camera_instance;
	uintptr_t tod_sky;
	std::size_t object_size;
};

namespace game {
	BasePlayer* m_pLocalplayer;
	rLocal m_rLocalInfo;
	entSet players;
	stringSet special;
	staticStringSet stashes, animals;
	staticStringColorSet items, collectibles, ores, vehicles, barrel, traps, bag;
	staticStringListColorSet cupboard;
	raidSet raid;
	PlayerIdName user_cache;
	std::unordered_map<ULONG, rCache> cache;
	std::unordered_set<ULONG> friendlist;
	std::mutex draw_mutex, aim_mutex;
};

void object::setup ( )
{

	addr::unityplayer = mist::obj->find ( h ( "UnityPlayer.dll" ) );
	addr::gameassembly = mist::obj->find ( h ( "GameAssembly.dll" ) );

	addr::base_networkable = rust::get_base_networkable ( mist::obj.get ( ) );
	addr::client_entities = rust::get_client_entities ( mist::obj.get ( ) );
	addr::entity_realm = rust::get_entity_realm ( mist::obj.get ( ) );
	addr::buffer_list = rust::get_buffer_list ( mist::obj.get ( ) );
	addr::object_list = rust::get_object_list ( mist::obj.get ( ) );
	addr::object_size = rust::get_object_size ( mist::obj.get ( ) );
	//

#ifdef _DEBUG
	if ( addr::gameassembly != 0 )
		logging::print ( TYPE_DEBUG, "UnityPlayer.dll: 0x%p", addr::unityplayer );
	else
		logging::print ( TYPE_ERROR, "Couldn't find UnityPlayer.dll!" );
	if ( addr::unityplayer != 0 )
		logging::print ( TYPE_DEBUG, "GameAssembly.dll: 0x%p", addr::gameassembly );
	else
		logging::print ( TYPE_ERROR, "Couldn't find GameAssembly.dll!" );

	logging::print ( TYPE_DEBUG, "Object list: 0x%p (%d)", addr::object_list, addr::object_size );
#endif
	std::thread game ( object::refresh_game );
	game.detach ( );
}

void object::refresh_game ( )
{

	uintptr_t gameobjectmanager = mist::obj->read<uintptr_t> ( addr::unityplayer + offsets::gom_offset );
	if ( !gameobjectmanager )
		return;

	while ( true )
	{

		addr::base_networkable = rust::get_base_networkable ( mist::obj.get ( ) );
		addr::client_entities = rust::get_client_entities ( mist::obj.get ( ) );
		addr::entity_realm = rust::get_entity_realm ( mist::obj.get ( ) );
		addr::buffer_list = rust::get_buffer_list ( mist::obj.get ( ) );
		addr::object_list = rust::get_object_list ( mist::obj.get ( ) );
		addr::object_size = rust::get_object_size ( mist::obj.get ( ) );

		uintptr_t tagged_objects = mist::obj->read<uintptr_t> ( gameobjectmanager + 0x8 );
		if ( !tagged_objects )
		{

			sleep_for ( 20ms );
			continue;
		}

		uintptr_t game_object = mist::obj->read<uintptr_t> ( tagged_objects + 0x10 );
		if ( !game_object )
		{

			sleep_for ( 20ms );
			continue;
		}

		uintptr_t object_class = mist::obj->read<uintptr_t> ( game_object + 0x30 );
		if ( !object_class )
		{

			sleep_for ( 20ms );
			continue;
		}

		addr::camera_instance = mist::obj->read<uintptr_t> ( object_class + 0x18 );

		if ( !addr::tod_sky )
		{
			uintptr_t m_pTODClass = rust::scan_for_class ( mist::obj.get ( ), _ ( "TOD_Sky" ) );
			uintptr_t m_pInstanceList = mist::obj->read<uintptr_t> ( m_pTODClass + 0xB8 );
			m_pInstanceList = mist::obj->read<uintptr_t> ( m_pInstanceList );
			uintptr_t m_pInstances = mist::obj->read<uintptr_t> ( m_pInstanceList + 0x10 );
			addr::tod_sky = mist::obj->read<uintptr_t> ( m_pInstances + 0x20 );
		}

		sleep_for ( seconds ( 4 ) );
	}
}

void raid_esp ( )
{
	uintptr_t m_pEffectClass = rust::scan_for_class ( mist::obj.get ( ), _ ( "EffectNetwork" ) );
	if ( !m_pEffectClass )
	{
		return;
	}

	while ( true )
	{
		if ( !var::esp::raid )
		{
			sleep_for ( milliseconds ( 800 ) );
			continue;
		}

		uintptr_t m_pEffect = mist::obj->read<uintptr_t> ( m_pEffectClass + 0xB8 );
		if ( !m_pEffect )
		{

			std::this_thread::sleep_for ( std::chrono::nanoseconds ( 500 ) );
			continue;
		}

		m_pEffect = mist::obj->read<uintptr_t> ( m_pEffect );
		if ( !m_pEffect )
		{

			std::this_thread::sleep_for ( std::chrono::nanoseconds ( 200 ) );
			continue;
		}

		Vector3 m_vecOrigin = mist::obj->read<Vector3> ( m_pEffect + 0x5C );
		if ( m_vecOrigin.empty ( ) )
		{

			std::this_thread::sleep_for ( std::chrono::nanoseconds ( 200 ) );
			continue;
		}

		std::string element_name = rust::get_class_name ( mist::obj.get ( ), m_pEffect );
		uintptr_t m_pName = mist::obj->read <uintptr_t> ( m_pEffect + 0x88 );
		std::wstring m_szPrefab = mist::obj->wread_unicode ( m_pName + 0x14, 128 );

		std::string m_szTranslated;

		if ( m_szPrefab.find ( _ ( L"explosion" ) ) == std::string::npos )
			continue;

		if ( m_szPrefab.find ( _ ( L"patrol" ) ) != std::string::npos )
			continue;

		if ( m_szPrefab.find ( _ ( L"c4" ) ) != std::string::npos )
			m_szTranslated = _ ( "C4" );

		else if ( m_szPrefab.find ( _ ( L"rocket" ) ) != std::string::npos )
			m_szTranslated = _ ( "Rocket" );

		else if ( m_szPrefab.find ( _ ( L"satchel" ) ) != std::string::npos )
			m_szTranslated = _ ( "Satchel" );
		else
			continue;

		auto m_tpVars = std::make_tuple ( m_pEffect, m_szTranslated, m_vecOrigin, std::chrono::steady_clock::now ( ) );
		game::raid.emplace_back ( m_tpVars );

		std::this_thread::sleep_for ( std::chrono::microseconds ( 1200 ) );
	}
}

void object::main ( )
{
	while ( addr::object_size == 0 )
		sleep_for ( milliseconds ( 50 ) );

	uintptr_t m_pTODClass = rust::scan_for_class ( mist::obj.get ( ), _ ( "TOD_Sky" ) );
	uintptr_t m_pInstanceList = mist::obj->read<uintptr_t> ( m_pTODClass + 0xB8 );
	m_pInstanceList = mist::obj->read<uintptr_t> ( m_pInstanceList );
	uintptr_t m_pInstances = mist::obj->read<uintptr_t> ( m_pInstanceList + 0x10 );
	addr::tod_sky = mist::obj->read<uintptr_t> ( m_pInstances + 0x20 );

	std::thread raid ( raid_esp );
	raid.detach ( );

	/* ORES */
	constexpr auto metal = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab" );
	constexpr auto metal2 = hashc_constfnv64_string ( "assets/bundled/prefabs/radtown/ore_metal.prefab" );
	constexpr auto metal3 = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab" );
	constexpr auto sulfur = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab" );
	constexpr auto sulfur2 = hashc_constfnv64_string ( "assets/bundled/prefabs/radtown/ore_sulfur.prefab" );
	constexpr auto sulfur3 = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab" );
	constexpr auto stone = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab" );
	constexpr auto stone2 = hashc_constfnv64_string ( "assets/bundled/prefabs/radtown/ore_stone.prefab" );
	constexpr auto stone3 = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab" );

	/* COLLECTABLES*/
	constexpr auto hemp = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab" );
	constexpr auto mushroom = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab" );
	constexpr auto pumpkin = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab" );
	constexpr auto corn = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab" );
	constexpr auto berry = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/berry-white/berry-white-collectable.prefab" );
	constexpr auto potato = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/potato/potato-collectable.prefab" );
	constexpr auto csulfur = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab" );
	constexpr auto cstone = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab" );
	constexpr auto wood = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab" );
	constexpr auto cmetal = hashc_constfnv64_string ( "assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab" );

	/* ITEM FILTER */
	constexpr auto waterjug = hashc_constfnv64_string ( "waterjug (world)" );
	constexpr auto waterbucket = hashc_constfnv64_string ( "bucket.water (world)" );
	constexpr auto arrow = hashc_constfnv64_string ( "arrow.wooden (world)" );
	constexpr auto nail = hashc_constfnv64_string ( "ammo.nailgun.nails (world)" );

	/* CLASSES */
	constexpr auto item_hash = hashc_constfnv64_string ( "DroppedItem" );
	constexpr auto corpse_hash = hashc_constfnv64_string ( "PlayerCorpse" );
	constexpr auto ore_hash = hashc_constfnv64_string ( "OreResourceEntity" );
	constexpr auto heli_hash = hashc_constfnv64_string ( "BaseHelicopter" );
	constexpr auto bradley_hash = hashc_constfnv64_string ( "BradleyAPC" );
	constexpr auto container_hash = hashc_constfnv64_string ( "LootContainer" );
	constexpr auto collectible_hash = hashc_constfnv64_string ( "CollectibleEntity" );
	constexpr auto stash_hash = hashc_constfnv64_string ( "StashContainer" );
	//player entities
	constexpr auto player_hash = hashc_constfnv64_string ( "BasePlayer" );
	constexpr auto sc1 = hashc_constfnv64_string ( "Scientist" );
	constexpr auto sc2 = hashc_constfnv64_string ( "ScientistNPC" );
	constexpr auto sc3 = hashc_constfnv64_string ( "HTNAnimal" );
	constexpr auto sc4 = hashc_constfnv64_string ( "HumanNPC" );
	constexpr auto sc5 = hashc_constfnv64_string ( "VehicleVendor" );
	constexpr auto sc6 = hashc_constfnv64_string ( "NPCShopKeeper" );

	//animals
	constexpr auto bear_hash = hashc_constfnv64_string ( "Bear" );
	constexpr auto boar_hash = hashc_constfnv64_string ( "Boar" );
	constexpr auto chicken_hash = hashc_constfnv64_string ( "Chicken" );
	constexpr auto horse_hash = hashc_constfnv64_string ( "Horse" );
	constexpr auto stag_hash = hashc_constfnv64_string ( "Stag" );
	constexpr auto wolf_hash = hashc_constfnv64_string ( "Wolf" );
	constexpr auto zombie_hash = hashc_constfnv64_string ( "Zombie" );
	//vehicles
	constexpr auto minicopter_hash = hashc_constfnv64_string ( "MiniCopter" );
	constexpr auto scrapheli_hash = hashc_constfnv64_string ( "ScrapTransportHelicopter" );
	constexpr auto boat = hashc_constfnv64_string ( "MotorRowboat" );
	constexpr auto rhib = hashc_constfnv64_string ( "RHIB" );
	constexpr auto horse = hashc_constfnv64_string ( "RidableHorse" );
	//traps
	constexpr auto turret_hash = hashc_constfnv64_string ( "AutoTurret" );
	constexpr auto landmine_hash = hashc_constfnv64_string ( "Landmine" );
	constexpr auto beartrap_hash = hashc_constfnv64_string ( "BearTrap" );
	constexpr auto guntrap_hash = hashc_constfnv64_string ( "GunTrap" );
	constexpr auto flameturret_hash = hashc_constfnv64_string ( "FlameTurret" );
	constexpr auto samsite_hash = hashc_constfnv64_string ( "SamSite" );
	constexpr auto toolcupboard_hash = hashc_constfnv64_string ( "BuildingPrivlidge" );
	constexpr auto sleepingbag_hash = hashc_constfnv64_string ( "SleepingBag" );

	while ( true )
	{
		game::entSet players;
		game::stringSet special;
		game::staticStringSet stashes, animals;
		game::staticStringColorSet items, collectibles, ores, vehicles, barrel, traps, sleepingbag;
		game::staticStringListColorSet cupboard;

		addr::object_list = rust::get_object_list ( mist::obj.get ( ) );
		addr::object_size = rust::get_object_size ( mist::obj.get ( ) );

		for ( uint64_t i = 0x0; i < addr::object_size; i++ )
		{
			uintptr_t element = mist::obj->read<uintptr_t> ( addr::object_list + 0x20 + ( i * 0x08 ) );
			if ( !element )
				continue;

			std::string element_name = rust::get_class_name ( mist::obj.get ( ), element );
			uint64_t element_hash = hash_constfnv64_string ( element_name.c_str ( ) );
			if ( element_hash != item_hash &&
				element_hash != corpse_hash &&
				element_hash != ore_hash &&
				element_hash != heli_hash &&
				element_hash != bradley_hash &&
				element_hash != container_hash &&
				element_hash != collectible_hash &&
				element_hash != stash_hash &&
				element_hash != bear_hash &&
				element_hash != boar_hash &&
				element_hash != chicken_hash &&
				element_hash != horse_hash &&
				element_hash != stag_hash &&
				element_hash != wolf_hash &&
				element_hash != zombie_hash &&
				element_hash != player_hash &&
				element_hash != sc1 &&
				element_hash != sc2 &&
				element_hash != sc3 &&
				element_hash != sc4 &&
				element_hash != sc5 &&
				element_hash != sc6 &&
				element_hash != minicopter_hash &&
				element_hash != scrapheli_hash &&
				element_hash != boat &&
				element_hash != rhib &&
				element_hash != horse &&
				element_hash != turret_hash &&
				element_hash != landmine_hash &&
				element_hash != beartrap_hash &&
				element_hash != guntrap_hash &&
				element_hash != flameturret_hash &&
				element_hash != samsite_hash &&
				element_hash != toolcupboard_hash &&
				element_hash != sleepingbag_hash
				)

				continue;

			uintptr_t base_mono_object = mist::obj->read<uintptr_t> ( element + 0x10 );
			if ( !base_mono_object )
				continue;

			uintptr_t object = mist::obj->read<uintptr_t> ( base_mono_object + 0x30 );
			if ( !object )
				continue;
			uintptr_t objectClass = mist::obj->read<uintptr_t> ( object + 0x30 );
			if ( !objectClass )
				continue;

			USHORT tag = mist::obj->read<USHORT> ( object + 0x54 );

			if ( tag == 6 )
			{
				BasePlayer* player = (BasePlayer*)rust::get_base_entity ( mist::obj.get ( ), objectClass );
				//player->mist_override ( mist::obj.get ( ) );
				if ( !player )
					continue;

				if ( i == 0x0 )
				{
					game::m_rLocalInfo.m_pPlayer = objectClass;
					game::m_pLocalplayer = player;

					Item* weapon = game::m_pLocalplayer->get_held_item ( mist::obj.get ( ) );

					if ( game::m_rLocalInfo.m_pWeapon != weapon )
					{
						game::m_rLocalInfo.m_pWeapon = weapon;
						game::m_rLocalInfo.m_pModelState = (ModelState*)mist::obj->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::modelState );
						game::m_rLocalInfo.m_szWeaponName = weapon->get_nice_name ( mist::obj.get ( ) );
						game::m_rLocalInfo.m_szBulletType = weapon->get_bullet_name ( mist::obj.get ( ) );
						game::m_rLocalInfo.m_hWeaponName = hash_constfnv64_string ( game::m_rLocalInfo.m_szWeaponName.c_str ( ) );
						game::m_rLocalInfo.m_hBulletType = hash_constfnv64_string ( game::m_rLocalInfo.m_szBulletType.c_str ( ) );
						game::m_rLocalInfo.m_vecLocal = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
						game::m_rLocalInfo.m_vInfo.m_bPrediction = rust::use_weapon_prediction ( );
						game::m_rLocalInfo.m_vInfo.m_flGravity = rust::get_bullet_gravity ( );
						game::m_rLocalInfo.m_vInfo.m_flSpeed = rust::get_bullet_speed ( );
					}
				}
				else
				{
					ULONG uid = player->get_uid ( mist::obj.get ( ) );

					if ( game::cache.find ( uid ) == game::cache.end ( ) )
					{
						Item* weapon = player->get_held_item ( mist::obj.get ( ) );
						rCache cache;
						cache.m_szName = player->get_name ( mist::obj.get ( ) );
						cache.m_szWeaponName = weapon ? weapon->get_nice_name ( mist::obj.get ( ) ) : "";
						cache.m_pWeapon = weapon;
						cache.m_pLastWeapon = weapon;
						cache.player_model = mist::obj->read<uintptr_t> ( (uintptr_t)player + offsets::model );
						cache.transform = mist::obj->read<uintptr_t> ( cache.player_model + 0x48 );
						cache.m_bWhitelist = false;
						cache.m_bFriend = player->is_teammate ( mist::obj.get ( ) );
						cache.m_bSleeper = player->is_sleeping ( mist::obj.get ( ) );
						if ( game::friendlist.find ( uid ) != game::friendlist.end ( ) )
							cache.m_bFriend = true;

						if ( ( element_name.length ( ) > 9 && ( element_name[9] == 'N' ) || ( element_name[0] == 'S' && element_name[1] == 'c' ) ) || ( element_name[0] == 'H' && element_name[1] == 'T' ) || ( element_name[0] == 'N' && element_name[3] == 'S' ) || ( element_name[0] == 'V' && element_name[7] == 'V' ) ) // Scientist
							cache.m_szName = _ ( "SCIENTIST" );
						if ( game::user_cache.find ( SteamID3To64 ( uid ) ) == game::user_cache.end ( ) )
							game::user_cache.emplace ( SteamID3To64 ( uid ), cache.m_szName );

						game::cache.emplace ( uid, cache );
					}
					Item* weapon = player->get_held_item ( mist::obj.get ( ) );
					game::cache[uid].m_bFriend = player->is_teammate ( mist::obj.get ( ) );
					game::cache[uid].m_pWeapon = weapon;
					game::cache[uid].m_szWeaponName = weapon->get_nice_name ( mist::obj.get ( ) );

					game::cache[uid].player_model = mist::obj->read<uintptr_t> ( (uintptr_t)player + offsets::model );
					game::cache[uid].transform = mist::obj->read<uintptr_t> ( game::cache[uid].player_model + 0x48 );
					game::cache[uid].m_bSleeper = player->is_sleeping ( mist::obj.get ( ) );

					auto pair = std::make_pair ( objectClass, game::cache[uid].m_szName == _ ( "SCIENTIST" ) ? entity_type::Scientist : entity_type::BasePlayer );

					players.emplace_back ( pair );
				}
			}
			else if ( var::esp::item && ( element_hash == item_hash || element_hash == corpse_hash ) ) // DroppedItem PlayerCorpse
			{
				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );

				if ( object_hash == waterjug || object_hash == waterbucket )
					continue;

				std::string m_szItemName = object_name;

				if ( object_hash == arrow || object_hash == nail )
					continue;

				constexpr auto corpse = hashc_constfnv64_string ( "assets/prefabs/player/player_corpse.prefab" );

				std::string_view str1 = _ ( " (world)" );
				std::string::size_type i = m_szItemName.find ( str1 );
				if ( i != std::string::npos )
					m_szItemName.erase ( i, str1.length ( ) );
				if ( object_hash == corpse )
					m_szItemName = _ ( "corpse" );
				else if ( m_szItemName.find ( _ ( "backpack" ) ) != std::string::npos )
					m_szItemName = _ ( "backpack" );
				else if ( m_szItemName.find ( _ ( "itemdrop" ) ) != std::string::npos )
					m_szItemName = _ ( "inventory sash" );

				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				ImColor clr = { colors::dropped::other[0], colors::dropped::other[1], colors::dropped::other[2] };
				if ( ( m_szItemName[0] == 'r' && m_szItemName[1] == 'i' ) ||
					( m_szItemName[0] == 's' && m_szItemName[2] == 'm'  && m_szItemName[2] == 'g' ) ||
					( m_szItemName[0] == 'p' && m_szItemName[5] == 'l' ) ||
					( m_szItemName[0] == 'b' && m_szItemName[2] == 'w' ) ||
					( m_szItemName[0] == 'l' && m_szItemName[1] == 'm' ) )
				{
					if ( !var::esp::item_weapon )
						continue;

					clr = { colors::dropped::weapon[0], colors::dropped::weapon[1], colors::dropped::weapon[2] };
				}

				else if ( m_szItemName.length ( ) > 8 &&
					( ( m_szItemName[0] == 'r' && m_szItemName[7] == 'l' ) ||
						( m_szItemName[0] == 'c' && m_szItemName[7] == 'w' ) || // crossbow
						( m_szItemName[0] == 's' && m_szItemName[4] == 'g' ) ) )
				{
					if ( !var::esp::item_weapon )
						continue;

					clr = { colors::dropped::weapon[0], colors::dropped::weapon[1], colors::dropped::weapon[2] };
				}
				else if ( ( m_szItemName[0] == 'c' && m_szItemName[2] == 'r' && m_szItemName[4] == 's' ) ||
					( m_szItemName[0] == 'b' && m_szItemName[3] == 'k' && m_szItemName[4] == 'p' ) )
				{
					if ( !var::esp::item_corpse )
						continue;

					clr = { colors::dropped::corpse[0], colors::dropped::corpse[1], colors::dropped::corpse[2] };
				}
				else if ( !var::esp::item_other )
					continue;

				auto pair = std::make_tuple ( objectClass, m_szItemName, position, clr );

				items.emplace_back ( pair );
			}
			else if ( var::esp::ores && ( element_hash == ore_hash ) ) // OreResourceEntity
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );


				std::string translated_name = "";
				ImColor clr = { colors::ore::stone[0], colors::ore::stone[1], colors::ore::stone[2] };

				if ( var::esp::metal && ( object_hash == metal || object_hash == metal2 || object_hash == metal3 ) )
				{
					clr = { colors::ore::metal[0], colors::ore::metal[1], colors::ore::metal[2] };
					translated_name = _ ( "METAL ORE" );
				}
				else if ( var::esp::sulfur && ( object_hash == sulfur || object_hash == sulfur2 || object_hash == sulfur3 ) )
				{
					clr = { colors::ore::sulfur[0], colors::ore::sulfur[1], colors::ore::sulfur[2] };

					translated_name = _ ( "SULFUR ORE" );
				}
				else if ( var::esp::stone && ( object_hash == stone || object_hash == stone2 || object_hash == stone3 ) )
				{
					translated_name = _ ( "STONE ORE" );
				}
				else
					continue;

				auto pair = std::make_tuple ( objectClass, translated_name, position, clr );

				ores.emplace_back ( pair );
			} // BaseHelicopter
			else if ( var::esp::special && ( element_hash == heli_hash || element_hash == bradley_hash ) ) // BaseHelicopter Bradley
			{
				auto pair = std::make_pair ( objectClass, element_name );
				special.emplace_back ( pair );
			}
			else if ( var::esp::radtown && element_hash == container_hash ) // LootContainer
			{
				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				if ( object_name[23] == 'r' )
				{
					ImColor clr;
					std::string translated_name = "";
					if ( object_name[31] == 'l' && object_name[36] == 'b' )
					{
						if ( !var::esp::barrel )
							continue;
						translated_name = _ ( "barrel" );
						clr = { colors::radtown::barrel[0], colors::radtown::barrel[1], colors::radtown::barrel[2] };
					}
					else if ( object_name[31] == 'c' )
					{
						if ( object_name[37] == 'e' )
						{
							if ( !var::esp::ecrate )
								continue;
							translated_name = _ ( "elite crate" );
							clr = { colors::radtown::ecrate[0], colors::radtown::ecrate[1], colors::radtown::ecrate[2] };
						}
						else if ( object_name[44] == '2' || object_name[37] == 'm' )
						{
							if ( !var::esp::crate )
								continue;
							translated_name = _ ( "crate" );
							clr = { colors::radtown::crate[0], colors::radtown::crate[1], colors::radtown::crate[2] };
						}
						else if ( object_name[37] == 't' )
						{
							if ( !var::esp::toolbox )
								continue;
							translated_name = _ ( "toolbox" );
							clr = { colors::radtown::toolbox[0], colors::radtown::toolbox[1], colors::radtown::toolbox[2] };
						}
						else
						{
							if ( !var::esp::mcrate )
								continue;
							translated_name = _ ( "military crate" );
							clr = { colors::radtown::mcrate[0], colors::radtown::mcrate[1], colors::radtown::mcrate[2] };
						}
					}
					else if ( object_name[31] == 'o' )
					{
						if ( !var::esp::oil )
							continue;
						translated_name = _ ( "oil" );
						clr = { colors::radtown::oil[0], colors::radtown::oil[1], colors::radtown::oil[2] };
					}
					else if ( object_name[31] == 'm' )
					{
						if ( !var::esp::minecart )
							continue;
						translated_name = _ ( "minecart" );
						clr = { colors::radtown::minecart[0], colors::radtown::minecart[1], colors::radtown::minecart[2] };
					}
					else
						continue;

					Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
					if ( position.empty ( ) )
						continue;

					float m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );
					if ( m_flDistance > 180.f )
						continue;

					if ( m_flDistance > var::esp::distance_limit )
						continue;

					auto pair = std::make_tuple ( objectClass, translated_name, position, clr );
					barrel.emplace_back ( pair );
				}
				continue;
			}
			else if ( var::esp::animals && ( ( element_hash == bear_hash ) || ( element_hash == boar_hash ) || ( element_hash == chicken_hash ) || ( element_hash == horse_hash ) || ( element_hash == stag_hash ) || ( element_hash == wolf_hash ) ) )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				const auto pair = std::make_tuple ( objectClass, element_name, position );
				animals.emplace_back ( pair );
			}
			else if ( var::esp::collectable && element_hash == collectible_hash ) // CollectibleEntity
			{
				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				std::string translated_name;
				if ( object_name.length ( ) > 50 && ( object_name[23] == 'a' && object_name[33] == 'c' ) )
				{
					const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );

					if ( object_hash == hemp ) // assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab
						translated_name = _ ( "hemp" );
					else if ( object_hash == mushroom ) // assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab
						translated_name = _ ( "mushroom" );
					else if ( object_hash == pumpkin ) // assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab
						translated_name = _ ( "pumpkin" );
					else if ( object_hash == corn ) // assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab
						translated_name = _ ( "corn" );
					else if ( object_hash == berry ) // assets/bundled/prefabs/autospawn/collectable/berry-white/berry-white-collectable.prefab
						translated_name = _ ( "berry" );
					else if ( object_hash == potato ) // assets/bundled/prefabs/autospawn/collectable/potato/potato-collectable.prefab
						translated_name = _ ( "potato" );

					else if ( object_hash == csulfur )
						translated_name = _ ( "sulfur" );
					else if ( object_hash == cstone )
						translated_name = _ ( "stone" );
					else if ( object_hash == wood )
						translated_name = _ ( "wood" );
					else if ( object_hash == cmetal )
						translated_name = _ ( "metal" );
					else
						continue;
				}
				else
					continue;

				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				ImColor collColor = ImColors::SlateBlue;

				if ( translated_name[0] == 'h' && var::esp::hemp ) // hemp
					collColor = { colors::collectable::hemp[0], colors::collectable::hemp[1], colors::collectable::hemp[2] };
				else if ( translated_name[0] == 'm' && var::esp::mushroom ) // mushroom
					collColor = { colors::collectable::mushroom[0], colors::collectable::mushroom[1], colors::collectable::mushroom[2] };
				else if ( translated_name[0] == 'c' && var::esp::corn ) //  corn
					collColor = { colors::collectable::corn[0], colors::collectable::corn[1], colors::collectable::corn[2] };
				else if ( translated_name[0] == 'b' && var::esp::berry ) // berry
					collColor = { colors::collectable::berry[0], colors::collectable::berry[1], colors::collectable::berry[2] };
				else if ( translated_name[2] == 't' && var::esp::potato ) // potato
					collColor = { colors::collectable::potato[0], colors::collectable::potato[1], colors::collectable::potato[2] };
				else if ( translated_name[3] == 'f' && var::esp::csulfur ) // sulfur
					collColor = { colors::collectable::sulfur[0], colors::collectable::sulfur[1], colors::collectable::sulfur[2] };
				else if ( translated_name[1] == 't' && var::esp::cstone ) // stone
					collColor = { colors::collectable::stone[0], colors::collectable::stone[1], colors::collectable::stone[2] };
				else if ( translated_name[4] == 'l' && var::esp::cmetal ) // metal
					collColor = { colors::collectable::metal[0], colors::collectable::metal[1], colors::collectable::metal[2] };
				else if ( translated_name[0] == 'w' && var::esp::cwood )
					collColor = { colors::collectable::wood[0], colors::collectable::wood[1], colors::collectable::wood[2] };
				else
					continue;

				float m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );
				if ( m_flDistance > 250.f )
					continue;

				if ( m_flDistance > var::esp::distance_limit )
					continue;

				const auto pair = std::make_tuple ( objectClass, translated_name, position, collColor );
				collectibles.emplace_back ( pair );
			}
			else if ( var::esp::stash && element_hash == stash_hash )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				const auto pair = std::make_tuple ( objectClass, _ ( "stash" ), position );

				stashes.emplace_back ( pair );
			}
			/*
				element_hash != minicopter_hash &&
				element_hash != scrapheli_hash &&
				element_hash != boat &&
				element_hash != rhib &&
				element_hash != horse
			*/
			else if ( var::esp::vehicles && ( element_hash == minicopter_hash || element_hash == scrapheli_hash || element_hash == boat || element_hash == rhib || element_hash == horse ) )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );

				std::string translated_name = "";
				ImColor clr = { colors::vehicles::horse[0], colors::vehicles::horse[1], colors::vehicles::horse[2] };

				if ( var::esp::vehicles_helicopters && ( element_hash == minicopter_hash || element_hash == scrapheli_hash ) )
				{
					clr = { colors::vehicles::helicopter[0], colors::vehicles::helicopter[1], colors::vehicles::helicopter[2] };
					translated_name = ( element_hash == minicopter_hash ? _ ( "Minicopter" ) : _ ( "ScrapHelicopter" ) );
				}
				else if ( var::esp::vehicles_boats && ( element_hash == boat || element_hash == rhib ) )
				{
					clr = { colors::vehicles::boat[0], colors::vehicles::boat[1], colors::vehicles::boat[2] };
					translated_name = ( element_hash == boat ? _ ( "Boat" ) : _ ( "RHIB" ) );
				}
				else if ( var::esp::vehicles_horse && ( element_hash == horse ) )
				{
					translated_name = _ ( "Horse" );
				}
				else
					continue;

				auto pair = std::make_tuple ( objectClass, translated_name, position, clr );
				vehicles.emplace_back ( pair );
			}
			else if ( var::esp::traps && ( element_hash == turret_hash || element_hash == flameturret_hash || element_hash == landmine_hash || element_hash == beartrap_hash || element_hash == samsite_hash || element_hash == guntrap_hash ) )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );
				std::string translated_name = "";
				ImColor clr { };

				if ( var::esp::autoturret && HE ( element_hash, turret_hash ) )
				{
					translated_name = _ ( "Auto-turret" );
					clr = { colors::traps::autoturret[0], colors::traps::autoturret[1], colors::traps::autoturret[2] };
				}
				else if ( var::esp::flameturret && HE ( element_hash, flameturret_hash ) )
				{
					translated_name = _ ( "Flame-turret" );
					clr = { colors::traps::flameturret[0], colors::traps::flameturret[1], colors::traps::flameturret[2] };
				}
				else if ( var::esp::landmine && HE ( element_hash, landmine_hash ) )
				{
					translated_name = _ ( "Land-mine" );
					clr = { colors::traps::landmine[0], colors::traps::landmine[1], colors::traps::landmine[2] };
				}
				else if ( var::esp::beartrap && HE ( element_hash, beartrap_hash ) )
				{
					translated_name = _ ( "Bear-trap" );
					clr = { colors::traps::beartrap[0], colors::traps::beartrap[1], colors::traps::beartrap[2] };
				}
				else if ( var::esp::samsite && HE ( element_hash, samsite_hash ) )
				{
					translated_name = _ ( "SAM-site" );
					clr = { colors::traps::samsite[0], colors::traps::samsite[1], colors::traps::samsite[2] };
				}
				else if ( var::esp::shotguntrap && HE ( element_hash, guntrap_hash ) )
				{
					translated_name = _ ( "Shotgun-trap" );
					clr = { colors::traps::guntrap[0], colors::traps::guntrap[1], colors::traps::guntrap[2] };
				}
				else
					continue;

				auto pair = std::make_tuple ( objectClass, translated_name, position, clr );
				traps.emplace_back ( pair );
			}
			else if ( var::esp::toolcupboard && element_hash == toolcupboard_hash )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );
				ImColor clr { colors::other::toolcupboard[0], colors::other::toolcupboard[1], colors::other::toolcupboard[2] };
				std::vector<std::string> usernames;
				if ( var::esp::toolcupboard_list )
				{
					uintptr_t m_pList = mist::obj->read<uintptr_t> ( element + 0x568 );
					uintptr_t m_pAuthorizedPlayersList = mist::obj->read<uintptr_t> ( m_pList + 0x10 );
					uint32_t m_pAuthorizedPlayersSize = mist::obj->read<uint32_t> ( m_pList + 0x18 );
					for ( uint32_t i = 0; i < m_pAuthorizedPlayersSize; ++i )
					{
						uintptr_t m_pEntry = mist::obj->read<uintptr_t> ( m_pAuthorizedPlayersList + 0x20 + ( i * 0x08 ) ); CASSERT ( m_pEntry );
						std::string username = mist::obj->read_unicode ( mist::obj->read<uintptr_t> ( m_pEntry + 0x18 ) + 0x14, 32 );
						//logging::print ( TYPE_DEBUG, "[0x%p] CUPBOARD LIST -> %s", objectClass, username.c_str() );
						if ( !username.empty ( ) )
							usernames.emplace_back ( username );
					}
				}

				auto pair = std::make_tuple ( objectClass, usernames, position, clr );
				cupboard.emplace_back ( pair );
			}
			else if ( var::esp::sleepingbag && element_hash == sleepingbag_hash )
			{
				Vector3 position = mist::obj->read<Vector3> ( rust::get_object_pos_component ( mist::obj.get ( ), objectClass ) + 0x90 );
				if ( position.empty ( ) )
					continue;

				uintptr_t pObjName = mist::obj->read<uintptr_t> ( object + 0x60 );
				std::string object_name = mist::obj->read_ascii ( pObjName, 128 );
				const auto object_hash = hash_constfnv64_string ( object_name.c_str ( ) );
				std::string translated_name = _ ( "bag" );
				ImColor clr { colors::other::sleepingbag[0], colors::other::sleepingbag[1], colors::other::sleepingbag[2] };

				ULONG64 owner = SteamID3To64 ( mist::obj->read<ULONG64> ( element + 0x3F0 ) );
				std::string owner_name;
				if ( game::user_cache.find ( owner ) == game::user_cache.end ( ) )
				{
					steam::get_username_steam64 ( owner, owner_name );
					game::user_cache.emplace ( owner, owner_name );
				}
				else
					owner_name = game::user_cache[owner];

				translated_name = _ ( "bag (" ) + owner_name + _ ( ")" );

				auto pair = std::make_tuple ( objectClass, translated_name, position, clr );
				sleepingbag.emplace_back ( pair );
			}

			_mm_pause ( );
		}

		game::players = players;
		game::items = items;
		game::ores = ores;
		game::collectibles = collectibles;
		game::barrel = barrel;
		game::stashes = stashes;
		game::animals = animals;
		game::special = special;
		game::vehicles = vehicles;
		game::traps = traps;
		game::bag = sleepingbag;
		game::cupboard = cupboard;

		sleep_for ( 1s );
	}
}