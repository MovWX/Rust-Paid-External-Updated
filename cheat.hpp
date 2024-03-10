#pragma once

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <mutex>
#include <vector>

#include "math.hpp"
#include "include.hpp"
#include "sdk.hpp"
#include "baseplayer.hpp"
#include "item.hpp"
#include "steam.hpp"
#include "menu.hpp"

enum class entity_type : size_t
{
	BasePlayer,
	StorageContainer,
	SupplyDrop,
	VendingMachineMapMarker,
	ExcavatorArm,
	LightListener,
	BaseVehicleSeat,
	Boar,
	Bear,
	Deer,
	Chicken,
	Wolf,
	Horse,
	HotAirBalloon,
	MiniCopter,
	DroppedItemContainer,
	OreResourceEntity,
	ResourceEntity,
	StashContainer,
	DroppedItem,
	CollectibleEntity,
	BaseMelee,
	BaseProjectile,
	TreeEntity,
	OreHotSpot,
	LootContainer,
	FreeableLootContainer,
	Recycler,
	JunkPileWater,
	Keycard,
	HackableLockedCrate,
	ItemPickup,
	Undefined,
	WorldItem,
	BradleyAPC,
	PlayerCorpse,
	BaseHelicopter,
	Scientist,
	GunTrap,
	FlameTurret,
	Landmine,
	BearTrap,
	AutoTurret,
	Barricade,
	RidableHorse,
	GeneralAnimal
};

enum BoneList : int
{
	l_hip = 1,
	l_knee,
	l_foot,
	l_toe,
	l_ankle_scale,
	pelvis,
	penis,
	GenitalCensor,
	GenitalCensor_LOD0,
	Inner_LOD0,
	GenitalCensor_LOD1,
	GenitalCensor_LOD2,
	r_hip,
	r_knee,
	r_foot,
	r_toe,
	r_ankle_scale,
	spine1,
	spine1_scale,
	spine2,
	spine3,
	spine4,
	l_clavicle,
	l_upperarm,
	l_forearm,
	l_hand,
	l_index1,
	l_index2,
	l_index3,
	l_little1,
	l_little2,
	l_little3,
	l_middle1,
	l_middle2,
	l_middle3,
	l_prop,
	l_ring1,
	l_ring2,
	l_ring3,
	l_thumb1,
	l_thumb2,
	l_thumb3,
	IKtarget_righthand_min,
	IKtarget_righthand_max,
	l_ulna,
	neck,
	head,
	jaw,
	eyeTranform,
	l_eye,
	l_Eyelid,
	r_eye,
	r_Eyelid,
	r_clavicle,
	r_upperarm,
	r_forearm,
	r_hand,
	r_index1,
	r_index2,
	r_index3,
	r_little1,
	r_little2,
	r_little3,
	r_middle1,
	r_middle2,
	r_middle3,
	r_prop,
	r_ring1,
	r_ring2,
	r_ring3,
	r_thumb1,
	r_thumb2,
	r_thumb3,
	IKtarget_lefthand_min,
	IKtarget_lefthand_max,
	r_ulna,
	l_breast,
	r_breast,
	BoobCensor,
	BreastCensor_LOD0,
	BreastCensor_LOD1,
	BreastCensor_LOD2,
	collision,
	displacement
};

enum class BoneListAttackHeli : int {
	PatrolHelicopter = 0,
	RotorPivot,
	patrol_helicopter,
	w_patrol_helicopter_body,
	w_patrol_helicopter_body_LOD0,
	w_patrol_helicopter_body_LOD1,
	w_patrol_helicopter_body_LOD2,
	w_patrol_helicopter_body_LOD3,
	w_patrol_helicopter_body_LOD4,
	gun_left_yaw,
	gun_left,
	gun_left_muzzle,
	gun_left_LOD0,
	gun_left_LOD1,
	gun_right_yaw,
	gun_right,
	gun_right_muzzle,
	gun_right_LOD0,
	gun_right_LOD1,
	main_rotor,
	main_rotor_blades,
	main_rotor_blur,
	rear_rotor,
	rear_rotor_blades,
	rear_rotor_blur,
	w_patrol_helicopter_glass,
	w_patrol_helicopter_interior,
	w_patrol_helicopter_interior_LOD0,
	w_patrol_helicopter_interior_LOD1,
	w_patrol_helicopter_interior_LOD2,
	w_patrol_helicopter_rockets,
	w_patrol_helicopter_rockets_LOD0,
	w_patrol_helicopter_rockets_LOD1,
	w_patrol_helicopter_rockets_LOD2,
	DamageEffects,
	tailDestroyed,
	Light_1_1,
	Explosion_v1,
	Light_1_1_1,
	engineDestroyed,
	Light_1_2,
	Explosion_v1_1,
	Light_1_2_1,
	tailDamaged,
	mainRotorDamaged,
	tail_rotor_col,
	main_rotor_col,
	engine_col,
	spotlight_pivot,
	lighthack,
	Spotlight,
	Quad,
	Quad_1,
	Quad_2,
	Flare,
	Sphere_1,
	beam_1,
	NightLights,
	left_light,
	right_Light,
	rightBulb,
	leftBulb,
	ChaffEffects,
	chaff_right,
	smoketrail,
	chaff_left,
	smoketrail_1,
	rocket_tube_right,
	right_tube_rear,
	rocket_tube_left,
	left_tube_rear,
	WindZone
};

