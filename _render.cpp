#include "_render.hpp"
#include "menu.hpp"

namespace var
{
	namespace aim {
		bool enabled;
		bool prediction;
		bool recoil_compensation;

		int hitbox;
		int smoothing;
		int aim_fov;
		int aim_key;
	};

	namespace esp {
		bool enabled;
		bool players;
		bool sleeper;
		bool npc;
		bool special;
		bool animal;
		bool fov_circle;

		bool name;
		bool box;
		bool skeleton;
		bool health;
		bool distance;
		bool weapon;
		bool item_info;
		bool item;

		int distance_limit;
	};

	namespace misc {
		int recoil_pitch;
		int recoil_yaw;

		bool force_automatic;
		bool nospread;
		bool spider;

		bool eoka;
		bool compound;
		bool omnisprint;
	};
};

bool activeWindowRust = false;
bool m_bMenuEnabled = false;

BOOL CALLBACK EnumWindowsCallbackFindRust ( HWND hwnd, LPARAM lParam )
{
	if (GetForegroundWindow ( ) == hwnd)
	{
		int length = GetWindowTextLength ( hwnd );
		if (length == 0)
			return true;

		char* title = new char[length + 1];
		GetWindowTextA ( hwnd, title, length + 1 );

		activeWindowRust = strcmp ( title, "Rust" ) == 0;

		return false;
	}

	return true;
}
void CheckMenuInput()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT))
		{
			m_bMenuEnabled = !m_bMenuEnabled;
			Sleep(80);
		}

		Sleep(5);
	}
}
void CheckIfOverlayShouldDraw ( )
{
	while (true)
	{
		EnumWindows ( EnumWindowsCallbackFindRust, NULL );

		sleep_for ( milliseconds ( 1000 ) );
	}
}
void CheckIfMenuOpen ( )
{
	while (true)
	{
		if (m_bMenuEnabled)
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = true;
			sleep_for ( milliseconds ( 25 ) );
			continue;
		}

		if (!activeWindowRust)
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = false;
			sleep_for(milliseconds(10));
			continue;
		}

		if (!game::m_pLocalplayer) // not in game, no local
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = false;
			sleep_for ( milliseconds ( 500 ) );
			continue;
		}

		uintptr_t playerInput = driver::read<uintptr_t> ( (uintptr_t)game::m_pLocalplayer + offsets::player_input );

		if (!playerInput)
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = false;
			sleep_for ( milliseconds ( 100 ) );
			continue;
		}

		DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = driver::read<bool>(playerInput + 0x94);

		sleep_for ( milliseconds ( 25 ) );
	}
}

void renderer::OverlayThread ( )
{
	//HWND m_pWindowHandle = DIA4A::UIUtilities::m_pFindCurrentHWND();
	HWND m_pWindowHandle = GetConsoleWindow ( );
	if (m_pWindowHandle)
	{
		std::thread checkOverlayThread ( CheckIfOverlayShouldDraw );
		checkOverlayThread.detach ( );

		std::thread checkMenuThread ( CheckIfMenuOpen );
		checkMenuThread.detach ( );

		std::thread checkMenuInput(CheckMenuInput);
		checkMenuInput.detach();

		std::string m_strError = "";
		DIA4A::UIExternalDirect3DOverlay::m_bFullInitializeUIExternalDirect3DOverlay ( m_pWindowHandle, &m_strError );
	}
	else
	{
		MessageBoxA ( NULL, "Failed To Initialize Overlay", "Ricochet", 0 );
	}
}

void ImGuiKeyBind ( std::string label, int* key )
{
	CHAR name[128];
	UINT scanCode = MapVirtualKeyA ( *key, MAPVK_VK_TO_VSC );
	LONG lParamValue = (scanCode << 16);
	int result = GetKeyNameTextA ( lParamValue, name, 128 );

	switch (*key)
	{
	case VK_LBUTTON: strcpy_s ( name, "left mouse" ); break;
	case VK_RBUTTON: strcpy_s ( name, "right mouse" ); break;
	case VK_MBUTTON: strcpy_s ( name, "middle mouse" ); break;
	case VK_XBUTTON1: strcpy_s ( name, "X1 mouse" ); break;
	case VK_XBUTTON2: strcpy_s ( name, "X2 mouse" ); break;
	}

	std::string btnName = label;
	btnName.append ( ": " );
	btnName.append ( name );

	if (ImGui::Button ( btnName.c_str ( ) ))
	{
		Sleep ( 20 );

		bool waitingForKey = true;

		while (waitingForKey)
		{
			for (int i = 1; i < 256; i++)
			{
				if (GetAsyncKeyState ( i ) & 0x8000)
				{
					*key = i;

					waitingForKey = false;

					break;
				}
			}
		}
	}
}

void AimTab ( )
{
	ImGui::Checkbox ( "enabled", &var::aim::enabled );
	ImGui::Checkbox ( "prediction", &var::aim::prediction );
	ImGui::Checkbox ( "recoil compensation", &var::aim::recoil_compensation );

	ImGui::Separator ( );

	const char* items[] = { "head", "chest" };
	static const char* current_item = items[0];

	if (ImGui::BeginCombo ( "##hitboxCombo", current_item ))
	{
		for (int n = 0; n < IM_ARRAYSIZE ( items ); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable ( items[n], is_selected ))
				current_item = items[n];

			if (is_selected)
			{
				var::aim::hitbox = n;

				ImGui::SetItemDefaultFocus ( );
			}
		}
		ImGui::EndCombo ( );
	}

	ImGui::SliderInt ( "fov", &var::aim::aim_fov, 0, 180 );
	ImGui::SliderInt ( "smooth", &var::aim::smoothing, 0, 100 );

	ImGuiKeyBind ( "aim key", &var::aim::aim_key );
}

