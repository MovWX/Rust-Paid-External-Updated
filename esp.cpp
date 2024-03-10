#include "include.hpp"
#include "overlay.hpp"
#include "cheat.hpp"
#include "object_loop.hpp"
#include "fonts.hpp"
#ifdef GETREKT
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imguirekt/imgui_internal.h"
#include "imguirekt/extra.h"
#else
#include "imgui/imgui_internal.h"
#endif
#include <Psapi.h>
#include "esp.hpp"
#include "esputils.hpp"
#include "minhook/MinHook.h"

#pragma comment(lib, "dminhook.lib")

#pragma warning ( push )
#pragma warning ( disable: 4244 )

static std::chrono::steady_clock::time_point start;

void render_shitz ( )
{
	start = std::chrono::steady_clock::now ( );
	ImGui_ImplDX11_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );

	dwl = ImGui::GetBackgroundDrawList ( );
	renderer->m_pDrawList = dwl;

	uintptr_t m_pPlayerInput = mist::esp->read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::player_input );

	if ( keybinds::menu )
	{
#ifdef GETREKT
		GetRekt::gui ( );
#else
		imgui_stuff ( );
#endif
	}

	if ( var::esp::enabled )
	{
		game::draw_mutex.lock ( );
		auto stash = game::stashes;
		auto animals = game::animals;
		auto collectable = game::collectibles;
		auto ores = game::ores;
		auto items = game::items;
		auto radtown = game::barrel;
		auto vehicles = game::vehicles;
		auto special = game::special;
		auto traps = game::traps;
		auto raid = game::raid;
		auto players = game::players;
		auto cupboard = game::cupboard;
		auto bags = game::bag;
		game::draw_mutex.unlock ( );

		ImGui::PushFont ( espfont );
		do {
			if ( m_flFontHeight == 0.f )
				m_flFontHeight = renderer->measure_text ( "A" ).y;

			char buffer[64];
			sprintf ( buffer, _ ( "ricochet.gg (%d)" ), frmtm );
			renderer->draw_text ( 5, 5, buffer, 0xFFFFFFFF, true, false );

			if ( !game::m_rLocalInfo.m_szWeaponName.empty ( ) )
				renderer->draw_text ( 15, 20, game::m_rLocalInfo.m_szWeaponName.c_str ( ), 0xFFFFFFFF, true, false );

			auto m_mxViewMatrix = rust::get_view_matrix ( mist::esp.get ( ) );

			if ( game::m_pLocalplayer == 0 )
				break;

			game::m_rLocalInfo.m_vecLocal = mist::esp->read<Vector3> ( rust::get_object_pos_component ( mist::esp.get ( ), game::m_rLocalInfo.m_pPlayer ) + 0x90 );

			if ( !mist::esp->read<uint8_t> ( m_pPlayerInput + 0x94 ) && var::esp::hide )
				break;

			if ( var::misc::omnisprint )
				game::m_rLocalInfo.m_pModelState->set_sprinting ( mist::esp.get ( ), true );

			if ( var::esp::stash )
				ESP_staticstringset ( stash, ImColor { colors::other::stash[0], colors::other::stash[1], colors::other::stash[2] } );

			if ( var::esp::animals )
				ESP_staticstringset ( animals, ImColor { colors::other::animal[0], colors::other::animal[1], colors::other::animal[2] } );

			if ( var::esp::collectable )
				ESP_staticstringcolorset ( collectable );

			if ( var::esp::ores )
				ESP_staticstringcolorset ( ores );

			if ( var::esp::item )
				ESP_staticstringcolorset ( items );

			if ( var::esp::radtown )
				ESP_staticstringcolorset ( radtown );

			if ( var::esp::vehicles )
			{
				if ( var::esp::special )
					ESP_stringset ( special );

				ESP_staticstringcolorset ( vehicles );
			}

			if ( var::esp::traps )
				ESP_staticstringcolorset ( traps );

			if ( var::esp::raid )
				ESP_raidset ( raid );

			if ( var::esp::players )
				ESP_player ( players );

			if ( var::misc::omnisprint )
				game::m_rLocalInfo.m_pModelState->set_sprinting ( mist::esp.get ( ), true );

			if ( var::esp::toolcupboard )
				ESP_staticstringlistcolorset ( cupboard );

			if ( var::esp::sleepingbag )
				ESP_staticstringcolorset ( bags );

			if ( data::aimbot::m_bFound && var::esp::item_info )
			{
				static auto m_pCachedPlayer = 0;
				static char iterator = 0;
				static std::vector<PlayerItems> m_vecCachedItems;
				auto m_pPlayer = data::aimbot::m_pTarget;
				BasePlayer* m_pBasePlayer = (BasePlayer*)rust::get_base_entity ( mist::esp.get ( ), m_pPlayer );

				if ( m_pCachedPlayer != m_pPlayer || iterator % 1000 == 0 )
				{
					m_vecCachedItems = m_pBasePlayer->get_item_list ( mist::esp.get ( ) );
					m_pCachedPlayer = m_pPlayer;

					iterator = 0;
				}

				auto& cache = game::cache[m_pBasePlayer->get_uid ( mist::esp.get ( ) )];
				if ( m_vecCachedItems.size ( ) > 0 )
				{
					char header[128];
					sprintf ( header, "%s [%s] %dHP", cache.m_szName.c_str ( ), cache.m_szWeaponName.c_str ( ), static_cast<int>( m_pBasePlayer->get_health ( mist::esp.get ( ) ) ) );
					ImVec2 m_vecSize = renderer->measure_text ( header );

					m_vecSize.x = max ( m_vecSize.x, 256 );
					renderer->draw_rectangle ( 10, 150, m_vecSize.x + 35, 120, ImColor { 0, 0, 0, 120 } );
					renderer->draw_outline ( 10, 150, m_vecSize.x + 35, 120, 1.f, 0xFFFFFFFF );
					renderer->draw_text ( 10 + 5, 160, header, ImColors::White, false, false );

					int bx = 10;
					int by = 176;

					for ( auto& m_piEntry : m_vecCachedItems )
					{
						if ( std::empty ( m_piEntry.m_szName ) || m_piEntry.m_iAmount == 0 || !m_piEntry.m_iAmount )
							continue;

						char bfr[128];
						sprintf ( bfr, "[%d] %s", m_piEntry.m_iAmount, m_piEntry.m_szName.c_str ( ) );
						bool m_bActive = m_piEntry.m_bActive;
						int r = 255, g = m_bActive ? 0 : 255, b = m_bActive ? 0 : 255;

						renderer->draw_text ( bx + 10, by, bfr, ImColor { r, g, b }, false );
						by += 12;
					}
				}
				++iterator;
			}

			if ( var::misc::radar )
			{
				float x = monitor::width - 175, y = 200;

				dwl->AddLine ( ImVec2 { x, y - 150 }, ImVec2 { x, y + 150 }, ImColor { 0.f, 0.f, 0.f, 1.f } );
				dwl->AddLine ( ImVec2 { x - 150, y }, ImVec2 { x + 150, y }, ImColor { 0.f, 0.f, 0.f, 1.f } );
				dwl->AddRectFilled ( ImVec2 { x - 1, y - 1 }, ImVec2 { x + 2, y + 2 }, 0xFFFFFFFF, 0.5f );

				Vector2 m_vecLocal { game::m_rLocalInfo.m_vecLocal.x, game::m_rLocalInfo.m_vecLocal.z };
				Vector2 m_angLocal = game::m_pLocalplayer->get_body_angles ( mist::esp.get ( ) );
				for ( const auto& entry : players )
				{
					auto entity = entry.first;
					if ( !entity )
						continue;

					if ( entry.second == entity_type::Scientist && !var::esp::npc )
						continue;

					const Vector3 position = mist::esp->read<Vector3> ( rust::get_object_pos_component ( mist::esp.get ( ), entity ) + 0x90 );
					if ( position.empty ( ) )
						continue;

					BasePlayer* nn = (BasePlayer*)rust::get_base_entity ( mist::esp.get ( ), entity );

					if ( nn->is_sleeping ( mist::esp.get ( ) ) )
						continue;

					Vector2 m_vecEnemy { position.x, position.z };
					m_vecEnemy = m_vecLocal - m_vecEnemy;

					float m_flDistance = m_vecEnemy.length ( ) * (float)var::misc::scale / 100.f;
					m_vecEnemy = m_vecEnemy.normalized ( );
					m_flDistance = min ( 150.f, m_flDistance );
					m_vecEnemy *= m_flDistance;
					m_vecEnemy = rust::RotatePoint ( m_vecEnemy, { 0.f, 0.f }, 360.f - m_angLocal.y, false );
					{
						ImColor color = entry.second == entity_type::Scientist ? ImColor { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f } : ImColor { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
						if ( nn->is_wounded ( mist::esp.get ( ) ) )
							color = ImColor { 1.f, 1.f, 0.f, 0.5f };
						if ( game::cache[nn->get_uid ( mist::esp.get ( ) )].m_bFriend == true )
							color = ImColor { 0.f, 1.f, 0.f };

						dwl->AddRectFilled ( ImVec2 { x - m_vecEnemy.x - 2, y + m_vecEnemy.y - 2 }, ImVec2 { x - m_vecEnemy.x + 2, y + m_vecEnemy.y + 2 }, color, 0.5f );
					}
				}
			}

			if ( data::aimbot::m_bFound && game::m_rLocalInfo.m_vInfo.m_bPrediction == true )
			{
				Vector2 predpos;
				if ( rust::world_to_screen ( data::aimbot::m_vecPosition, predpos, m_mxViewMatrix ) )
					dwl->AddRectFilled ( ImVec2 { predpos.x - 2, predpos.y - 2 }, ImVec2 { predpos.x + 2, predpos.y + 2 }, ImColor { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f } );
			}

			if ( var::esp::fov_circle )
				dwl->AddCircle ( ImVec2 { monitor::width / 2, monitor::height / 2 }, data::aimbot::m_flFOV, ImColor { colors::other::fov[0], colors::other::fov[1], colors::other::fov[2] }, 64, 1.f );

			if ( var::esp::crosshair )
			{
				dwl->AddRectFilled ( ImVec2 { monitor::width / 2 - 10, monitor::height / 2 - 2 }, ImVec2 { monitor::width / 2 - 5, monitor::height / 2 + 2 }, ImColor { colors::other::crosshair[0], colors::other::crosshair[1], colors::other::crosshair[2] } );
				dwl->AddRectFilled ( ImVec2 { monitor::width / 2 + 10, monitor::height / 2 - 2 }, ImVec2 { monitor::width / 2 + 5, monitor::height / 2 + 2 }, ImColor { colors::other::crosshair[0], colors::other::crosshair[1], colors::other::crosshair[2] } );
				dwl->AddRectFilled ( ImVec2 { monitor::width / 2 - 2, monitor::height / 2 - 10 }, ImVec2 { monitor::width / 2 + 2, monitor::height / 2 - 5 }, ImColor { colors::other::crosshair[0], colors::other::crosshair[1], colors::other::crosshair[2] } );
				dwl->AddRectFilled ( ImVec2 { monitor::width / 2 - 2, monitor::height / 2 + 5 }, ImVec2 { monitor::width / 2 + 2, monitor::height / 2 + 10 }, ImColor { colors::other::crosshair[0], colors::other::crosshair[1], colors::other::crosshair[2] } );
			}

			Vector3 local_pos = game::m_pLocalplayer->get_bone_position ( mist::esp.get ( ), BoneList::head );
			Vector2 screen_pos;
			if ( rust::world_to_screen ( local_pos, screen_pos, m_mxViewMatrix ) )
				dwl->AddRect ( { screen_pos.x - 1, screen_pos.y - 1 }, { screen_pos.x + 1, screen_pos.y + 1 }, ImColor { 1.f, 1.f, 1.f } );


		} while ( 0 );

		ImGui::PopFont ( );
	}

	ImGui::Render ( );

	device_context->OMSetRenderTargets ( 1, render_target_view.GetAddressOf ( ), NULL );
	ImGui_ImplDX11_RenderDrawData ( ImGui::GetDrawData ( ) );
	swap_chain->Present ( vsync, 0 );
	frmtm = ( std::chrono::steady_clock::now ( ) - start ).count ( );
}

