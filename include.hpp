#ifndef _INCLUDE_GUARD
#define _INCLUDE_GUARD

#include "stdafx.hpp"

#include "communication.hpp"
#include "lazy_importer.hpp"
#include "xorfloat.hpp"
#include "xorstr.hpp"
#include "crypto.hpp"
#include "syscall.hpp"
#include "mutex.hpp"
#include "math.hpp"

using namespace std::chrono;
using namespace std::this_thread;

namespace offsets {
	constexpr uint64_t gom_offset = 0x17C1F18;
	constexpr uint64_t bn_offset = 0x2FFD588; // BaseNetworkable_c*
	constexpr uint64_t fov_offset = 0x2FFC2B8; // ConVar_Graphics_c*
	constexpr uint64_t global_offset = 0x2FFDF68; // ConVar_Global_c*
	constexpr uint64_t admin_offset = 0x2FF8D70; // ConVar_Admin_c*
	constexpr uint64_t cl_ent = 0xB8;
	constexpr uint64_t ent_realm = 0x10;
	constexpr uint64_t buf_list = 0x28;
	constexpr uint64_t obj_list = 0x18;
	constexpr uint64_t obj_size = 0x10;
	constexpr uint64_t base_ply = 0x28;
	constexpr uint64_t class_name = 0x10;
	constexpr uint64_t player_visual = 0x08;
	constexpr uint64_t obj_state = 0x38;
	constexpr uint64_t obj_pos = 0x90;
	constexpr uint64_t model = 0x118;
	constexpr uint64_t player_model = 0x4A8;
	constexpr uint64_t multi_mesh = 0x280;
	constexpr uint64_t element = 0x20;

	constexpr uint64_t clothingBlocksAiming = 0x6B8;
	constexpr uint64_t clothingMoveSpeedReduction = 0x6BC;
	constexpr uint64_t clothingWaterSpeedBonus = 0x6C0;
	constexpr uint64_t modelState = 0x588;
	constexpr uint64_t basemovement = 0x4D0;
	constexpr uint64_t groundangle = 0xB0;
	constexpr uint64_t groundangle_new = 0xB8;
	constexpr uint64_t gravity = 0x74;
	constexpr uint64_t flying = 0x13C;
	constexpr uint64_t swimming = 0x136;
	constexpr uint64_t climbing = 0x132;
	constexpr uint64_t groundtime = 0xBC;
	constexpr uint64_t maxangle_climb = 0x80;
	constexpr uint64_t maxangle_walk = 0x7C;

	constexpr uint64_t waterLevel = 0x14;
	constexpr uint64_t target_movement = 0x34;

	constexpr uint64_t flags = 0x24;

	constexpr uint64_t player_input = 0x4C8;
	constexpr uint64_t body_angles = 0x3C;
	constexpr uint64_t head_angles = 0x58;
	constexpr uint64_t recoil_angles = 0x64;

	constexpr uint64_t eyes = 0x600;

	constexpr uint64_t velocity = 0x1FC;
	constexpr uint64_t oldvelocity = 0x1E4;
	constexpr uint64_t magazine = 0x2A0;
	constexpr uint64_t ammotype = 0x20;
	constexpr uint64_t shortname = 0x20;
	constexpr uint64_t active_item = 0x570;

	constexpr uint64_t base_projectile = 0x98;
	constexpr uint64_t sway = 0x2B8;
	constexpr uint64_t aimcone = 0x2D0;

	constexpr uint64_t recoil_properties = 0x2C0;

	constexpr uint64_t PlayerFlags = 0x5F8;

	constexpr uint64_t inventory = 0x608;
	constexpr uint64_t display_name = 0x658;
	constexpr uint64_t uid = 0x640;
	constexpr uint64_t health = 0x20C;
	constexpr uint64_t team = 0x540;
	constexpr uint64_t timescale = 0x17C1CD0;
	constexpr uint64_t effect = 0x1D98DF4;
};

namespace addr {
	extern uintptr_t unityplayer;
	extern uintptr_t gameassembly;

	extern uintptr_t base_networkable;
	extern uintptr_t client_entities;
	extern uintptr_t entity_realm;
	extern uintptr_t buffer_list;
	extern uintptr_t object_list;
	extern uintptr_t camera_instance;
	extern uintptr_t tod_sky;
	extern std::size_t object_size;
};

namespace monitor {
	extern HWND game_window;
	extern float width, height;
};

namespace security {
	extern bool unload;
	extern bool eject;
};

namespace syscalls {
	HWND find_window ( const char* szClassName, const char* szWindowName );
	SHORT get_async_key_state ( SHORT vk );
	HWND get_foreground_window ( );
	BOOL show_window ( HWND hWnd, int nCmdShow );
	BOOL set_window_pos ( HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags );
	HWND get_window ( HWND hWnd, UINT uCmd );
	int set_window_rgn ( HWND hWnd, HRGN hRgn, BOOL bRedraw );
};
enum : char {
	TYPE_LOG,
	TYPE_DEBUG,
	TYPE_ERROR,
	TYPE_WARNING
};
#ifdef _DEBUG
constexpr inline const char* GET_PREFIX ( const char type )
{
	switch ( type )
	{
	case TYPE_LOG:
		return "\u001b[37m[-] ";
		break;
	case TYPE_DEBUG:
		return "\u001b[36m[+] ";
		break;
	case TYPE_ERROR:
		return "\u001b[31m[x] ";
		break;
	case TYPE_WARNING:
		return "\u001b[33m[!] ";
		break;
	default:
		return "\u001b[37m[?] ";
		break;
	}
}

namespace logging {
	inline void print ( const char type, const char* format, ... )
	{
		char buffer[256];
		va_list arguments;
		va_start ( arguments, format );
		vsnprintf_s ( buffer, 256, format, arguments );

		const char* PREFIX = GET_PREFIX ( type );
		printf ( "%s%s\n", PREFIX, buffer );
	}
};
#else
namespace logging {
	inline void print ( const char type, const char* format, ... )
	{
		return;
	}
};
#endif

#define VASSERT(x) \
if ( !x ) \
return;

#define CASSERT(x) \
if ( !x ) \
continue;

#define ASSERT(x) \
if ( !x ) \
return false;

#define BASSERT(x) \
if ( !x ) \
break;
#endif