enum class player_flags : int
{
	admin = 4,
	snapshot = 8,
	sleeping = 16,
	spectating = 32,
	wounded = 64,
	developer = 128,
	connected = 256,
	thirdperson = 1024,
	eyesviewmode = 2048,
	muted = 4096,
	nosprint = 8192,
	aiming = 16384,
	sash = 32768,
	relaxed = 65536,
	safezone = 131072,
	serverfall = 262144,
	workbench1 = 1048576,
	workbench2 = 2097152,
	workbench3 = 4194304
};

#define RAD2DEG(x) { (x * (180.f / M_PI)) }
#define DEG2RAD(x) { (x * (M_PI / 180.f)) }

struct rWeaponInfo {
	bool m_bPrediction;
	float m_flSpeed;
	float m_flGravity;
};

struct rEntity {
	uintptr_t m_pPlayer;
	entity_type m_eType;
};

struct rLocal {
	uintptr_t m_pPlayer;
	ModelState* m_pModelState;
	Item* m_pWeapon;
	std::string m_szWeaponName;
	std::string m_szBulletType;
	uint64_t m_hWeaponName;
	uint64_t m_hBulletType;
	Vector3 m_vecLocal;

	rWeaponInfo m_vInfo;
};

struct rCache {
	std::string m_szName;
	std::wstring m_wzName;
	std::string m_szWeaponName;
	uintptr_t player_model;
	uintptr_t transform;

	BasePlayer* m_pPlayer;

	Item* m_pWeapon;
	Item* m_pLastWeapon;

	bool m_bWhitelist;
	bool m_bFriend;
	bool m_bSleeper;
};

enum EffectType {
	C4_Explosion,
	Rocket_Explosion,
	RocketIncen_Explosion,
	Satchel_Explosion,
	HeGl_Explosion,
	ExploAmmo_Explosion
};

struct Effect {
public:
	Vector3 position;
	EffectType type;
	std::time_t time;

	Effect ( Vector3 pos, EffectType effectType )
	{
		position = pos;
		type = effectType;
		time = std::time ( nullptr );
	}
};

struct Raid {
public:
	Vector3 position;
	int c4 = 0;
	int rocket = 0;
	int rocketIncen = 0;
	int satchel = 0;
	int heGrenadeLauncher = 0;
	int exploAmmo = 0;
	std::time_t lastTime;
};

struct color_t {
	int r, g, b;
};

namespace keybinds {
	extern bool menu;
};

namespace data {
	namespace aimbot {
		extern uintptr_t m_pTarget;
		extern bool m_bFound;
		extern bool m_bActive;
		extern entity_type m_eType;
		extern int m_iHitbox;
		extern Vector3 m_vecPosition;
		extern float m_flFOV;
		extern std::vector<Vector3> m_vecBulletSimulation;
		extern time_t m_tShot;
		extern bool m_bLocked;
	};
};

struct ImColor;
namespace game {
	typedef std::vector<std::pair<uintptr_t, entity_type>> entSet;
	typedef std::vector<std::pair<uintptr_t, std::string>> stringSet;
	typedef std::vector<std::tuple<uintptr_t, std::string, Vector3, std::chrono::steady_clock::time_point>> raidSet;
	typedef std::vector<std::tuple<uintptr_t, std::string, Vector3>> staticStringSet;
	typedef std::vector<std::tuple<uintptr_t, std::string, Vector3, ImColor>> staticStringColorSet;
	typedef std::vector<std::tuple<uintptr_t, std::vector<std::string>, Vector3, ImColor>> staticStringListColorSet;
	typedef std::unordered_map<ULONG, std::string> PlayerIdName;

	extern BasePlayer* m_pLocalplayer;
	extern rLocal m_rLocalInfo;
	extern entSet players;
	extern stringSet special;
	extern staticStringSet stashes, animals;
	extern staticStringColorSet items, collectibles, ores, vehicles, barrel, traps, bag;
	extern staticStringListColorSet cupboard;
	extern raidSet raid;
	extern PlayerIdName user_cache;
	extern std::unordered_set<ULONG> friendlist;
	extern std::unordered_map<ULONG, rCache> cache;
	extern std::mutex draw_mutex, aim_mutex;
};

namespace colors {
	namespace player {
		extern float name[3];
		extern float box[3];
		extern float skeleton[3];
		extern float distance[3];
		extern float weapon[3];
		extern float ammo[3];
		extern float sleeper[3];
		extern float npc[3];
		extern float snapline[3];
	};

	namespace dropped {
		extern float weapon[3];
		extern float corpse[3];
		extern float other[3];
	};

	namespace ore {
		extern float stone[3];
		extern float sulfur[3];
		extern float metal[3];
	};

	namespace collectable {
		extern float hemp[3];
		extern float mushroom[3];
		extern float corn[3];
		extern float berry[3];
		extern float potato[3];
		extern float sulfur[3];
		extern float stone[3];
		extern float metal[3];
		extern float wood[3];
	};

	namespace radtown {
		extern float barrel[3];
		extern float crate[3];
		extern float mcrate[3];
		extern float ecrate[3];
		extern float toolbox[3];
		extern float oil[3];
		extern float minecart[3];
	};

	namespace vehicles {
		extern float boat[3];
		extern float helicopter[3];
		extern float horse[3];
		extern float special[3];
	};

	namespace traps {
		extern float autoturret[3];
		extern float flameturret[3];
		extern float samsite[3];
		extern float landmine[3];
		extern float beartrap[3];
		extern float guntrap[3];
	};

	namespace other {
		extern float stash[3];
		extern float raid[3];
		extern float fov[3];
		extern float animal[3];
		extern float crosshair[3];
		extern float toolcupboard[3];
		extern float sleepingbag[3];
	};
};