bool setonce = false, setonce2 = false;

int esp::main ( )
{
	MSG msg { };
	RtlZeroMemory ( &msg, sizeof ( MSG ) );

	if ( !init_overlay ( ) )
		return ERROR;

	while ( !( target_window = LI_FN ( FindWindowW )( _ ( L"UnityWndClass" ), nullptr ) ) )
		sleep_for ( 10s );

	float clear_color[4] { 0.45f, 0.55f, 0.60f, 1.00f };
	load_config ( );

	ImGuiIO& io = ImGui::GetIO ( );
	ImGui::StyleColorsClassic ( );

	HWND hDesktop = LI_FN ( GetDesktopWindow ) ( );
	RECT resolution;
	LI_FN ( GetWindowRect ) ( hDesktop, &resolution );

	monitor::width = resolution.right;
	monitor::height = resolution.bottom;

	renderer = std::make_unique< Renderer > ( device.Get ( ), device_context.Get ( ), resolution.right, resolution.bottom );

	arial = io.Fonts->AddFontFromMemoryCompressedTTF ( Arial_compressed_data, Arial_compressed_size, 13.f, NULL, io.Fonts->GetGlyphRangesJapanese ( ) );
	espfont = io.Fonts->AddFontFromMemoryCompressedTTF ( Pixel_compressed_data, Pixel_compressed_size, 10.f, NULL );
	ImFontConfig config = ImFontConfig ( );
	config.OversampleH = 1;

	auto nigger = ImGui::GetStyle ( );
	nigger.Alpha = 1.f;

	ImGui::GetIO ( ).FontDefault = arial;
	ImGui::GetIO ( ).WantCaptureMouse = true;
#ifndef _DEBUG
	while ( !( target_window = LI_FN ( FindWindowW )( _ ( L"UnityWndClass" ), nullptr ) ) )
		sleep_for ( 10s );
#endif
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != WM_NULL )
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
			continue;
		}

		//
		RECT overlay_rect { };
		bool overlay_visible = IsWindowVisible ( window_handle );
		bool overlay_running = GetWindowRect ( window_handle, &overlay_rect );
		// 
		RECT target_rect { };
		bool target_running = GetWindowRect ( target_window, &target_rect );
		bool target_focused = target_window == GetForegroundWindow ( );

		if ( !target_focused )
		{
			// Hide overlay if the game is not in the foreground
			if ( overlay_visible )
			{
				ShowWindow ( window_handle, SW_HIDE );
			}
			// Close overlay if the target is not available.
			if ( !target_running )
			{
				Beep ( 5, 400 );
				ExitProcess ( 0 );
				break;
			}
			continue;
		}

		if ( !overlay_visible ||
			( overlay_rect.right != target_rect.right &&
				overlay_rect.left != target_rect.left &&
				overlay_rect.bottom != target_rect.bottom &&
				overlay_rect.top != target_rect.top ) )
		{
			SetWindowPos ( window_handle, HWND_TOPMOST, target_rect.left, target_rect.top, target_rect.right - target_rect.left, target_rect.bottom - target_rect.top, SWP_SHOWWINDOW );
			ClearRenderTargetView ( );
			swap_chain->ResizeBuffers ( 0, target_rect.right - target_rect.left, target_rect.bottom - target_rect.top, DXGI_FORMAT_UNKNOWN, 0 );
			CreateRenderTargetView ( );
		}
		const float nigga[4] = { 0.f, 0.f, 0.f, 0.f };
		device_context->ClearRenderTargetView ( render_target_view.Get ( ), nigga );
		render_shitz ( );
	}

	return ERROR_SUCCESS;
}
#pragma warning ( pop )