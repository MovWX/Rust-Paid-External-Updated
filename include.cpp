#include "include.hpp"
#include "math.hpp"

#pragma comment(lib, "ntdll")

HWND syscalls::find_window ( const char* szClassName, const char* szWindowName )
{
	ANSI_STRING classname, wndname;
	UNICODE_STRING classname_unicode { }, wndname_unicode { };

	if ( !szWindowName && !szClassName )
		return NULL;

	if ( szWindowName )
	{
		RtlInitAnsiString ( &wndname, szWindowName );
		RtlAnsiStringToUnicodeString ( &wndname_unicode, &wndname, TRUE );
	}

	if ( szClassName )
	{
		RtlAnsiStringToUnicodeString ( &classname_unicode, &classname, TRUE );
		RtlInitAnsiString ( &classname, szClassName );
	}

	auto res = SyscallWin32k< HWND > ( _ ( "NtUserFindWindowEx" ), NULL, NULL, &classname_unicode, &wndname_unicode, NULL );

	if ( classname_unicode.Length > 0 )
		RtlFreeUnicodeString ( &classname_unicode );

	if ( wndname_unicode.Length > 0 )
		RtlFreeUnicodeString ( &wndname_unicode );

	return res;
}

SHORT syscalls::get_async_key_state ( SHORT vk )
{
	return SyscallWin32k< SHORT > ( _ ( "NtUserGetAsyncKeyState" ), vk );
}

HWND syscalls::get_foreground_window ( )
{
	return SyscallUser32< HWND > ( _ ( "NtUserGetForegroundWindow" ) );
}

BOOL syscalls::show_window ( HWND hWnd, int nCmdShow )
{
	return SyscallUser32< BOOL > ( _ ( "NtUserShowWindow" ), hWnd, nCmdShow );
}

BOOL syscalls::set_window_pos ( HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags )
{
	return SyscallWin32k< BOOL > ( _ ( "NtUserSetWindowPos" ), hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags );
}

HWND syscalls::get_window ( HWND hWnd, UINT uCmd )
{
	return SyscallWin32k< HWND > ( _ ( "NtUserGetWindow" ), hWnd, uCmd );
}

int syscalls::set_window_rgn ( HWND hWnd, HRGN hRgn, BOOL bRedraw )
{
	auto res = SyscallUser32< int > ( _ ( "NtUserSetWindowRgn" ), hWnd, hRgn, bRedraw );
	if ( res )
		DeleteObject ( hRgn );

	return res;
}