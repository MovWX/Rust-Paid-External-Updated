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

void CheckIfOverlayShouldDraw ( )
{
	while (true)
	{
		EnumWindows ( EnumWindowsCallbackFindRust, NULL );

		sleep_for ( milliseconds ( 1000 ) );
	}
}

bool m_bMenuEnabled = false;

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
			sleep_for ( milliseconds ( 10 ) );
			continue;
		}

		if (false) // not in game, no local
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = false;
			sleep_for ( milliseconds ( 500 ) );
			continue;
		}

		uintptr_t playerInput = driver::read<uintptr_t> ( (uintptr_t)rust::localplayer + offsets::player_input );

		if (!playerInput)
		{
			DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = false;
			sleep_for ( milliseconds ( 100 ) );
			continue;
		}

		DIA4A::UIExternalDirect3DOverlay::m_bRenderOverlay = driver::read<bool> ( playerInput + 0x94 );

		sleep_for ( milliseconds ( 25 ) );
	}
}