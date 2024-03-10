#pragma once
#include <unordered_map>
#include "xorstr.hpp"

extern "C" void _DoSyscall ( );

inline std::unordered_map< std::string, std::uint32_t > v_syscall_w32k_cache { };
inline std::unordered_map< std::string, std::uint32_t > v_syscall_us32_cache { };


template< typename ReturnType = void, typename... Args, typename T1 = void*, typename T2 = void*, typename T3 = void*, typename T4 = void* >
ReturnType SyscallWin32k ( std::string szProcName, T1 A1 = { }, T2 A2 = { }, T3 A3 = { }, T4 A4 = { }, Args... Arguments )
{
	std::uint32_t Index = 0;

	const auto it = v_syscall_w32k_cache.find ( szProcName );

	if ( it == v_syscall_w32k_cache.end ( ) )
	{
		auto pProc = PUCHAR ( GetProcAddress ( LoadLibraryA ( _ ( "win32u.dll" ) ), szProcName.c_str ( ) ) );
		if ( pProc )
		{
			for ( int i = 0; i < 24; ++i )
			{
				if ( pProc[i] == 0xC2 || pProc[i] == 0xC3 )
					break;

				if ( pProc[i] == 0xB8 )
				{
					Index = *(std::uint32_t*)( pProc + i + 1 );
					break;
				}
			}
		}

		v_syscall_w32k_cache.insert ( { szProcName, Index } );
	}
	else
		Index = it->second;

	return reinterpret_cast<ReturnType ( * )( T1, T2, T3, T4, uint64_t, uint64_t, Args... )>( _DoSyscall )( A1, A2, A3, A4, Index, 0, Arguments... );
}

template< typename ReturnType = void, typename... Args, typename T1 = void*, typename T2 = void*, typename T3 = void*, typename T4 = void* >
ReturnType SyscallUser32 ( std::string szProcName, T1 A1 = { }, T2 A2 = { }, T3 A3 = { }, T4 A4 = { }, Args... Arguments )
{
	std::uint32_t Index = 0;

	const auto it = v_syscall_us32_cache.find ( szProcName );

	if ( it == v_syscall_us32_cache.end ( ) )
	{
		auto pProc = PUCHAR ( GetProcAddress ( LoadLibraryA ( _ ( "user32.dll" ) ), szProcName.c_str ( ) ) );
		if ( pProc )
		{
			for ( int i = 0; i < 24; ++i )
			{
				if ( pProc[i] == 0xC2 || pProc[i] == 0xC3 )
					break;

				if ( pProc[i] == 0xB8 )
				{
					Index = *(std::uint32_t*)( pProc + i + 1 );
					break;
				}
			}
		}

		v_syscall_us32_cache.insert ( { szProcName, Index } );
	}
	else
		Index = it->second;

	return reinterpret_cast<ReturnType ( * )( T1, T2, T3, T4, uint64_t, uint64_t, Args... )>( _DoSyscall )( A1, A2, A3, A4, Index, 0, Arguments... );
}