void EspTab ( )
{
	ImGui::Checkbox ( "enabled", &var::esp::enabled );
	ImGui::Checkbox ( "players", &var::esp::players );
	ImGui::Checkbox ( "sleepers", &var::esp::sleeper );
	ImGui::Checkbox ( "npc", &var::esp::npc );
	ImGui::Checkbox ( "special", &var::esp::special );
	ImGui::Checkbox ( "animal", &var::esp::animal );
	ImGui::Checkbox ( "fov circle", &var::esp::fov_circle );

	ImGui::Separator ( );

	ImGui::Checkbox ( "name", &var::esp::name );
	ImGui::Checkbox ( "box", &var::esp::box );
	ImGui::Checkbox ( "skeleton", &var::esp::skeleton );
	ImGui::Checkbox ( "health", &var::esp::health );
	ImGui::Checkbox ( "distance", &var::esp::distance );
	ImGui::Checkbox ( "weapon", &var::esp::weapon );
	ImGui::Checkbox ( "item info", &var::esp::item_info );
	ImGui::Checkbox ( "item", &var::esp::item );

	ImGui::Separator ( );

	ImGui::SliderInt ( "distance limit", &var::esp::distance_limit, 0, 3500 );
}

void MiscTab ( )
{
	ImGui::SliderInt ( "recoil pitch", &var::misc::recoil_pitch, 0, 10 );
	ImGui::SliderInt ( "recoil yaw", &var::misc::recoil_yaw, 0, 10 );

	ImGui::Separator ( );

	ImGui::Checkbox ( "force automatic", &var::misc::force_automatic );
	ImGui::Checkbox ( "no spread", &var::misc::nospread );
	ImGui::Checkbox ( "spider", &var::misc::spider );
	ImGui::Checkbox ( "insta eoka", &var::misc::eoka );
	ImGui::Checkbox ( "insta compound", &var::misc::compound );
	ImGui::Checkbox ( "omni sprint", &var::misc::omnisprint );
}

// actual render
void DIA4A::UIExternalDirect3DOverlay::m_pUserSideRender ( DIA4A::Direct3DHandlingSystem m_pExternalDirect3DHandler )
{
	renderer::m_pDirect3DSystem = m_pExternalDirect3DHandler;

	renderer::DrawESP ( );

	if (m_bMenuEnabled)
	{
		static int page = 0;

		ImGui::SetNextWindowSize ( ImVec2 ( 800, 400 ), ImGuiCond_::ImGuiCond_FirstUseEver );
		if (ImGui::Begin ( "ricochet", &m_bMenuEnabled, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse ))
		{
			const char* tabs[] = {
				"aim",
				"esp",
				"misc"
			};


			ImGui::BeginChild ( "Main1", ImVec2 ( 0, 50 ), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar );
			{
				for (int i = 0; i < IM_ARRAYSIZE ( tabs ); i++) {
					int distance = i == page ? 0 : i > page ? i - page : page - i;

					ImGui::SameLine ( );
					if (ImGui::Button ( tabs[i], ImVec2 ( ImGui::GetWindowSize ( ).x / IM_ARRAYSIZE ( tabs ) - 18,
						ImGui::GetWindowSize ( ).y - 18 ) ))
						page = i;


					i < IM_ARRAYSIZE ( tabs ) - 1;

				}
				ImGui::EndChild ( );
			}

			ImGui::BeginChild ( "Main2", ImVec2 ( 0, 0 ), true, ImGuiWindowFlags_NoResize );
			{
				switch (page) {
				case 0:
					AimTab ( );
					break;
				case 1:
					EspTab ( );
					break;
				case 2:
					MiscTab ( );
					break;
				}

				ImGui::EndChild ( );
			}

			ImGui::End ( );
		}
	}
}

void renderer::DrawESP ( )
{
	if (fonts::m_pFont == nullptr)
	{
		fonts::m_pFont = m_pDirect3DSystem.m_pCreateFont(18, "Tomorrow");
	}

	D3DRenderLine(20, 20, 50, 50, D3DColor(255, 255, 255) D3DPostFunction);

	D3DRenderText(100, 50, D3DColor(255, 0, 0), fonts::m_pFont, "example text");

	if (!var::esp::enabled)
	{
		return;
	}
	/*
	for (int i = 0; i < menu::esp::m_pEntityList.size ( ); i++)
	{
		EntityInfo entity = menu::esp::m_pEntityList.at ( i );

		float maxDist = 8000.f;

		switch (entity.type)
		{
		case EntityType::Scientist:
		case EntityType::BasePlayer:

			if (menu::esp::m_bLimitPlayerDistance)
				maxDist = menu::esp::m_flPlayerDistance;

			if (!menu::esp::DrawPlayer ( entity, maxDist, menu::esp::m_bPlayerBox, menu::esp::m_bPlayerSkeleton, menu::esp::m_bPlayerName, menu::esp::m_bPlayerDist, menu::esp::m_bPlayerHealthBar, menu::esp::m_bPlayerShotPred, menu::esp::m_bPlayerActiveItem ))
				menu::esp::m_pEntityList.erase ( menu::esp::m_pEntityList.begin ( ) + i-- );

			break;
		default:

			if (var::esp::m_bLimitMiscDistance)
				maxDist = menu::esp::m_flMiscDistance;

			if (!var::esp::DrawOther ( entity, maxDist, menu::esp::m_bMiscName, menu::esp::m_bMiscDist, menu::esp::m_bMiscHealthBar ))
				var::esp::m_pEntityList.erase ( menu::esp::m_pEntityList.begin ( ) + i-- );

			break;
		}
	}
	*/
	
}