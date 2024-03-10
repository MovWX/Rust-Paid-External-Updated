#pragma once
#include <filesystem>
#include <fstream>
Microsoft::WRL::ComPtr<ID3D11Device> device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context;
Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;

static bool overlay_initialized = false;


template<typename T, typename E>
__forceinline constexpr void erase_from_vec ( T& vector, E& entry )
{
	vector.erase ( std::remove ( vector.begin ( ), vector.end ( ), entry ), vector.end ( ) );
}

HRESULT ( WINAPI* oPresent )( IDXGISwapChain*, UINT, UINT );

using WndProcHk_ = __int64 ( __stdcall* )( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
WndProcHk_ oWndProc = NULL;

BOOL bDataCompare ( const BYTE* pData, const BYTE* bMask, const char* szMask )
{
	for ( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if ( *szMask == 'x' && *pData != *bMask )
			return FALSE;
	}
	return ( *szMask ) == NULL;
}

DWORD64 FindPattern ( BYTE* bMask, const char* szMask )
{
	MODULEINFO mi { };
	GetModuleInformation ( GetCurrentProcess ( ), GetModuleHandleA ( NULL ), &mi, sizeof ( mi ) );

	DWORD64 dwBaseAddress = DWORD64 ( mi.lpBaseOfDll );
	const auto dwModuleSize = mi.SizeOfImage;

	for ( auto i = 0ul; i < dwModuleSize; i++ )
	{
		if ( bDataCompare ( PBYTE ( dwBaseAddress + i ), bMask, szMask ) )
			return DWORD64 ( dwBaseAddress + i );
	}
	return NULL;
}


D3DPRESENT_PARAMETERS params { };
HWND unitywin;
ImDrawList* dwl;
ImFont* arial, * espfont;
#ifdef GETREKT
ImFont* menufont;
ImFont* menufont_bold;
ImFont* tabfont;
ImFont* spacerfont;
ImFont* weap_icons;
ImFont* weap_icons_menu_size;
ImFont* logofont;
ImFont* infocorrner_text;

////// Colors ////////
ImVec4 button;
ImVec4 buttonhovered;
ImVec4 buttonActive;
ImVec4 text;
ImVec4 bg_color;
ImVec4 themecolor;
ImVec4 bg_child_color;
ImVec4 separator;
ImVec4 slider = ImVec4 ( 0.69f, 0.69f, 0.69f, 0.80f );
bool color_picker_open = false;
int scrrenWidth = 1280;
int scrrenHeight = 800;
Config g_Options;
ImGuiID Colorpicker_Close = 0;
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

ImGuiColorEditFlags noob = ImGuiColorEditFlags_NoInputs;
void get_closest_point ( Vector3 from, int depth );
void imgui_stuff ( );
bool save_config ( );
bool load_config ( );

int frmtm = 0;

namespace monitor {
	HWND game_window;
	float width, height;
};

enum {
	TAB_AIM,
	TAB_ESP,
	TAB_MISC,
};

enum {
	TAB_PLAYER,
	TAB_DROPPED,
	TAB_ORE,
	TAB_RAD,
	TAB_COLLECT,
	TAB_OTHER,
	TAB_VEHICLES,
	TAB_TRAPS
};

int m_iTab = 0, m_iSubTab = 0;
bool* a;
bool vsync;
float m_flFontHeight = 0.f;

namespace var
{
	namespace aim {
		bool enabled = true;

		int hitbox = 3;
		int aim_fov = 45;
		int aim_key = VK_XBUTTON2;
		bool silent = false;
		bool terraincheck = true;
		bool ignore_friends = true;
		float smoothing = 10.f;
	};

	namespace esp {
		bool enabled = true;
		bool players = true;
		bool sleeper = true;
		bool npc = true;
		bool special = true;
		bool animal = false;
		bool fov_circle = false;
		bool raid = false;

		bool name = true;
		bool box = true;
		bool skeleton = true;
		bool health = true;
		bool distance = true;
		bool ammo = false;
		bool snaplines = false;
		bool teammate = true;
		bool highlight_target = true;
		bool weapon = true;
		bool item_info = true;
		bool item = true;
		bool item_weapon = true;
		bool item_other = true;
		bool item_corpse = true;
		bool ores = false;
		bool sulfur = true, stone = true, metal = false;
		bool stash = false;
		bool radtown = false;
		bool collectable = false;
		bool animals = false;
		bool hide = true;

		bool vehicles;
		bool vehicles_horse;
		bool vehicles_boats;
		bool vehicles_helicopters;

		bool traps;
		bool shotguntrap;
		bool autoturret;
		bool flameturret;
		bool landmine;
		bool beartrap;
		bool samsite;

		bool crate;
		bool ecrate;
		bool mcrate;
		bool barrel;
		bool toolbox;
		bool oil;
		bool minecart;

		bool toolcupboard = true;
		bool toolcupboard_list = true;
		bool sleepingbag;

		bool hemp = true;
		bool mushroom = true;
		bool corn = true;
		bool berry = true;
		bool potato = true;
		bool csulfur = true;
		bool cstone = true;
		bool cmetal = true;
		bool crosshair = true;
		bool cwood = true;

		float distance_limit = 4000.f;
	};

	namespace misc {
		float recoil_pitch = 100.f;
		float recoil_yaw = 100.f;
		bool force_automatic;
		bool nospread;
		bool noshotgunspread;
		bool spider;

		bool eoka = false;
		bool compound = false;
		bool fullbright = false;
		bool alwaysday = false;
		bool admin = false;
		bool noslide = false;
		bool turboclip = false;
		float noclip_speed = 10.f;
		bool bgravity;
		int gravity;
		bool btimescale;
		int timescale;
		float timescale_speed = 3.f;
		bool bfly;
		int fly;
		bool radar = true;
		int scale = 100;
		bool bview_offset;
		int view_offset;
		bool bclimb;
		int assist;
		bool fakelag = false;
		bool fastbow = false;
		bool jumpshoot = false;
		bool omnisprint = false;
		bool alwayswhite = false;
		bool alwaysnight = false;
		bool thickness = false;
		float flthickness = 0.4f;
		float fov = 70.f;
		float x, y, z = .75f;
	};
};

namespace colors {
	namespace player {
		float name[3] = { 1.f, 1.f, 1.f };
		float box[3] = { 1.f, 1.f, 1.f };
		float skeleton[3] = { 1.f, 1.f, 1.f };
		float distance[3] = { 1.f, 1.f, 1.f };
		float weapon[3] = { 1.f, 1.f, 1.f };
		float ammo[3] = { 1.f, 1.f, 1.f };
		float sleeper[3] = { 0.913725555f, 0.588235319f, 0.478431404f };
		float npc[3] = { 0.2588f, 0.5294f, 0.96f };
		float snapline[3] = { 1.f, 1.f, 1.f };
		float teammate[3] = { 0.196078449f, 0.803921640f, 0.196078449f };
		float target[3] = { 1.f, 0.f, 0.f };
	};

	namespace dropped {
		float weapon[3] = { 0.196078449f, 0.803921640f, 0.196078449f };
		float corpse[3] = { 0.372549027f, 0.619607866f, 0.627451003f };
		float other[3] = { 0.941176534f, 0.901960850f, 0.549019635f };
	};

	namespace ore {
		float stone[3] = { 1.000000000f, 0.980392218f, 0.980392218f };
		float sulfur[3] = { 0.854902029f, 0.647058845f, 0.125490203f };
		float metal[3] = { 0.662745118f, 0.662745118f, 0.662745118f };
	};

	namespace collectable {
		float hemp[3] = { 0.333333343f, 0.419607878f, 0.184313729f };
		float mushroom[3] = { 0.737254918f, 0.560784340f, 0.560784340f };
		float corn[3] = { 0.980392218f, 0.980392218f, 0.823529482f };
		float berry[3] = { 0.780392230f, 0.082352944f, 0.521568656f };
		float potato[3] = { 1.000000000f, 0.921568692f, 0.803921640f };
		float sulfur[3] = { 0.678431392f, 1.000000000f, 0.184313729f };
		float stone[3] = { 1.000000000f, 0.980392218f, 0.980392218f };
		float metal[3] = { 0.184313729f, 0.309803933f, 0.309803933f };
		float wood[3] = { 0.415686309f, 0.352941185f, 0.803921640f };
	};

	namespace radtown {
		float barrel[3] = { 1.f, 1.f, 1.f };
		float crate[3] = { 1.f, 1.f, 1.f };
		float mcrate[3] = { 1.f, 1.f, 1.f };
		float ecrate[3] = { 1.f, 1.f, 1.f };
		float toolbox[3] = { 1.f, 1.f, 1.f };
		float oil[3] = { 1.f, 1.f, 1.f };
		float minecart[3] = { 1.f, 1.f, 1.f };
	};

	namespace vehicles {
		float boat[3] = { 1.f, 1.f, 1.f };
		float helicopter[3] = { 1.f, 1.f, 1.f };
		float horse[3] = { 1.f, 1.f, 1.f };
		float special[3] = { 0.823529482f, 0.411764741f, 0.117647067f };
	};

	namespace traps {
		float autoturret[3] = { 1.f, 1.f, 1.f };
		float flameturret[3] = { 1.f, 1.f, 1.f };
		float samsite[3] = { 1.f, 1.f, 1.f };
		float landmine[3] = { 1.f, 1.f, 1.f };
		float beartrap[3] = { 1.f, 1.f, 1.f };
		float guntrap[3] = { 1.f, 1.f, 1.f };
	};

	namespace other {
		float stash[3] = { 1.f, 0.f, 0.f };
		float raid[3] = { 1.f, 0.f, 0.f };
		float fov[3] = { 1.f, 1.f, 1.f };
		float animal[3] = { 0.9607f, 0.7255f, 0.2588f };
		float crosshair[3] = { 1.000000000f, 0.752941251f, 0.796078503f };
		float toolcupboard[3] = { 1.f, 1.f, 1.f };
		float sleepingbag[3] = { 1.f, 1.f, 1.f };
	};
};

template <typename T>
void ESP_stringset ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = entry.first;
		if ( !entity )
			continue;

		Vector3 position = mist::esp->read<Vector3> ( rust::get_object_pos_component ( mist::esp.get ( ), entity ) + 0x90 );
		if ( position.empty ( ) )
			continue;

		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );
		if ( m_flDistance > var::esp::distance_limit )
			continue;

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		char bfr[128];
		sprintf ( bfr, "%s (%dM)", entry.second.c_str ( ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr, ImColor { colors::vehicles::special[0], colors::vehicles::special[1], colors::vehicles::special[2] }, true, true );
	}
}

template <typename T>
void ESP_raidset ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = std::get<0> ( entry );
		auto time = std::get<3> ( entry );

		if ( time > std::chrono::steady_clock::now ( ) + minutes ( 10 ) )
			continue;

		auto position = std::get<2> ( entry );
		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		char bfr[128];
		sprintf ( bfr, "%s (%dM)", std::get<1> ( entry ).c_str ( ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr, ImColor { colors::other::raid[0], colors::other::raid[1], colors::other::raid[2] }, true, true );
	}
}

template <typename T>
void ESP_staticstringset ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = std::get<0> ( entry );
		if ( !entity )
			continue;

		Vector3 position = std::get<2> ( entry );

		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		if ( m_flDistance > 250.f )
			continue;

		if ( m_flDistance > var::esp::distance_limit )
			continue;

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		auto radColor = ImColor { colors::other::animal[0], colors::other::animal[1], colors::other::animal[2] };
		auto name = std::get<1> ( entry );

		char bfr[128];
		sprintf ( bfr, "%s (%dM)", name.c_str ( ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr, radColor, true, true );
	}
}

template <typename T>
void ESP_staticstringset ( T& vec, ImColor clr )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = std::get<0> ( entry );
		if ( !entity )
			continue;

		Vector3 position = std::get<2> ( entry );

		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		if ( m_flDistance > 250.f )
			continue;

		if ( m_flDistance > var::esp::distance_limit )
			continue;

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		auto name = std::get<1> ( entry );

		char bfr[128];
		sprintf ( bfr, "%s (%dM)", name.c_str ( ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr, clr, true, true );
	}
}

template <typename T>
void ESP_staticstringcolorset ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = std::get<0> ( entry );
		if ( !entity )
			continue;

		Vector3 position = std::get<2> ( entry );

		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		if ( m_flDistance > var::esp::distance_limit )
			continue;

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		auto name = std::get<1> ( entry );

		char bfr[128];
		sprintf ( bfr, "%s (%dM)", name.c_str ( ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr, std::get<3> ( entry ), true, true );
	}
}

template <typename T>
void ESP_staticstringlistcolorset ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto entity = std::get<0> ( entry );
		if ( !entity )
			continue;

		Vector3 position = std::get<2> ( entry );

		int m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		if ( m_flDistance > var::esp::distance_limit )
			continue;

		Vector2 m_scrPosition;
		if ( !rust::world_to_screen ( position, m_scrPosition, m_mxViewMatrix ) )
			continue;

		auto names = std::get<1> ( entry );

		char bfr1[78];
		sprintf ( bfr1, "%s (%dM)", _ ( "Cupboard" ), m_flDistance );
		renderer->draw_text ( m_scrPosition.x, m_scrPosition.y, bfr1, std::get<3> ( entry ), true, true );
		int i = 1;
		for ( const auto& username : names )
		{
			char bfr[78];
			sprintf ( bfr, "%s", username.c_str ( ), m_flDistance );
			renderer->draw_text ( m_scrPosition.x, m_scrPosition.y + 13 * i, bfr, std::get<3> ( entry ), true, true );
			++i;
		}
	}
}

template <typename T>
void ESP_player ( T& vec )
{
	Matrix4x4 m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );
	for ( const auto& entry : vec )
	{
		const auto& entity = entry.first;
		if ( !entity )
			continue;

		if ( entry.second == entity_type::Scientist && !var::esp::npc )
			continue;

		const Vector3 position = mist::esp->read<Vector3> ( rust::get_object_pos_component ( mist::esp.get ( ), entity ) + 0x90 );
		if ( position.empty ( ) )
			continue;

		float m_flDistance = game::m_rLocalInfo.m_vecLocal.distance ( position );

		if ( m_flDistance > var::esp::distance_limit )
			continue;

		BasePlayer* player = (BasePlayer*)rust::get_base_entity ( mist::esp.get ( ), entity );

		const ULONG uid = player->get_uid ( mist::esp.get ( ) );
		auto& cache = game::cache[uid];

		Vector2 right_foot, left_foot;
		Vector3 reduce { 0.f, 0.05f, 0.f };
		Vector2 head;

		if ( !rust::world_to_screen ( player->get_bone_position ( mist::esp.get ( ), BoneList::r_foot, cache.transform ) - reduce, right_foot, m_mxViewMatrix, true ) ||
			!rust::world_to_screen ( player->get_bone_position ( mist::esp.get ( ), BoneList::l_foot, cache.transform ) - reduce, left_foot, m_mxViewMatrix, true ) ||
			!rust::world_to_screen ( player->get_bone_position ( mist::esp.get ( ), BoneList::head, cache.transform ) + Vector3 ( xf ( 0.f ), xf ( 0.22f ), xf ( 0.f ) ), head, m_mxViewMatrix, true ) )
		{
			continue;
		}
		const Vector2 tempFeet = ( right_foot + left_foot ) / 2.f;
		const float _h = head.y - tempFeet.y;
		const float h = tempFeet.y - head.y;
		const float y = head.y;
		const float w = std::abs ( _h / 2.f );
		float x = tempFeet.x - _h / 4.f;

		if ( cache.m_bSleeper )
		{
			if ( var::esp::sleeper )
			{
				char bfr[128];
				sprintf ( bfr, "%s (%.0fM)", cache.m_szName.c_str ( ), m_flDistance );
				renderer->draw_text ( x + w / 2, y, bfr, { colors::player::sleeper[0], colors::player::sleeper[1], colors::player::sleeper[2] }, true, true );
			}
			continue;
		}

		const float max_health = entry.second == entity_type::Scientist ? 150.f : 100.f;
		const float health = min ( player->get_health ( mist::esp.get ( ) ), max_health );
		const ImVec2 health_size = renderer->measure_text ( std::to_string ( (int)health ) );
		const float x_offset = health_size.x;
		const float y_offset = health_size.y;

		if ( health <= 0.f )
			continue;

		x = x - w;

		float bx_r = colors::player::box[0], bx_g = colors::player::box[1], bx_b = colors::player::box[2];
		if ( entry.second == entity_type::Scientist )
			bx_r = colors::player::npc[0], bx_g = colors::player::npc[1], bx_b = colors::player::npc[2];

		if ( var::esp::teammate && cache.m_bFriend )
			bx_r = colors::player::teammate[0], bx_g = colors::player::teammate[1], bx_b = colors::player::teammate[2];
		else if ( var::esp::highlight_target && entity == data::aimbot::m_pTarget )
			bx_r = colors::player::target[0], bx_g = colors::player::target[1], bx_b = colors::player::target[2];

		float h_offset = max ( w / 8, 8 );

		if ( var::esp::box )
			renderer->draw_outline ( x, y, w, h, 2.f, ImColor { 0, 0, 0 } );

		if ( var::esp::name )
			renderer->draw_text ( x + w / 2, y - m_flFontHeight, cache.m_szName.c_str ( ), { colors::player::name[0], colors::player::name[1], colors::player::name[2] }, true, true );

		if ( var::esp::health )
		{
			const float percentage = health / max_health;
			const float green = percentage * xf ( 255.f );
			const float red = xf ( 255.f ) - green;

			renderer->draw_outline ( x - h_offset, y, 2, h, 2.f, ImColor { 0, 0, 0 } );
			renderer->draw_rectangle ( x - h_offset, y, 2, h, ImColor { 0, 0, 0 } );
			renderer->draw_rectangle ( x - h_offset, y + h - ( h * percentage ), 2, ( h * percentage ), ImColor { (int)red, (int)green, 0, 255 } );
			renderer->draw_text ( x - h_offset - x_offset - 4, y + h - ( h * percentage ) - y_offset / 2, std::to_string ( (int)health ), ImColors::WhiteSmoke, true, false );
		}

		if ( var::esp::box )
			renderer->draw_outline ( x, y, w, h, 1.f, ImColor { bx_r, bx_g, bx_b } );

		if ( !cache.m_szWeaponName.empty ( ) && var::esp::weapon )
		{
			//Ammo m_vecAmmo = weapon->get_bullet_amount ( );

			if ( var::esp::weapon )
				renderer->draw_text ( x + w / 2, y + h + m_flFontHeight, cache.m_szWeaponName.c_str ( ), ImColor { colors::player::weapon[0], colors::player::weapon[1], colors::player::weapon[2] }, true, true );
			if ( var::esp::distance )
				renderer->draw_text ( x + w / 2, y + h + m_flFontHeight * 2, std::to_string ( (int)m_flDistance ) + _ ( "M" ), ImColor { colors::player::distance[0], colors::player::distance[1], colors::player::distance[2] }, true, true );
		}
		else if ( var::esp::distance )
			renderer->draw_text ( x + w / 2, y + h + m_flFontHeight, std::to_string ( (int)m_flDistance ) + _ ( "M" ), ImColor { colors::player::distance[0], colors::player::distance[1], colors::player::distance[2] }, true, true );

		if ( var::esp::snaplines )
		{
			ImVec2 m_scrPosition1 = { monitor::width / 2.f, monitor::height / 1.2f };
			dwl->AddLine ( m_scrPosition1, ImVec2 ( tempFeet.x, tempFeet.y ), ImColor { colors::player::snapline[0], colors::player::snapline[1], colors::player::snapline[2] } );
		}

		if ( m_flDistance > 100.f || !var::esp::skeleton )
			continue;

		auto _neck = player->get_bone_position ( mist::esp.get ( ), BoneList::neck, cache.transform );
		auto spine = player->get_bone_position ( mist::esp.get ( ), BoneList::spine1, cache.transform );
		auto lupper = player->get_bone_position ( mist::esp.get ( ), BoneList::l_upperarm, cache.transform );
		auto rupper = player->get_bone_position ( mist::esp.get ( ), BoneList::r_upperarm, cache.transform );
		auto lfore = player->get_bone_position ( mist::esp.get ( ), BoneList::l_forearm, cache.transform );
		auto rfore = player->get_bone_position ( mist::esp.get ( ), BoneList::r_forearm, cache.transform );
		auto lhand = player->get_bone_position ( mist::esp.get ( ), BoneList::l_hand, cache.transform );
		auto rhand = player->get_bone_position ( mist::esp.get ( ), BoneList::r_hand, cache.transform );
		auto lknee = player->get_bone_position ( mist::esp.get ( ), BoneList::l_knee, cache.transform );
		auto rknee = player->get_bone_position ( mist::esp.get ( ), BoneList::r_knee, cache.transform );
		auto lhip = player->get_bone_position ( mist::esp.get ( ), BoneList::l_hip, cache.transform );
		auto rhip = player->get_bone_position ( mist::esp.get ( ), BoneList::r_hip, cache.transform );

		Vector3 _head = player->get_bone_position ( mist::esp.get ( ), BoneList::head, cache.transform );
		Vector2 chest, l_shoulder, r_shoulder, l_forearm, r_forearm, l_hand, r_hand, neck, l_arm, r_arm, l_knee, r_knee, l_hipbone, r_hipbone;

		if ( !rust::world_to_screen ( _head, head, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( _neck, neck, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( spine, chest, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( lupper, l_shoulder, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( rupper, r_shoulder, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( lfore, l_forearm, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( rfore, r_forearm, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( lhand, l_hand, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( rhand, r_hand, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( lknee, l_knee, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( rknee, r_knee, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( lhip, l_hipbone, m_mxViewMatrix ) )
			continue;
		if ( !rust::world_to_screen ( rhip, r_hipbone, m_mxViewMatrix ) )
			continue;

		dwl->AddLine ( ImVec2 { head.x, head.y }, ImVec2 { neck.x, neck.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { neck.x, neck.y }, ImVec2 { chest.x, chest.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );

		// left 
		dwl->AddLine ( ImVec2 { neck.x, neck.y }, ImVec2 { l_shoulder.x, l_shoulder.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { l_shoulder.x, l_shoulder.y }, ImVec2 { l_forearm.x, l_forearm.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { l_forearm.x, l_forearm.y }, ImVec2 { l_hand.x, l_hand.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );

		dwl->AddLine ( ImVec2 { chest.x, chest.y }, ImVec2 { l_hipbone.x, l_hipbone.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { l_hipbone.x, l_hipbone.y }, ImVec2 { l_knee.x, l_knee.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { l_knee.x, l_knee.y }, ImVec2 { left_foot.x, left_foot.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );

		// right

		dwl->AddLine ( ImVec2 { neck.x, neck.y }, ImVec2 { r_shoulder.x, r_shoulder.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { r_shoulder.x, r_shoulder.y }, ImVec2 { r_forearm.x, r_forearm.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { r_forearm.x, r_forearm.y }, ImVec2 { r_hand.x, r_hand.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );

		dwl->AddLine ( ImVec2 { chest.x, chest.y }, ImVec2 { r_hipbone.x, r_hipbone.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { r_hipbone.x, r_hipbone.y }, ImVec2 { r_knee.x, r_knee.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
		dwl->AddLine ( ImVec2 { r_knee.x, r_knee.y }, ImVec2 { right_foot.x, right_foot.y }, ImColor { colors::player::skeleton[0], colors::player::skeleton[1], colors::player::skeleton[2] }, 1.f );
	}
}
#ifndef GETREKT
void imgui_stuff ( )
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	static const char* items[] = { "HEAD", "NECK", "CHEST", "CLOSEST" };

	ImGui::Begin ( _ ( "catjpghook" ), a, window_flags );
	ImGui::SetWindowSize ( ImVec2 ( 835, 380 ) );

	if ( ImGui::Button ( _ ( "AIM" ), ImVec2 { 100, 20 } ) )
		m_iTab = TAB_AIM;
	ImGui::SameLine ( );
	if ( ImGui::Button ( _ ( "ESP" ), ImVec2 { 100, 20 } ) )
		m_iTab = TAB_ESP;
	ImGui::SameLine ( );
	if ( ImGui::Button ( _ ( "MISC" ), ImVec2 { 100, 20 } ) )
		m_iTab = TAB_MISC;

	if ( m_iTab == TAB_AIM )
	{
		ImGui::Checkbox ( _ ( "Enabled" ), &var::aim::enabled ); ImGui::SameLine ( ); ImGui::Hotkey ( "Aimbot key", &var::aim::aim_key, ImVec2 { 100, 20 } );
		if ( var::aim::enabled )
		{
			ImGui::Combo ( _ ( "Hitbox" ), &var::aim::hitbox, items, IM_ARRAYSIZE ( items ) );
			ImGui::SliderInt ( _ ( "FOV" ), &var::aim::aim_fov, 0, 180 );
			ImGui::Checkbox ( _ ( "Silent" ), &var::aim::silent );
			ImGui::Checkbox ( _ ( "Terrain check" ), &var::aim::terraincheck );
			ImGui::Checkbox ( _ ( "Ignore friends" ), &var::aim::ignore_friends );
			ImGui::SliderFloat ( _ ( "Smoothing" ), &var::aim::smoothing, 0.f, 10.f );
		}
	}
	else if ( m_iTab == TAB_ESP )
	{
		if ( ImGui::Button ( _ ( "PLAYER" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_PLAYER;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "DROPPED" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_DROPPED;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "ORE" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_ORE;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "RADTOWN" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_RAD;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "COLLECTABLE" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_COLLECT;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "VEHICLES" ), ImVec2 { 100, 20 } ) )
			m_iSubTab = TAB_VEHICLES;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "TRAPS" ), ImVec2 { 90, 20 } ) )
			m_iSubTab = TAB_TRAPS;
		ImGui::SameLine ( );

		if ( ImGui::Button ( _ ( "OTHER" ), ImVec2 { 75, 20 } ) )
			m_iSubTab = TAB_OTHER;

		if ( m_iSubTab == TAB_PLAYER )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::players );
			ImGui::Checkbox ( _ ( "Name" ), &var::esp::name ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##nameclr" ), colors::player::name, noob );
			ImGui::Checkbox ( _ ( "Box" ), &var::esp::box ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##boxclr" ), colors::player::box, noob );
			ImGui::Checkbox ( _ ( "Skeleton" ), &var::esp::skeleton ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##skeletonclr" ), colors::player::skeleton, noob );
			ImGui::Checkbox ( _ ( "Snapline" ), &var::esp::snaplines ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##snaplineclr" ), colors::player::snapline, noob );
			ImGui::Checkbox ( _ ( "Health" ), &var::esp::health );
			ImGui::Checkbox ( _ ( "Distance" ), &var::esp::distance ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##distanceclr" ), colors::player::distance, noob );
			ImGui::Checkbox ( _ ( "Weapon" ), &var::esp::weapon ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##weaponclr" ), colors::player::weapon, noob );
			ImGui::Checkbox ( _ ( "Ammo" ), &var::esp::ammo ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##ammoclr" ), colors::player::ammo, noob );
			ImGui::Checkbox ( _ ( "Sleepers" ), &var::esp::sleeper ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##sleeperclr" ), colors::player::sleeper, noob );
			ImGui::Checkbox ( _ ( "NPC" ), &var::esp::npc ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##npcclr" ), colors::player::npc, noob );
			ImGui::Checkbox ( _ ( "Teammates" ), &var::esp::teammate ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##teamclr" ), colors::player::teammate, noob );
			ImGui::Checkbox ( _ ( "Aimbot Target" ), &var::esp::highlight_target ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##targetclr" ), colors::player::target, noob );
		}

		else if ( m_iSubTab == TAB_DROPPED )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::item );
			ImGui::Checkbox ( _ ( "Weapons" ), &var::esp::item_weapon ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##wepclr" ), colors::dropped::weapon, noob );
			ImGui::Checkbox ( _ ( "Corpse" ), &var::esp::item_corpse ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##corpseclr" ), colors::dropped::corpse, noob );
			ImGui::Checkbox ( _ ( "Other" ), &var::esp::item_other ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##otherclr" ), colors::dropped::other, noob );
		}

		else if ( m_iSubTab == TAB_ORE )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::ores );
			ImGui::Checkbox ( _ ( "Stone" ), &var::esp::stone ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##stoneclr" ), colors::ore::stone, noob );
			ImGui::Checkbox ( _ ( "Sulfur" ), &var::esp::sulfur ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##sulfurclr" ), colors::ore::sulfur, noob );
			ImGui::Checkbox ( _ ( "Metal" ), &var::esp::metal ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##metalclr" ), colors::ore::metal, noob );
		}

		else if ( m_iSubTab == TAB_COLLECT )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::collectable );
			ImGui::Checkbox ( _ ( "Hemp" ), &var::esp::hemp ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##hempclr" ), colors::collectable::hemp, noob );
			ImGui::Checkbox ( _ ( "Mushroom" ), &var::esp::mushroom ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##mushroomclr" ), colors::collectable::mushroom, noob );
			ImGui::Checkbox ( _ ( "Corn" ), &var::esp::corn ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cornclr" ), colors::collectable::corn, noob );
			ImGui::Checkbox ( _ ( "Berry" ), &var::esp::berry ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##berryclr" ), colors::collectable::berry, noob );
			ImGui::Checkbox ( _ ( "Potato" ), &var::esp::potato ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##potatoclr" ), colors::collectable::potato, noob );
			ImGui::Checkbox ( _ ( "Sulfur" ), &var::esp::csulfur ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##csulfurclr" ), colors::collectable::sulfur, noob );
			ImGui::Checkbox ( _ ( "Stone" ), &var::esp::cstone ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cstoneclr" ), colors::collectable::stone, noob );
			ImGui::Checkbox ( _ ( "Metal" ), &var::esp::cmetal ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cmetalclr" ), colors::collectable::metal, noob );
			ImGui::Checkbox ( _ ( "Wood" ), &var::esp::cwood ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cwoodclr" ), colors::collectable::wood, noob );

		}

		else if ( m_iSubTab == TAB_RAD )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::radtown );
			if ( var::esp::radtown )
			{
				ImGui::Checkbox ( _ ( "Barrels" ), &var::esp::barrel ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##barrelsclr" ), colors::radtown::barrel, noob );
				ImGui::Checkbox ( _ ( "Crates" ), &var::esp::crate ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cratesclr" ), colors::radtown::crate, noob );
				ImGui::Checkbox ( _ ( "Military crates" ), &var::esp::mcrate ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##militaryclr" ), colors::radtown::mcrate, noob );
				ImGui::Checkbox ( _ ( "Elite crates" ), &var::esp::ecrate ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##eliteclr" ), colors::radtown::ecrate, noob );
				ImGui::Checkbox ( _ ( "Toolboxes" ), &var::esp::toolbox ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##toolboxesclr" ), colors::radtown::toolbox, noob );
				ImGui::Checkbox ( _ ( "Minecarts" ), &var::esp::minecart ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##minecartsclr" ), colors::radtown::minecart, noob );
				ImGui::Checkbox ( _ ( "Oil barrels" ), &var::esp::oil ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##oilclr" ), colors::radtown::oil, noob );
			}
		}

		else if ( m_iSubTab == TAB_VEHICLES )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::vehicles );
			if ( var::esp::vehicles )
			{
				ImGui::Checkbox ( _ ( "Horses" ), &var::esp::vehicles_horse ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##horseclr" ), colors::vehicles::horse, noob );
				ImGui::Checkbox ( _ ( "Boats" ), &var::esp::vehicles_boats ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##boatclr" ), colors::vehicles::boat, noob );
				ImGui::Checkbox ( _ ( "Helicopters" ), &var::esp::vehicles_helicopters ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##heliclr" ), colors::vehicles::helicopter, noob );
				ImGui::Checkbox ( _ ( "Special" ), &var::esp::special ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##specialclr" ), colors::vehicles::special, noob );
			}
		}

		else if ( m_iSubTab == TAB_TRAPS )
		{
			ImGui::Checkbox ( _ ( "Enabled" ), &var::esp::traps );
			if ( var::esp::traps )
			{
				ImGui::Checkbox ( _ ( "Auto-turret" ), &var::esp::autoturret ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##autotclr" ), colors::traps::autoturret, noob );
				ImGui::Checkbox ( _ ( "Flame-turret" ), &var::esp::flameturret ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##flamclr" ), colors::traps::flameturret, noob );
				ImGui::Checkbox ( _ ( "Shotgun-trap" ), &var::esp::shotguntrap ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##shotgclr" ), colors::traps::guntrap, noob );
				ImGui::Checkbox ( _ ( "Land-mine" ), &var::esp::landmine ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##landmclr" ), colors::traps::landmine, noob );
				ImGui::Checkbox ( _ ( "Bear-trap" ), &var::esp::beartrap ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##beartrclr" ), colors::traps::beartrap, noob );
				ImGui::Checkbox ( _ ( "SAM-site" ), &var::esp::samsite ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##samclr" ), colors::traps::samsite, noob );
			}
		}

		else if ( m_iSubTab == TAB_OTHER )
		{
			ImGui::Checkbox ( _ ( "ESP Enabled" ), &var::esp::enabled );
			ImGui::Checkbox ( _ ( "Hotbar" ), &var::esp::item_info );
			ImGui::Checkbox ( _ ( "Raid" ), &var::esp::raid ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##raidclr" ), colors::other::raid, noob );
			ImGui::Checkbox ( _ ( "Animals" ), &var::esp::animals ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##animalclr" ), colors::other::animal, noob );
			ImGui::Checkbox ( _ ( "Stashes" ), &var::esp::stash ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##stashclr" ), colors::other::stash, noob );
			ImGui::Checkbox ( _ ( "Tool cupboard" ), &var::esp::toolcupboard ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##cupbclr" ), colors::other::toolcupboard, noob );
			if ( var::esp::toolcupboard )
				ImGui::Checkbox ( _ ( "Tool cupboard list" ), &var::esp::toolcupboard_list );

			ImGui::Checkbox ( _ ( "Sleeping bag" ), &var::esp::sleepingbag ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##sleepclr" ), colors::other::sleepingbag, noob );
			ImGui::Checkbox ( _ ( "FOV Circle" ), &var::esp::fov_circle ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##fovclr" ), colors::other::fov, noob );
			ImGui::Checkbox ( _ ( "Crosshair" ), &var::esp::crosshair ); ImGui::SameLine ( ); ImGui::ColorEdit3 ( _ ( "##crossclr" ), colors::other::crosshair, noob );
			ImGui::SliderFloat ( _ ( "Global distance limit" ), &var::esp::distance_limit, 1.f, 4000.f );
			ImGui::Checkbox ( _ ( "Hide in menu" ), &var::esp::hide );
			ImGui::Checkbox ( _ ( "VSync" ), &vsync );
		}
	}
	else if ( m_iTab == TAB_MISC )
	{
		if ( ImGui::Button ( "Load config" ) )
			load_config ( );
		ImGui::SameLine ( );
		if ( ImGui::Button ( "Save config" ) )
			save_config ( );
		ImGui::Checkbox ( _ ( "No-spread" ), &var::misc::nospread );
		if ( var::misc::nospread )
			ImGui::Checkbox ( _ ( "Very accurate no-spread" ), &var::misc::noshotgunspread );
		ImGui::Checkbox ( _ ( "Spider-man" ), &var::misc::spider );
		ImGui::Checkbox ( _ ( "Instant eoka" ), &var::misc::eoka );
		ImGui::Checkbox ( _ ( "Instant compound bow" ), &var::misc::compound );
		ImGui::Checkbox ( _ ( "Fullbright (off)" ), &var::misc::fullbright );
		ImGui::Checkbox ( _ ( "Always day" ), &var::misc::alwaysday );
		ImGui::Checkbox ( _ ( "Always night" ), &var::misc::alwaysnight );
		ImGui::Checkbox ( _ ( "No-slide" ), &var::misc::noslide );
		ImGui::Checkbox ( _ ( "Fakelag" ), &var::misc::fakelag );
		ImGui::Checkbox ( _ ( "Admin flag" ), &var::misc::admin );
		ImGui::Checkbox ( _ ( "Radar" ), &var::misc::radar );

		ImGui::Checkbox ( _ ( "Low Gravity" ), &var::misc::bgravity );
		if ( var::misc::bgravity )
			ImGui::Hotkey ( _ ( "##Lowgravity" ), &var::misc::gravity, ImVec2 { 100.f, 20.f } );
		ImGui::Checkbox ( _ ( "No-clip" ), &var::misc::bfly );
		if ( var::misc::bfly )
			ImGui::Hotkey ( _ ( "##Noclip" ), &var::misc::fly, ImVec2 { 100.f, 20.f } );
		if ( var::misc::fly != 0 )
		{
			ImGui::Checkbox ( _ ( "Climb assist" ), &var::misc::bclimb );
			ImGui::Hotkey ( _ ( "##Climbassist" ), &var::misc::assist, ImVec2 { 100.f, 20.f } );
		}
		ImGui::Checkbox ( _ ( "Timescale" ), &var::misc::btimescale );
		if ( var::misc::btimescale )
			ImGui::Hotkey ( _ ( "##Timescale" ), &var::misc::timescale, ImVec2 { 100.f, 20.f } );
		ImGui::Checkbox ( _ ( "Fake-duck" ), &var::misc::bview_offset );
		if ( var::misc::bview_offset )
			ImGui::Hotkey ( _ ( "##Fakeduck" ), &var::misc::view_offset, ImVec2 { 100.f, 20.f } );
		if ( var::misc::radar )
			ImGui::SliderInt ( _ ( "Radar scale" ), &var::misc::scale, 0, 100 );
		ImGui::SliderFloat ( _ ( "Recoil X" ), &var::misc::recoil_pitch, 0.1f, 100.f );
		ImGui::SliderFloat ( _ ( "Recoil Y" ), &var::misc::recoil_yaw, 0.1f, 100.f );
		if ( var::misc::view_offset != 0 )
		{
			ImGui::SliderFloat ( _ ( "Fake duck X offset" ), &var::misc::x, -300.f, 300.f );
			ImGui::SliderFloat ( _ ( "Fake duck Y offset" ), &var::misc::y, -300.f, 300.f );
			ImGui::SliderFloat ( _ ( "Fake duck Z offset" ), &var::misc::z, -100.f, 100.f );
		}
		if ( var::misc::timescale != 0 )
			ImGui::SliderFloat ( _ ( "Timescale speed" ), &var::misc::timescale_speed, 1.f, 20.f );
		if ( var::misc::fly != 0 )
			ImGui::SliderFloat ( _ ( "Noclip speed" ), &var::misc::noclip_speed, 10.f, 100.f );
		ImGui::SliderFloat ( _ ( "Field of View" ), &var::misc::fov, 10.f, 180.f );

		if ( var::misc::alwaysnight )
			var::misc::alwaysday = false;
	}
	ImGui::End ( );
}
#endif
std::string GetSystemFontFile ( const std::string& faceName ) {

	static LPCWSTR fontRegistryPath = _ ( L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" );
	HKEY hKey;
	LONG result;
	std::wstring wsFaceName ( faceName.begin ( ), faceName.end ( ) );

	// Open Windows font registry key
	result = RegOpenKeyExW ( HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey );
	if ( result != ERROR_SUCCESS ) {
		return "";
	}

	DWORD maxValueNameSize, maxValueDataSize;
	result = RegQueryInfoKeyW ( hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0 );
	if ( result != ERROR_SUCCESS ) {
		return "";
	}

	DWORD valueIndex = 0;
	LPWSTR valueName = new WCHAR[maxValueNameSize];
	LPBYTE valueData = new BYTE[maxValueDataSize];
	DWORD valueNameSize, valueDataSize, valueType;
	std::wstring wsFontFile;

	// Look for a matching font name
	do {

		wsFontFile.clear ( );
		valueDataSize = maxValueDataSize;
		valueNameSize = maxValueNameSize;

		result = RegEnumValueW ( hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize );

		valueIndex++;

		if ( result != ERROR_SUCCESS || valueType != REG_SZ ) {
			continue;
		}

		std::wstring wsValueName ( valueName, valueNameSize );

		// Found a match
		if ( _wcsnicmp ( wsFaceName.c_str ( ), wsValueName.c_str ( ), wsFaceName.length ( ) ) == 0 ) {

			wsFontFile.assign ( (LPWSTR)valueData, valueDataSize );
			break;
		}
	} while ( result != ERROR_NO_MORE_ITEMS );

	delete[] valueName;
	delete[] valueData;

	RegCloseKey ( hKey );

	if ( wsFontFile.empty ( ) ) {
		return "";
	}

	// Build full font file path
	WCHAR winDir[MAX_PATH];
	HRESULT ret = GetWindowsDirectoryW ( winDir, MAX_PATH );

	if ( ret == 0 )
	{
		MessageBoxW ( GetActiveWindow ( ), _ ( L"Error while getting fonts!" ), _ ( L"Error" ), MB_OK | MB_ICONQUESTION );
	}

	std::wstringstream ss;
	ss << winDir << _ ( "\\Fonts\\" ) << wsFontFile;
	wsFontFile = ss.str ( );

	return std::string ( wsFontFile.begin ( ), wsFontFile.end ( ) );
}

inline void copy_clr ( float ( &x )[3], std::vector<float> ( y ) )
{
	std::copy ( std::begin ( y ), std::end ( y ), x );
}

bool load_config ( )
{
	/* load file */
#ifdef GETREKT
	std::filesystem::path path { "C:\\GETREKTRUST" };
#else
	std::filesystem::path path { "C:\\CATJPG" };
#endif
	path /= "config.json";
	std::filesystem::create_directories ( path.parent_path ( ) );

	std::ifstream fcfg ( path );
	std::string scfg ( ( std::istreambuf_iterator<char> ( fcfg ) ), std::istreambuf_iterator<char> ( ) );
	fcfg.close ( );

	if ( scfg.empty ( ) || scfg.length ( ) < 100 )
		return false;

	json cfg = json::parse ( scfg );

	// cfg["aim"]
	try {
		var::aim::enabled = cfg["aim"]["enabled"];
		var::aim::aim_key = cfg["aim"]["aim_key"];
		var::aim::hitbox = cfg["aim"]["hitbox"];
		var::aim::aim_fov = cfg["aim"]["fov"];
		var::aim::silent = cfg["aim"]["silent"];
		var::aim::terraincheck = cfg["aim"]["terrain"];
		var::aim::ignore_friends = cfg["aim"]["ignore_friend"];
		var::aim::smoothing = cfg["aim"]["smoothing"];
		// cfg["esp"]["player"]
		var::esp::players = cfg["esp"]["player"]["enabled"];
		var::esp::name = cfg["esp"]["player"]["name"];
		copy_clr ( colors::player::name, cfg["esp"]["player"]["nameclr"] );
		var::esp::box = cfg["esp"]["player"]["box"];
		copy_clr ( colors::player::box, cfg["esp"]["player"]["boxclr"] );
		var::esp::skeleton = cfg["esp"]["player"]["skeleton"];
		copy_clr ( colors::player::skeleton, cfg["esp"]["player"]["skeletonclr"] );
		var::esp::snaplines = cfg["esp"]["player"]["snapline"];
		copy_clr ( colors::player::snapline, cfg["esp"]["player"]["snaplineclr"] );
		var::esp::health = cfg["esp"]["player"]["health"];
		var::esp::distance = cfg["esp"]["player"]["distance"];
		copy_clr ( colors::player::distance, cfg["esp"]["player"]["distanceclr"] );
		var::esp::weapon = cfg["esp"]["player"]["weapon"];
		copy_clr ( colors::player::weapon, cfg["esp"]["player"]["weaponclr"] );
		var::esp::ammo = cfg["esp"]["player"]["ammo"];
		copy_clr ( colors::player::ammo, cfg["esp"]["player"]["ammoclr"] );
		var::esp::sleeper = cfg["esp"]["player"]["sleepers"];
		copy_clr ( colors::player::sleeper, cfg["esp"]["player"]["sleeperclr"] );
		var::esp::npc = cfg["esp"]["player"]["npc"];
		copy_clr ( colors::player::npc, cfg["esp"]["player"]["npcclr"] );
		// cfg["esp"]["items"]
		var::esp::item = cfg["esp"]["items"]["enabled"];
		var::esp::item_weapon = cfg["esp"]["items"]["weapon"];
		copy_clr ( colors::dropped::weapon, cfg["esp"]["items"]["weaponclr"] );
		var::esp::item_corpse = cfg["esp"]["items"]["corpse"];
		copy_clr ( colors::dropped::corpse, cfg["esp"]["items"]["corpseclr"] );
		var::esp::item_other = cfg["esp"]["items"]["other"];
		copy_clr ( colors::dropped::other, cfg["esp"]["items"]["otherclr"] );

		// cfg["esp"]["ore"]
		var::esp::ores = cfg["esp"]["ore"]["enabled"];
		var::esp::stone = cfg["esp"]["ore"]["stone"];
		copy_clr ( colors::ore::stone, cfg["esp"]["ore"]["stoneclr"] );
		var::esp::stone = cfg["esp"]["ore"]["sulfur"];
		copy_clr ( colors::ore::stone, cfg["esp"]["ore"]["sulfurclr"] );
		var::esp::stone = cfg["esp"]["ore"]["metal"];
		copy_clr ( colors::ore::stone, cfg["esp"]["ore"]["metalclr"] );

		// cfg["esp"]["collectables"]
		var::esp::collectable = cfg["esp"]["collectables"]["enabled"];
		var::esp::hemp = cfg["esp"]["collectables"]["hemp"];
		copy_clr ( colors::collectable::hemp, cfg["esp"]["collectables"]["hempclr"] );
		var::esp::mushroom = cfg["esp"]["collectables"]["mushroom"];
		copy_clr ( colors::collectable::mushroom, cfg["esp"]["collectables"]["mushroomclr"] );
		var::esp::corn = cfg["esp"]["collectables"]["corn"];
		copy_clr ( colors::collectable::corn, cfg["esp"]["collectables"]["cornclr"] );
		var::esp::berry = cfg["esp"]["collectables"]["berry"];
		copy_clr ( colors::collectable::berry, cfg["esp"]["collectables"]["berryclr"] );
		var::esp::potato = cfg["esp"]["collectables"]["potato"];
		copy_clr ( colors::collectable::potato, cfg["esp"]["collectables"]["potatoclr"] );
		var::esp::csulfur = cfg["esp"]["collectables"]["sulfur"];
		copy_clr ( colors::collectable::sulfur, cfg["esp"]["collectables"]["csulfurclr"] );
		var::esp::cstone = cfg["esp"]["collectables"]["stone"];
		copy_clr ( colors::collectable::stone, cfg["esp"]["collectables"]["cstoneclr"] );
		var::esp::cmetal = cfg["esp"]["collectables"]["metal"];
		copy_clr ( colors::collectable::metal, cfg["esp"]["collectables"]["cmetalclr"] );
		var::esp::cwood = cfg["esp"]["collectables"]["wood"];
		copy_clr ( colors::collectable::wood, cfg["esp"]["collectables"]["woodclr"] );

		// cfg["esp"]["radtown"]
		var::esp::radtown = cfg["esp"]["radtown"]["enabled"];
		var::esp::barrel = cfg["esp"]["radtown"]["barrels"];
		copy_clr ( colors::radtown::barrel, cfg["esp"]["radtown"]["barrelsclr"] );
		var::esp::crate = cfg["esp"]["radtown"]["crates"];
		copy_clr ( colors::radtown::crate, cfg["esp"]["radtown"]["cratesclr"] );
		var::esp::mcrate = cfg["esp"]["radtown"]["military"];
		copy_clr ( colors::radtown::mcrate, cfg["esp"]["radtown"]["militaryclr"] );
		var::esp::ecrate = cfg["esp"]["radtown"]["elite"];
		copy_clr ( colors::radtown::ecrate, cfg["esp"]["radtown"]["eliteclr"] );
		var::esp::toolbox = cfg["esp"]["radtown"]["toolboxes"];
		copy_clr ( colors::radtown::toolbox, cfg["esp"]["radtown"]["toolboxesclr"] );
		var::esp::minecart = cfg["esp"]["radtown"]["minecarts"];
		copy_clr ( colors::radtown::minecart, cfg["esp"]["radtown"]["minecartsclr"] );
		var::esp::oil = cfg["esp"]["radtown"]["oil"];
		copy_clr ( colors::radtown::oil, cfg["esp"]["radtown"]["oilclr"] );

		// cfg["esp"]["vehicles"]
		var::esp::vehicles = cfg["esp"]["vehicles"]["enabled"];
		var::esp::vehicles_horse = cfg["esp"]["vehicles"]["horses"];
		copy_clr ( colors::vehicles::horse, cfg["esp"]["vehicles"]["horseclr"] );
		var::esp::vehicles_boats = cfg["esp"]["vehicles"]["boats"];
		copy_clr ( colors::vehicles::boat, cfg["esp"]["vehicles"]["boatclr"] );
		var::esp::vehicles_helicopters = cfg["esp"]["vehicles"]["helicopters"];
		copy_clr ( colors::vehicles::helicopter, cfg["esp"]["vehicles"]["heliclr"] );
		var::esp::special = cfg["esp"]["vehicles"]["special"];
		copy_clr ( colors::vehicles::special, cfg["esp"]["vehicles"]["specialclr"] );

		// cfg["esp"]["traps"]
		var::esp::traps = cfg["esp"]["traps"]["enabled"];
		var::esp::autoturret = cfg["esp"]["traps"]["autoturret"];
		copy_clr ( colors::traps::autoturret, cfg["esp"]["traps"]["autotclr"] );
		var::esp::flameturret = cfg["esp"]["traps"]["flameturret"];
		copy_clr ( colors::traps::flameturret, cfg["esp"]["traps"]["flamclr"] );
		var::esp::shotguntrap = cfg["esp"]["traps"]["shotguntrap"];
		copy_clr ( colors::traps::guntrap, cfg["esp"]["traps"]["shotgclr"] );
		var::esp::landmine = cfg["esp"]["traps"]["landmine"];
		copy_clr ( colors::traps::landmine, cfg["esp"]["traps"]["landmclr"] );
		var::esp::beartrap = cfg["esp"]["traps"]["beartrap"];
		copy_clr ( colors::traps::beartrap, cfg["esp"]["traps"]["beartrclr"] );
		var::esp::samsite = cfg["esp"]["traps"]["samsite"];
		copy_clr ( colors::traps::samsite, cfg["esp"]["traps"]["samclr"] );

		// cfg["esp"]["other"]
		var::esp::enabled = cfg["esp"]["other"]["enabled"];
		var::esp::item_info = cfg["esp"]["other"]["hotbar"];
		var::esp::raid = cfg["esp"]["other"]["raid"];
		copy_clr ( colors::other::raid, cfg["esp"]["other"]["raidclr"] );
		var::esp::animals = cfg["esp"]["other"]["animals"];
		copy_clr ( colors::other::animal, cfg["esp"]["other"]["animalclr"] );
		var::esp::stash = cfg["esp"]["other"]["stashes"];
		copy_clr ( colors::other::stash, cfg["esp"]["other"]["stashclr"] );
		var::esp::toolcupboard = cfg["esp"]["other"]["cupboard"];
		copy_clr ( colors::other::toolcupboard, cfg["esp"]["other"]["cupboardclr"] );
		var::esp::toolcupboard_list, cfg["esp"]["other"]["cupboard_list"];
		var::esp::sleepingbag, cfg["esp"]["other"]["bag"];
		copy_clr ( colors::other::sleepingbag, cfg["esp"]["other"]["bagclr"] );
		var::esp::fov_circle = cfg["esp"]["other"]["fov"];
		copy_clr ( colors::other::fov, cfg["esp"]["other"]["fovclr"] );
		var::esp::crosshair = cfg["esp"]["other"]["crosshair"];
		copy_clr ( colors::other::crosshair, cfg["esp"]["other"]["crosshairclr"] );
		var::esp::distance_limit = cfg["esp"]["other"]["distance"];
		var::esp::hide = cfg["esp"]["other"]["hide"];
		vsync = cfg["esp"]["other"]["vsync"];

		// cfg["misc"]
		var::misc::nospread = cfg["misc"]["nospread"];
		var::misc::noshotgunspread = cfg["misc"]["shotgunspread"];
		var::misc::spider = cfg["misc"]["spider"];
		var::misc::eoka = cfg["misc"]["eoka"];
		var::misc::compound = cfg["misc"]["compound"];
		var::misc::fullbright = cfg["misc"]["fullbright"];
		var::misc::alwaysday = cfg["misc"]["day"];
		var::misc::alwaysnight = cfg["misc"]["night"];
		var::misc::noslide = cfg["misc"]["slide"];
		var::misc::fakelag = cfg["misc"]["fakelag"];
		var::misc::admin = cfg["misc"]["admin"];
		var::misc::radar = cfg["misc"]["radar"];
		var::misc::thickness = cfg["misc"]["thickness"];
		var::misc::flthickness = cfg["misc"]["flthickness"];
		var::misc::gravity = cfg["misc"]["gravity"];
		var::misc::fly = cfg["misc"]["noclip"];
		var::misc::assist = cfg["misc"]["climb"];
		var::misc::timescale = cfg["misc"]["timescale"];
		var::misc::view_offset = cfg["misc"]["fakeduck"];
		var::misc::scale = cfg["misc"]["radar_scale"];
		var::misc::recoil_pitch = cfg["misc"]["rx"];
		var::misc::recoil_yaw = cfg["misc"]["ry"];
		var::misc::x = cfg["misc"]["fx"];
		var::misc::y = cfg["misc"]["fy"];
		var::misc::z = cfg["misc"]["fz"];
		var::misc::timescale_speed = cfg["misc"]["timescale_speed"];
		var::misc::noclip_speed = cfg["misc"]["noclip_speed"];
		var::misc::fov = cfg["misc"]["fov"];

		var::esp::teammate = cfg["esp"]["player"]["teammate"];
		copy_clr ( colors::player::teammate, cfg["esp"]["player"]["teamclr"] );
		var::esp::highlight_target = cfg["esp"]["player"]["target"];
		copy_clr ( colors::player::target, cfg["esp"]["player"]["targetclr"] );
	}
	catch ( ... )
	{

	}
	Beep ( 242, 100 );
	return true;
}

bool save_config ( )
{
#ifdef GETREKT
	std::filesystem::path path { "C:\\GETREKTRUST" };
#else
	std::filesystem::path path { "C:\\CATJPG" };
#endif
	path /= "config.json";
	std::filesystem::create_directories ( path.parent_path ( ) );

	std::ofstream fcfg ( path );

	json cfg;
	cfg["aim"] = {
		{ "enabled", var::aim::enabled },
		{ "aim_key", var::aim::aim_key },
		{ "hitbox", var::aim::hitbox },
		{ "fov", var::aim::aim_fov },
		{ "silent", var::aim::silent },
		{ "terrain", var::aim::terraincheck },
		{ "ignore_friend", var::aim::ignore_friends },
		{ "smoothing", var::aim::smoothing }
	};

	cfg["esp"]["player"] = {
		{ "enabled", var::esp::players },
		{ "name", var::esp::name }, { "nameclr", colors::player::name },
		{ "box", var::esp::box }, { "boxclr", colors::player::box },
		{ "skeleton", var::esp::skeleton }, { "skeletonclr", colors::player::skeleton },
		{ "snapline", var::esp::snaplines }, { "snaplineclr", colors::player::snapline },
		{ "health", var::esp::health },
		{ "distance", var::esp::distance }, { "distanceclr", colors::player::distance },
		{ "weapon", var::esp::weapon }, { "weaponclr", colors::player::weapon },
		{ "ammo", var::esp::ammo }, { "ammoclr", colors::player::ammo },
		{ "sleepers", var::esp::sleeper }, { "sleeperclr", colors::player::sleeper },
		{ "npc", var::esp::npc }, { "npcclr", colors::player::npc },
		{ "teammate", var::esp::teammate }, { "teamclr", colors::player::teammate },
		{ "target", var::esp::highlight_target }, { "targetclr", colors::player::target }
	};

	cfg["esp"]["items"] = {
		{ "enabled", var::esp::item },
		{ "weapon", var::esp::item_weapon }, { "weaponclr", colors::dropped::weapon },
		{ "corpse", var::esp::item_corpse }, { "corpseclr", colors::dropped::corpse },
		{ "other", var::esp::item_other }, { "otherclr", colors::dropped::other }
	};

	cfg["esp"]["ore"] = {
		{ "enabled", var::esp::ores },
		{ "stone", var::esp::stone }, { "stoneclr", colors::ore::stone },
		{ "sulfur", var::esp::stone }, { "sulfurclr", colors::ore::stone },
		{ "metal", var::esp::stone }, { "metalclr", colors::ore::stone },
	};

	cfg["esp"]["collectables"] = {
		{ "enabled", var::esp::collectable },
		{ "hemp", var::esp::hemp }, { "hempclr", colors::collectable::hemp },
		{ "mushroom", var::esp::mushroom }, { "mushroomclr", colors::collectable::mushroom },
		{ "corn", var::esp::corn }, { "cornclr", colors::collectable::corn },
		{ "berry", var::esp::berry }, { "berryclr", colors::collectable::berry },
		{ "potato", var::esp::potato }, { "potatoclr", colors::collectable::potato },
		{ "sulfur", var::esp::csulfur }, { "csulfurclr", colors::collectable::sulfur },
		{ "stone", var::esp::cstone }, { "cstoneclr", colors::collectable::stone },
		{ "metal", var::esp::cmetal }, { "cmetalclr", colors::collectable::metal },
		{ "wood", var::esp::cwood }, { "woodclr", colors::collectable::wood },
	};

	cfg["esp"]["radtown"] = {
		{ "enabled", var::esp::radtown },
		{ "barrels", var::esp::barrel }, { "barrelsclr", colors::radtown::barrel },
		{ "crates", var::esp::crate }, { "cratesclr", colors::radtown::crate },
		{ "military", var::esp::mcrate }, { "militaryclr", colors::radtown::mcrate },
		{ "elite", var::esp::ecrate }, { "eliteclr", colors::radtown::ecrate },
		{ "toolboxes", var::esp::toolbox }, { "toolboxesclr", colors::radtown::toolbox },
		{ "minecarts", var::esp::minecart }, { "minecartsclr", colors::radtown::minecart },
		{ "oil", var::esp::oil }, { "oilclr", colors::radtown::oil },
	};

	cfg["esp"]["vehicles"] = {
		{ "enabled", var::esp::vehicles },
		{ "horses", var::esp::vehicles_horse }, { "horseclr", colors::vehicles::horse },
		{ "boats", var::esp::vehicles_boats }, { "boatclr", colors::vehicles::boat },
		{ "helicopters", var::esp::vehicles_helicopters }, { "heliclr", colors::vehicles::helicopter },
		{ "special", var::esp::special }, { "specialclr", colors::vehicles::special },
	};

	cfg["esp"]["traps"] = {
		{ "enabled", var::esp::traps },
		{ "autoturret", var::esp::autoturret }, { "autotclr", colors::traps::autoturret },
		{ "flameturret", var::esp::flameturret }, { "flamclr", colors::traps::flameturret },
		{ "shotguntrap", var::esp::shotguntrap }, { "shotgclr", colors::traps::guntrap },
		{ "landmine", var::esp::landmine }, { "landmclr", colors::traps::landmine },
		{ "beartrap", var::esp::beartrap }, { "beartrclr", colors::traps::beartrap },
		{ "samsite", var::esp::samsite }, { "samclr", colors::traps::samsite },
	};

	cfg["esp"]["other"] = {
		{ "enabled", var::esp::enabled },
		{ "hotbar", var::esp::item_info },
		{ "raid", var::esp::raid }, { "raidclr", colors::other::raid },
		{ "animals", var::esp::animals }, { "animalclr", colors::other::animal },
		{ "stashes", var::esp::stash }, { "stashclr", colors::other::stash },
		{ "cupboard", var::esp::toolcupboard }, { "cupboardclr", colors::other::toolcupboard },
		{ "cupboard_list", var::esp::toolcupboard_list },
		{ "bag", var::esp::sleepingbag }, { "bagclr", colors::other::sleepingbag },
		{ "fov", var::esp::fov_circle }, { "fovclr", colors::other::fov },
		{ "crosshair", var::esp::crosshair }, { "crosshairclr", colors::other::crosshair },
		{ "distance", var::esp::distance_limit },
		{ "hide", var::esp::hide },
		{ "vsync", vsync }
	};

	cfg["misc"] = {
		{ "nospread", var::misc::nospread },
		{ "shotgunspread", var::misc::noshotgunspread },
		{ "spider", var::misc::spider },
		{ "eoka", var::misc::eoka },
		{ "compound", var::misc::compound },
		{ "fullbright", var::misc::fullbright },
		{ "day", var::misc::alwaysday },
		{ "night", var::misc::alwaysnight },
		{ "slide", var::misc::noslide },
		{ "fakelag", var::misc::fakelag },
		{ "admin", var::misc::admin },
		{ "radar", var::misc::radar },
		{ "thickness", var::misc::thickness },
		{ "flthickness", var::misc::flthickness },
		{ "gravity", var::misc::gravity },
		{ "noclip", var::misc::fly },
		{ "climb", var::misc::assist },
		{ "timescale", var::misc::timescale },
		{ "fakeduck", var::misc::view_offset },
		{ "radar_scale", var::misc::scale },
		{ "rx", var::misc::recoil_pitch },
		{ "ry", var::misc::recoil_yaw },
		{ "fx", var::misc::x },
		{ "fy", var::misc::y },
		{ "fz", var::misc::z },
		{ "timescale_speed", var::misc::timescale_speed },
		{ "noclip_speed", var::misc::noclip_speed },
		{ "fov", var::misc::fov }
	};

	fcfg << cfg;
	fcfg.close ( );

	Beep ( 242, 100 );

	return true;
}
#ifdef GETREKT
namespace GetRekt {
	void style_update ( )
	{
		ImGuiStyle& style = ImGui::GetStyle ( );

		button = ImVec4 ( 0.98f, 0.98f, 0.98f, 1.00f );
		buttonhovered = ImVec4 ( 0.24f, 0.24f, 0.24f, 1.00f );
		buttonActive = ImVec4 ( 0.26f, 0.26f, 0.26f, 1.00f );
		text = Color_to_vec ( g_Options.color_text );
		themecolor = Color_to_vec ( g_Options.color_theme );
		bg_color = Color_to_vec ( g_Options.color_bg );
		bg_child_color = Color_to_vec ( g_Options.color_bg_child );
		separator = ImVec4 ( 0.39f, 0.39f, 0.39f, 1.00f );

		style.Colors[ImGuiCol_Themecolor] = themecolor;
		style.Colors[ImGuiCol_Text] = text;
		style.Colors[ImGuiCol_TextDisabled] = ImVec4 ( 0.44f, 0.44f, 0.44f, 1.00f );
		style.Colors[ImGuiCol_WindowBg] = ImVec4 ( 0.00f, 0.00f, 0.00f, 0.00f );
		style.Colors[ImGuiCol_ChildWindowBg] = bg_child_color;//ImVec4(0.89f, 0.890f, 0.89f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4 ( bg_color.x, bg_color.y, bg_color.z, 0.98f * bg_color.w );
		style.Colors[ImGuiCol_Border] = ImVec4 ( 0.00f, 0.00f, 0.00f, 0.40f );
		style.Colors[ImGuiCol_BorderShadow] = ImVec4 ( 0.00f, 0.00f, 0.00f, 0.00f );
		style.Colors[ImGuiCol_FrameBg] = bg_child_color;//ImVec4(0.76f, 0.76f, 0.76f, 1.f);//ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 ( themecolor.x, themecolor.y, themecolor.z, 0.40f * themecolor.w );
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4 ( 0.26f, 0.59f, 0.98f, 0.67f );
		style.Colors[ImGuiCol_TitleBg] = bg_child_color;//ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4 ( 0.82f, 0.82f, 0.82f, 1.00f );
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4 ( 1.00f, 1.00f, 1.00f, 0.51f );
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4 ( 0.86f, 0.86f, 0.86f, 1.00f );
		style.Colors[ImGuiCol_ScrollbarBg] = bg_color;//ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = slider; //ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 ( 0.49f, 0.49f, 0.49f, 0.80f );
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4 ( 0.49f, 0.49f, 0.49f, 1.00f );
		style.Colors[ImGuiCol_CheckMark] = themecolor;//ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4 ( 0.26f, 0.59f, 0.98f, 0.78f );
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4 ( 0.46f, 0.54f, 0.80f, 0.60f );
		style.Colors[ImGuiCol_Button] = button;
		style.Colors[ImGuiCol_ButtonHovered] = buttonhovered;
		style.Colors[ImGuiCol_ButtonActive] = buttonActive;//ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4 ( 0.05f, 0.67f, 0.91f, 0.31f );
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4 ( 0.23f, 0.74f, 0.88f, 0.80f );
		style.Colors[ImGuiCol_HeaderActive] = ImVec4 ( 0.09f, 0.66f, 0.93f, 0.53f );
		style.Colors[ImGuiCol_Separator] = separator;
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4 ( 0.14f, 0.44f, 0.80f, 0.78f );
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4 ( 0.14f, 0.44f, 0.80f, 1.00f );
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4 ( 0.80f, 0.80f, 0.80f, 0.56f );
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4 ( 0.26f, 0.59f, 0.98f, 0.67f );
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4 ( 0.26f, 0.59f, 0.98f, 0.95f );
		style.Colors[ImGuiCol_PlotLines] = ImVec4 ( 0.39f, 0.39f, 0.39f, 1.00f );
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4 ( 1.00f, 0.43f, 0.35f, 1.00f );
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4 ( 0.90f, 0.70f, 0.00f, 1.00f );
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4 ( 1.00f, 0.45f, 0.00f, 1.00f );
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4 ( 0.26f, 0.59f, 0.98f, 0.35f );
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4 ( 0.20f, 0.20f, 0.20f, 0.35f );
		style.Colors[ImGuiCol_CustomColor] = ImVec4 ( 1.f, 1.f, 1.f, 1.f );

		style.FrameBorderSize = 0.f;
		style.WindowBorderSize = 0.f;
		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2 ( 5, 5 );
		style.WindowMinSize = ImVec2 ( 32, 32 );
		style.WindowRounding = 3.f;
		style.WindowTitleAlign = ImVec2 ( 0.5f, 0.5f );
		style.FramePadding = ImVec2 ( 2, 2 ); // 2 2 
		style.FrameRounding = 0.f;
		style.ItemSpacing = ImVec2 ( 8, 4 );
		style.ItemInnerSpacing = ImVec2 ( 8, 4 );
		style.TouchExtraPadding = ImVec2 ( 0, 0 );
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 3.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 3.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 3.0f;
		style.ButtonTextAlign = ImVec2 ( 0.5f, 0.5f );
		style.DisplayWindowPadding = ImVec2 ( 22, 22 );
		style.DisplaySafeAreaPadding = ImVec2 ( 4, 4 );
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;
		style.CurveTessellationTol = 1.25f;
	}

	void DarkTheme ( )
	{
		g_Options.color_bg = Color ( 54, 54, 54 );
		g_Options.color_bg_child = Color ( 32, 31, 31 );
		g_Options.color_text = Color ( 255, 255, 255 );
		slider = ImVec4 ( bg_child_color.x, bg_child_color.y, bg_child_color.z, 0.80f * bg_child_color.w );
		style_update ( );
	}


	void LightTheme ( )
	{
		g_Options.color_bg = Color ( 240, 240, 240 );
		g_Options.color_bg_child = Color ( 194, 194, 194 );
		g_Options.color_text = Color ( 0, 0, 0 );
		slider = ImVec4 ( 0.69f, 0.69f, 0.69f, 0.80f );
		style_update ( );
	}

	//example tab
	void SomeTab ( )
	{
		auto& style = ImGui::GetStyle ( );
		float group_w = ImGui::GetCurrentWindow ( )->Size.x - style.WindowPadding.x * 2;

		if ( ImGui::BeginChild ( _ ( "##body_content" ) ) )
		{

		}
		ImGui::EndChild ( );
	}

	void MiscTab ( )
	{
		auto& style = ImGui::GetStyle ( );
		float group_w = ImGui::GetCurrentWindow ( )->Size.x - style.WindowPadding.x * 2;

		if ( ImGui::BeginChild ( _ ( "##MISC_TAB_CONTENT" ) ) )
		{
			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns ( 1, nullptr, false );
			ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

			ImGui::SpacerText ( _ ( "Misc:" ) );

			if ( ImGui::Button ( "Load config" ) )
				load_config ( );
			if ( ImGui::Button ( "Save config" ) )
				save_config ( );
			ImGui::CheckboxRect ( _ ( "No-spread" ), &var::misc::nospread );
			if ( var::misc::nospread )
				ImGui::CheckboxRect ( _ ( "No-spread shotguns" ), &var::misc::noshotgunspread );
			ImGui::CheckboxRect ( _ ( "Spider-man" ), &var::misc::spider );
			ImGui::CheckboxRect ( _ ( "Instant eoka" ), &var::misc::eoka );
			ImGui::CheckboxRect ( _ ( "Instant compound bow" ), &var::misc::compound );
			ImGui::CheckboxRect ( _ ( "Fullbright" ), &var::misc::fullbright );
			ImGui::CheckboxRect ( _ ( "Always day" ), &var::misc::alwaysday );
			ImGui::CheckboxRect ( _ ( "Always night" ), &var::misc::alwaysnight );
			ImGui::CheckboxRect ( _ ( "No-slide" ), &var::misc::noslide );
			ImGui::CheckboxRect ( _ ( "Fakelag" ), &var::misc::fakelag );
			ImGui::CheckboxRect ( _ ( "Admin flag" ), &var::misc::admin );
			ImGui::CheckboxRect ( _ ( "Radar" ), &var::misc::radar );
			ImGui::CheckboxRect ( _ ( "Bullet thickness" ), &var::misc::thickness );
			if ( var::misc::thickness )
				ImGui::SliderFloat ( _ ( "Bullet thickness" ), &var::misc::flthickness, 0.0f, 0.75f );

			ImGui::CheckboxRect ( _ ( "Low Gravity" ), &var::misc::bgravity );
			ImGui::Hotkey ( _ ( "##Lowgravity" ), &var::misc::gravity, ImVec2 { 100.f, 20.f } );
			ImGui::CheckboxRect ( _ ( "No-clip" ), &var::misc::bfly ); ImGui::SameLine ( );
			ImGui::Hotkey ( _ ( "##Noclip" ), &var::misc::fly, ImVec2 { 100.f, 20.f } );
			if ( var::misc::bfly && var::misc::fly != 0 )
			{
				ImGui::CheckboxRect ( _ ( "Climb-assist" ), &var::misc::bclimb );
				ImGui::Hotkey ( _ ( "##Climb assist" ), &var::misc::assist, ImVec2 { 100.f, 20.f } );
			}
			ImGui::CheckboxRect ( _ ( "Timescale" ), &var::misc::btimescale );
			ImGui::Hotkey ( _ ( "##Timescale" ), &var::misc::timescale, ImVec2 { 100.f, 20.f } );
			ImGui::CheckboxRect ( _ ( "Fake-duck" ), &var::misc::bview_offset );
			ImGui::Hotkey ( _ ( "##Fakeduck" ), &var::misc::view_offset, ImVec2 { 100.f, 20.f } );
			if ( var::misc::radar )
				ImGui::SliderInt ( _ ( "Radar scale" ), &var::misc::scale, 0, 100 );
			ImGui::SliderFloat ( _ ( "Recoil X" ), &var::misc::recoil_pitch, 0.1f, 100.f );
			ImGui::SliderFloat ( _ ( "Recoil Y" ), &var::misc::recoil_yaw, 0.1f, 100.f );
			if ( var::misc::bview_offset && var::misc::view_offset != 0 )
			{
				ImGui::SliderFloat ( _ ( "Fake duck height" ), &var::misc::z, -5.f, 5.f );
			}
			if ( var::misc::btimescale && var::misc::timescale != 0 )
				ImGui::SliderFloat ( _ ( "Timescale speed" ), &var::misc::timescale_speed, 1.f, 20.f );
			if ( var::misc::bfly && var::misc::fly != 0 )
				ImGui::SliderFloat ( _ ( "Noclip speed" ), &var::misc::noclip_speed, 10.f, 100.f );
			ImGui::SliderFloat ( _ ( "Field of View" ), &var::misc::fov, 10.f, 180.f );
			ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

			if ( var::misc::alwaysnight )
				var::misc::alwaysday = false;

			ImGui::PopStyleVar ( );
		}
		ImGui::EndChild ( );
	}

	void AimTab ( )
	{
		auto& style = ImGui::GetStyle ( );
		float group_w = ImGui::GetCurrentWindow ( )->Size.x - style.WindowPadding.x * 2;

		if ( ImGui::BeginChild ( _ ( "##Aim_TAB_Conetnt" ) ) )
		{
			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns ( 1, nullptr, false );
			ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

			ImGui::CheckboxRect ( _ ( "Enabled" ), &var::aim::enabled );
			ImGui::Hotkey ( "##aim", &var::aim::aim_key, ImVec2 { 100, 20 } );
			ImGui::SpacerText ( _ ( "Aim:" ) );

			if ( !var::aim::enabled )
			{
				ImGui::PushItemFlag ( ImGuiItemFlags_Disabled, true );
				ImGui::PushStyleVar ( ImGuiStyleVar_Alpha, ImGui::GetStyle ( ).Alpha * 0.5f );
			}

			static const char* hitbox_items[] = {
					"Head",
					"Neck",
					"Chest",
					"Closest"
			};
			ImGui::Combo ( _ ( "Hitbox" ), &var::aim::hitbox, hitbox_items, IM_ARRAYSIZE ( hitbox_items ) );
			ImGui::SliderInt ( _ ( "FOV" ), &var::aim::aim_fov, 0, 180 );
			ImGui::CheckboxRect ( _ ( "Silent" ), &var::aim::silent );
			ImGui::CheckboxRect ( _ ( "Terrain check" ), &var::aim::terraincheck );
			ImGui::CheckboxRect ( _ ( "Team check" ), &var::aim::ignore_friends );
			ImGui::SliderFloat ( _ ( "Smoothing" ), &var::aim::smoothing, 0.f, 10.f );

			if ( !var::aim::enabled ) {
				ImGui::PopItemFlag ( );
				ImGui::PopStyleVar ( );
			}

			ImGui::PopStyleVar ( );
		}
		ImGui::EndChild ( );
	}

	void VisualsTab ( )
	{
		static const char* esp_tab_left_names[] = { "PLAYER", "ITEMS", "ORE", "TRAPS" };
		static const char* esp_tab_right_names[] = { "RADTOWN", "COLLECTIBLE", "VEHICLES", "OTHER" };
		static int   active_left_esp_tab = 0;
		static int   active_right_esp_tab = 0;

		auto& style = ImGui::GetStyle ( );
		style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
		ImGui::Columns ( 2, NULL, false );
		{
			ImGui::PushItemWidth ( -1 );
			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
			if ( ImGui::BeginChild ( _ ( "##body_content_tabs1" ), ImVec2 ( 300.f, 31.25f ) ) )
			{
				float group_w = ImGui::GetCurrentWindow ( )->Size.x;
				ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
				{
					ImGui::PushItemWidth ( -1 );
					renderMENU_tabs ( esp_tab_left_names, active_left_esp_tab, ( group_w / _countof ( esp_tab_left_names ) ), 31.25f, true );
					style.Colors[ImGuiCol_Button] = ImVec4 ( bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w );
					ImGui::PopItemWidth ( );
				}
				ImGui::PopStyleVar ( );
			}
			ImGui::EndChild ( );
			ImGui::PopStyleVar ( );
			ImGui::PopItemWidth ( );

			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
			if ( ImGui::BeginChild ( _ ( "##body_contentESP" ), ImVec2 ( 300.f, 0.f ), true ) )
			{
				if ( active_left_esp_tab == 0 ) {
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::players );
					ImGui::SpacerText ( _ ( "Player:" ) );
					ImGui::CheckboxRect ( _ ( "Name" ), &var::esp::name );
					ColorPickerBox ( _ ( "##nameclr" ), colors::player::name );

					ImGui::CheckboxRect ( _ ( "Box" ), &var::esp::box );
					ColorPickerBox ( _ ( "##boxclr" ), colors::player::box );

					ImGui::CheckboxRect ( _ ( "Skeleton" ), &var::esp::skeleton );
					ColorPickerBox ( _ ( "##skeletonclr" ), colors::player::skeleton );

					ImGui::CheckboxRect ( _ ( "Snapline" ), &var::esp::snaplines );
					ColorPickerBox ( _ ( "##snaplineclr" ), colors::player::snapline );

					ImGui::CheckboxRect ( _ ( "Health" ), &var::esp::health );
					ImGui::CheckboxRect ( _ ( "Distance" ), &var::esp::distance );
					ColorPickerBox ( _ ( "##distanceclr" ), colors::player::distance );


					ImGui::CheckboxRect ( _ ( "Weapon" ), &var::esp::weapon );
					ColorPickerBox ( _ ( "##weaponclr" ), colors::player::weapon );

					//ImGui::CheckboxRect ( _ ( "Ammo" ), &var::esp::ammo );
					//ColorPickerBox ( _ ( "##ammoclr" ), colors::player::ammo );

					ImGui::CheckboxRect ( _ ( "Sleepers" ), &var::esp::sleeper );
					ColorPickerBox ( _ ( "##sleeperclr" ), colors::player::sleeper );

					ImGui::CheckboxRect ( _ ( "NPC" ), &var::esp::npc );
					ColorPickerBox ( _ ( "##npcclr" ), colors::player::npc );

					ImGui::Columns ( 1, nullptr, false );
				}
				else if ( active_left_esp_tab == 1 ) {
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::item );
					ImGui::SpacerText ( _ ( "Dropped:" ) );

					ImGui::CheckboxRect ( _ ( "Weapons" ), &var::esp::item_weapon );
					ColorPickerBox ( _ ( "##wepclr" ), colors::dropped::weapon );

					ImGui::CheckboxRect ( _ ( "Corpse" ), &var::esp::item_corpse );
					ColorPickerBox ( _ ( "##corpseclr" ), colors::dropped::corpse );

					ImGui::CheckboxRect ( _ ( "Other" ), &var::esp::item_other );
					ColorPickerBox ( _ ( "##otherclr" ), colors::dropped::other );

					ImGui::Columns ( 1, nullptr, false );
				}
				else if ( active_left_esp_tab == 2 ) {
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::ores );
					ImGui::SpacerText ( _ ( "Ore:" ) );

					ImGui::CheckboxRect ( _ ( "Stone" ), &var::esp::stone );
					ColorPickerBox ( _ ( "##stoneclr" ), colors::ore::stone );

					ImGui::CheckboxRect ( _ ( "Sulfur" ), &var::esp::sulfur );
					ColorPickerBox ( _ ( "##sulfurclr" ), colors::ore::sulfur );

					ImGui::CheckboxRect ( _ ( "Metal" ), &var::esp::metal );
					ColorPickerBox ( _ ( "##metalclr" ), colors::ore::metal );

					ImGui::Columns ( 1, nullptr, false );
				}
				else if ( active_left_esp_tab == 3 ) {
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::traps );
					ImGui::SpacerText ( _ ( "Traps:" ) );

					ImGui::CheckboxRect ( _ ( "Auto-turret" ), &var::esp::autoturret );
					ColorPickerBox ( _ ( "##autotclr" ), colors::traps::autoturret );
					ImGui::CheckboxRect ( _ ( "Flame-turret" ), &var::esp::flameturret );
					ColorPickerBox ( _ ( "##flamclr" ), colors::traps::flameturret );
					ImGui::CheckboxRect ( _ ( "Shotgun-trap" ), &var::esp::shotguntrap );
					ColorPickerBox ( _ ( "##shotgclr" ), colors::traps::guntrap );
					ImGui::CheckboxRect ( _ ( "Land-mine" ), &var::esp::landmine );
					ColorPickerBox ( _ ( "##landmclr" ), colors::traps::landmine );
					ImGui::CheckboxRect ( _ ( "Bear-trap" ), &var::esp::beartrap );
					ColorPickerBox ( _ ( "##beartrclr" ), colors::traps::beartrap );
					ImGui::CheckboxRect ( _ ( "SAM-site" ), &var::esp::samsite );
					ColorPickerBox ( _ ( "##samclr" ), colors::traps::samsite );

					ImGui::Columns ( 1, nullptr, false );
				}
			}
			ImGui::PopStyleVar ( );
			ImGui::EndChild ( );
		}
		ImGui::NextColumn ( );
		{
			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
			ImGui::Dummy ( ImVec2 ( 10.f, 0.f ) );
			ImGui::SameLine ( );
			ImGui::PushItemWidth ( -1 );
			if ( ImGui::BeginChild ( _ ( "##body_content_tabs2" ), ImVec2 ( 300.f, 31.25f ), false ) )
			{
				float group_w = ImGui::GetCurrentWindow ( )->Size.x;
				{
					ImGui::PushItemWidth ( -1 );
					renderMENU_tabs ( esp_tab_right_names, active_right_esp_tab, ( group_w / _countof ( esp_tab_right_names ) ), 31.25f, true );
					style.Colors[ImGuiCol_Button] = ImVec4 ( bg_color.x - 0.05f, bg_color.y - 0.05f, bg_color.z - 0.05f, bg_color.w );
					ImGui::PopItemWidth ( );
				}
			}
			ImGui::EndChild ( );

			ImGui::Dummy ( ImVec2 ( 10.f, 0.f ) );
			ImGui::SameLine ( );
			if ( ImGui::BeginChild ( _ ( "##body_contentESP2" ), ImVec2 ( 300.f, 0.f ), true ) )
			{
				ImGui::PopStyleVar ( );
				if ( active_right_esp_tab == 0 ) {
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );

					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::radtown );

					ImGui::SpacerText ( _ ( "Radtown:" ) );

					ImGui::CheckboxRect ( _ ( "Barrels" ), &var::esp::barrel );
					ColorPickerBox ( _ ( "##brclr" ), colors::radtown::barrel );

					ImGui::CheckboxRect ( _ ( "Crates" ), &var::esp::crate );
					ColorPickerBox ( _ ( "##crclr" ), colors::radtown::crate );

					ImGui::CheckboxRect ( _ ( "Military crates" ), &var::esp::mcrate );
					ColorPickerBox ( _ ( "##mmclr" ), colors::radtown::mcrate );

					ImGui::CheckboxRect ( _ ( "Elite crates" ), &var::esp::ecrate );
					ColorPickerBox ( _ ( "##eeclr" ), colors::radtown::ecrate );

					ImGui::CheckboxRect ( _ ( "Toolboxes" ), &var::esp::toolbox );
					ColorPickerBox ( _ ( "##cetclr" ), colors::radtown::toolbox );

					ImGui::CheckboxRect ( _ ( "Minecarts" ), &var::esp::minecart );
					ColorPickerBox ( _ ( "##minclr" ), colors::radtown::minecart );

					ImGui::CheckboxRect ( _ ( "Oil barrels" ), &var::esp::oil );
					ColorPickerBox ( _ ( "##oilclr" ), colors::radtown::oil );

					ImGui::Columns ( 1, nullptr, false );
				}
				else if ( active_right_esp_tab == 1 ) {
					ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );
					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::collectable );
					ImGui::SpacerText ( _ ( "Collectable:" ) );

					ImGui::CheckboxRect ( _ ( "Hemp" ), &var::esp::hemp );
					ColorPickerBox ( _ ( "##hempclr" ), colors::collectable::hemp );

					ImGui::CheckboxRect ( _ ( "Mushroom" ), &var::esp::mushroom );
					ColorPickerBox ( _ ( "##mushroomclr" ), colors::collectable::mushroom );

					ImGui::CheckboxRect ( _ ( "Corn" ), &var::esp::corn );
					ColorPickerBox ( _ ( "##cornclr" ), colors::collectable::corn );

					ImGui::CheckboxRect ( _ ( "Berry" ), &var::esp::berry );
					ColorPickerBox ( _ ( "##berryclr" ), colors::collectable::berry );

					ImGui::CheckboxRect ( _ ( "Potato" ), &var::esp::potato );
					ColorPickerBox ( _ ( "##potatoclr" ), colors::collectable::potato );

					ImGui::CheckboxRect ( _ ( "Sulfur" ), &var::esp::csulfur );
					ColorPickerBox ( _ ( "##csulfurclr" ), colors::collectable::sulfur );

					ImGui::CheckboxRect ( _ ( "Stone" ), &var::esp::cstone );
					ColorPickerBox ( _ ( "##cstoneclr" ), colors::collectable::stone );

					ImGui::CheckboxRect ( _ ( "Metal" ), &var::esp::cmetal );
					ColorPickerBox ( _ ( "##cmetalclr" ), colors::collectable::metal );

					ImGui::CheckboxRect ( _ ( "Wood" ), &var::esp::cwood );
					ColorPickerBox ( _ ( "##cwoodclr" ), colors::collectable::wood );

					ImGui::Columns ( 1, nullptr, false );
					ImGui::PopStyleVar ( );
				}
				else if ( active_right_esp_tab == 2 ) {
					ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );
					ImGui::CheckboxRect ( _ ( "Enabled" ), &var::esp::vehicles );
					ImGui::SpacerText ( _ ( "Vehicles:" ) );

					ImGui::CheckboxRect ( _ ( "Horses" ), &var::esp::raid );
					ColorPickerBox ( _ ( "##horseclr" ), colors::vehicles::horse );

					ImGui::CheckboxRect ( _ ( "Boats" ), &var::esp::raid );
					ColorPickerBox ( _ ( "##horseclr" ), colors::vehicles::boat );

					ImGui::CheckboxRect ( _ ( "Helicopters" ), &var::esp::raid );
					ColorPickerBox ( _ ( "##horseclr" ), colors::vehicles::helicopter );

					ImGui::CheckboxRect ( _ ( "Special" ), &var::esp::special );
					ColorPickerBox ( _ ( "##horseclr" ), colors::vehicles::special );

					ImGui::Columns ( 1, nullptr, false );
					ImGui::PopStyleVar ( );
				}
				else if ( active_right_esp_tab == 3 ) {
					ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 { style.WindowPadding.x, style.ItemSpacing.y } );
					ImGui::Columns ( 1, nullptr, false );

					ImGui::Dummy ( ImVec2 ( 0.f, 5.f ) );
					ImGui::CheckboxRect ( _ ( "ESP Enabled" ), &var::esp::enabled );

					ImGui::SpacerText ( _ ( "Other:" ) );

					ImGui::CheckboxRect ( _ ( "Raid" ), &var::esp::raid );
					ColorPickerBox ( _ ( "##raidclr" ), colors::other::raid );

					ImGui::CheckboxRect ( _ ( "Hotbar" ), &var::esp::item_info );
					ImGui::CheckboxRect ( _ ( "Animals" ), &var::esp::animals );
					ColorPickerBox ( _ ( "##animalclr" ), colors::other::animal );

					ImGui::CheckboxRect ( _ ( "Stashes" ), &var::esp::stash );
					ColorPickerBox ( _ ( "##stashclr" ), colors::other::stash );

					ImGui::CheckboxRect ( _ ( "Tool cupboard" ), &var::esp::toolcupboard );
					ColorPickerBox ( _ ( "##cupbclr" ), colors::other::toolcupboard );
					if ( var::esp::toolcupboard )
						ImGui::CheckboxRect ( _ ( "Tool cupboard list" ), &var::esp::toolcupboard_list );
					ImGui::CheckboxRect ( _ ( "Sleeping bag" ), &var::esp::sleepingbag );
					ColorPickerBox ( _ ( "##sleepclr" ), colors::other::sleepingbag );

					ImGui::CheckboxRect ( _ ( "FOV Circle" ), &var::esp::fov_circle );
					ColorPickerBox ( _ ( "##fovclr" ), colors::other::fov );

					ImGui::CheckboxRect ( _ ( "Crosshair" ), &var::esp::crosshair );
					ColorPickerBox ( _ ( "##crossclr" ), colors::other::crosshair );

					ImGui::CheckboxRect ( _ ( "Hide in menu" ), &var::esp::hide );
					ImGui::SliderFloat ( _ ( "Global distance limit" ), &var::esp::distance_limit, 1.f, 4000.f );
					ImGui::CheckboxRect ( _ ( "VSync" ), &vsync );

					ImGui::Columns ( 1, nullptr, false );
					ImGui::PopStyleVar ( );
				}
			}
			else
			{
				ImGui::PopStyleVar ( );
			}
			ImGui::EndChild ( );
		}
	}

	void RenderVisuals ( )
	{
		static const char* visuals_tab_names[] = { "ESP", "Other" };
		static int   active_visuals_tab = 0;

		auto& style = ImGui::GetStyle ( );
		ImGui::Columns ( 1, NULL, false );
		{
			style.Colors[ImGuiCol_ChildWindowBg] = bg_child_color;
			if ( ImGui::BeginChild ( _ ( "##body_visuals" ), ImVec2 ( 0.f, 31.25 ), false ) )
			{
				float group_w = ImGui::GetCurrentWindow ( )->Size.x;
				ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
				{
					ImGui::PushItemWidth ( -1 );
					renderOther_tabs ( visuals_tab_names, active_visuals_tab, ( group_w / _countof ( visuals_tab_names ) ), 31.25f, true );
					ImGui::PopItemWidth ( );
				}
				ImGui::PopStyleVar ( );
			}
			ImGui::EndChild ( );
			ImGui::Dummy ( ImVec2 ( 0.0f, 5.f ) );
			style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
		}

	}

	void gui ( ) {
		color_picker_open = false;
		static int active_sidebar_tab = 0;
		auto& style = ImGui::GetStyle ( );
		//set position and size
		ImGui::SetNextWindowPos ( ImVec2 { 300, 140 }, ImGuiCond_Once );
		ImGui::SetNextWindowSize ( ImVec2 { 710, 530 }, ImGuiCond_Always );//

		if ( ImGui::Begin ( "GetRekt", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding ) )
		{
			ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
			{
				style.Colors[ImGuiCol_Text] = ImVec4 ( 1.f, 1.f, 1.f, text.w );

				//----------
				//Button
				//----------

				style.Colors[ImGuiCol_Button] = ImVec4 ( themecolor.x, themecolor.y, themecolor.z, themecolor.w );
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4 ( themecolor.x - 0.1f, themecolor.y - 0.1f, themecolor.z - 0.1f, themecolor.w );
				style.Colors[ImGuiCol_ButtonActive] = ImVec4 ( themecolor.x - 0.15f, themecolor.y - 0.15f, themecolor.z - 0.15f, themecolor.w );

				style.Colors[ImGuiCol_Button] = button;
				style.Colors[ImGuiCol_ButtonHovered] = buttonhovered;
				style.Colors[ImGuiCol_ButtonActive] = buttonActive;

				//----------
				//End of button
				//----------

				style.Colors[ImGuiCol_ChildWindowBg] = ImVec4 ( themecolor.x, themecolor.y, themecolor.z, 0.39f * themecolor.w );  //header color

				//----------
				//Header
				//----------
				ImGui::PushFont ( spacerfont );
				if ( ImGui::BeginChild ( _ ( "##transparent" ), ImVec2 ( 690.f, 42.0f ) ) )
				{
					float window_size = ImGui::GetCurrentWindow ( )->Size.y / 2.0f;
					auto Header = _ ( "GetRekt" );

					auto pos = ImGui::GetCurrentWindow ( )->Pos + ImGui::GetCurrentWindow ( )->Size / 2.0f;

					ImGui::RenderText ( pos - ImGui::CalcTextSize ( Header ) / 2.0f, Header ); // McCheats text
				}
				ImGui::EndChild ( );

				ImGui::PopFont ( );
				//----------
				//End of header
				//----------

				style.Colors[ImGuiCol_Text] = text;
				style.Colors[ImGuiCol_ChildWindowBg] = bg_color;


				//----------
				//Window Tabs
				//----------

				if ( ImGui::BeginChild ( _ ( "##sidebar" ), ImVec2 ( 690.f, 32.0f ) ) )
				{
					float group_w = 690.f; //- style.WindowPadding.x * 2

					ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2::Zero );
					{
						static const char* sidebar_tabs[] = {
						"AIM",
						"VISUALS",
						"MISC"
						};

						render_tabs ( sidebar_tabs, active_sidebar_tab, ( group_w / _countof ( sidebar_tabs ) ), 32.0f, true );
					}
					ImGui::PopStyleVar ( );
				}
				ImGui::EndChild ( );
				//----------
				//End of Tabs
				//----------
			}
			ImGui::PopStyleVar ( );

			// Make the body the same vertical size as the sidebar
			// except for the width, which we will set to auto
			style.WindowPadding = ImVec2 ( 5, 5 );


			//----------
			//Content section
			//----------

			//child 1 bg white
			if ( ImGui::BeginChild ( _ ( "##body" ), ImVec2 ( 690.f, 430.f ), true, ImGuiWindowFlags_AlwaysUseWindowPadding ) )
			{

				style.Colors[ImGuiCol_ChildWindowBg] = bg_child_color;// ImVec4(0.76f, 0.76f, 0.76f, 1.f);
				style.WindowPadding = ImVec2 ( 25, 25 );

				//child 2 body bg gray
				if ( active_sidebar_tab == 5 )
					style.WindowPadding = ImVec2 ( 0, 0 );

				if ( ImGui::BeginChild ( _ ( "##Background" ), ImVec2 ( 680.f, 420.f ), true, ImGuiWindowFlags_AlwaysUseWindowPadding ) )
				{
					style.Colors[ImGuiCol_ChildWindowBg] = bg_color;
					style.WindowPadding = ImVec2 ( 5, 5 );

					if ( active_sidebar_tab == 0 )		AimTab ( );
					else if ( active_sidebar_tab == 1 ) 	VisualsTab ( );
					else if ( active_sidebar_tab == 2 )	MiscTab ( );

				}
				ImGui::EndChild ( );
			}
			ImGui::EndChild ( );

		}
		ImGui::End ( );

		if ( color_picker_open )
			style_update ( );
	}
}
#endif

#define EAC_WINDOW_TITLE	L"BiGlH"
#define EAC_WINDOW_CLASS	L"BiGlH"
#define EAC_WINDOW_STYLE	WS_POPUP | WS_CLIPSIBLINGS /*| WS_VISIBLE*/
#define EAC_WINDOW_EXSTYLE	WS_EX_NOACTIVATE | WS_EX_NOREDIRECTIONBITMAP | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST

LRESULT WINAPI WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

template< typename T >
inline void SAFE_RELEASE ( T p )
{
	if ( p )
	{
		p->Release ( );
		p = nullptr;
	}
}

BOOL init_overlay ( )
{
	WNDCLASS window_class = { };
	window_class.lpszClassName = EAC_WINDOW_CLASS;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;

	if ( FAILED ( RegisterClass ( &window_class ) ) )
		return FALSE;

	// Create the window for the overlay
	window_handle = CreateWindowExW ( EAC_WINDOW_EXSTYLE, EAC_WINDOW_CLASS, EAC_WINDOW_TITLE, EAC_WINDOW_STYLE, 0, 0, 1, 1, nullptr, nullptr, nullptr, nullptr );
	if ( !window_handle )
		return FALSE;

	/*if ( !InitDirectX( window_handle ) )
		return FALSE;*/

	if ( FAILED ( D3D11CreateDevice ( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
		D3D11_SDK_VERSION, device.GetAddressOf ( ), nullptr, device_context.GetAddressOf ( ) ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OEDDCD" ), 0 );
		return FALSE;
	}

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;
	if ( FAILED ( device.As ( &dxgi_device ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OEDXGI" ), 0 );
		return FALSE;
	}
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxFactory;
	if ( FAILED ( CreateDXGIFactory2 ( DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS ( dxFactory.GetAddressOf ( ) ) ) ) )
	{
		MessageBoxW ( 0, L"CRASH", L"OEDXGIF2", 0 );
		return FALSE;
	}
#else
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxFactory;
	if ( FAILED ( CreateDXGIFactory2 ( 0, IID_PPV_ARGS ( dxFactory.GetAddressOf ( ) ) ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OEDXGIF2" ), 0 );
		return FALSE;
	}
#endif
	DXGI_SWAP_CHAIN_DESC1 description;
	RtlZeroMemory ( &description, sizeof ( DXGI_SWAP_CHAIN_DESC1 ) );
	description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	description.Scaling = DXGI_SCALING_STRETCH;
	description.Width = 1;                                              // Minimum size only for resize.
	description.Height = 1;
	description.SampleDesc.Count = 1;                                   // Don't use multi-sampling.
	description.SampleDesc.Quality = 0;
	description.BufferCount = 2;                                        // Use two buffers to enable flip effect.
	description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;          // flip mode and discared buffer after presentation.

	//
	if ( FAILED ( dxFactory->CreateSwapChainForComposition ( dxgi_device.Get ( ), &description, nullptr, swap_chain.GetAddressOf ( ) ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OESWPCHN" ), 0 );
		return FALSE;
	}

	// 
	Microsoft::WRL::ComPtr<IDCompositionDevice> dcomp_device;
	if ( FAILED ( DCompositionCreateDevice ( dxgi_device.Get ( ), IID_PPV_ARGS ( dcomp_device.GetAddressOf ( ) ) ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OEDCOMPIN" ), 0 );
		return FALSE;
	}

	IDCompositionTarget* target;
	auto hr = dcomp_device->CreateTargetForHwnd ( window_handle, true, &target );

	Microsoft::WRL::ComPtr<IDCompositionVisual> visual;
	hr = dcomp_device->CreateVisual ( visual.GetAddressOf ( ) );

	hr = visual->SetContent ( swap_chain.Get ( ) );
	hr = target->SetRoot ( visual.Get ( ) );

	hr = dcomp_device->Commit ( );

	// Setup ImGui context
	ImGui::CreateContext ( );
	ImGui::StyleColorsClassic ( );
	ImGui_ImplWin32_Init ( window_handle );
	ImGui_ImplDX11_Init ( device.Get ( ), device_context.Get ( ) );
	overlay_initialized = true;

	return TRUE;
}

void ClearRenderTargetView ( )
{
	render_target_view.Reset ( );
}

void CreateRenderTargetView ( )
{
	// Smart pointer to back buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;

	// Get swap chain back buffer
	if ( FAILED ( swap_chain->GetBuffer ( 0, IID_PPV_ARGS ( back_buffer.GetAddressOf ( ) ) ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OERTV" ), 0 );
		return;
	}

	// Creates a render-target view for accessing resource data.
	if ( FAILED ( device->CreateRenderTargetView ( back_buffer.Get ( ), NULL, &render_target_view ) ) )
	{
		MessageBoxW ( 0, _ ( L"CRASH" ), _ ( L"OERTV2" ), 0 );
		return;
	}
}


LRESULT WINAPI WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_SIZE:
		if ( swap_chain )
		{
			ClearRenderTargetView ( );
			swap_chain->ResizeBuffers ( 0, (UINT)LOWORD ( lParam ), (UINT)HIWORD ( lParam ), DXGI_FORMAT_UNKNOWN, 0 );
			CreateRenderTargetView ( );
		}
		return 0;
	case WM_SYSCOMMAND:
		if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage ( 0 );
		return 0;
	}

	return ::DefWindowProc ( hWnd, msg, wParam, lParam );
}