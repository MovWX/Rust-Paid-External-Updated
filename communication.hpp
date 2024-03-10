#pragma once

#include <windows.h>
#include <winternl.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include <codecvt>
#include "mutex.hpp"

#include "xorstr.hpp"
#include "communication_structures.hpp"

using namespace structures;
class Vector3;

#define alternative(a, b) std::holds_alternative<a>(b)

inline std::string utf16_to_utf8 ( std::u16string utf16_string )
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t*>( utf16_string.data ( ) );
	return convert.to_bytes ( p, p + utf16_string.size ( ) );
}

class CSharedMemComms
{
	HANDLE				hEventDriver = nullptr;
	HANDLE				hEventUser = nullptr;
	HANDLE				hMapping = nullptr;
	PMEMORY_OPERATION   pMemData = nullptr;
	bool                Initialized = false;
	int					iCounter = 0;
	bool				bMutex;
public:

	PINOUT_SHAREDDATA pSharedMemData = nullptr;
	br::mutex guard;
	std::chrono::steady_clock::time_point start;

	CSharedMemComms ( LPCSTR lpName, bool use_mutex ) : bMutex { use_mutex }
	{
		this->hMapping = CreateFileMappingA ( INVALID_HANDLE_VALUE,
			nullptr,
			PAGE_READWRITE,
			0,
			sizeof ( _INOUT_SHAREDDATA ) + sizeof ( _MEMORY_OPERATION ),
			lpName );

		if ( !this->hMapping )
		{
#ifdef _DEBUG
			printf ( "CreateFileMappingA error %d\n", GetLastError ( ) );
			char szDst[256] { };
			sprintf_s ( szDst, "CreateFileMappingA err %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#else
			char szDst[256] { };
			sprintf_s ( szDst, "FAILED MAP %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#endif
			return;
		}

		this->pSharedMemData = reinterpret_cast<PINOUT_SHAREDDATA>( MapViewOfFile ( hMapping,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			sizeof ( _INOUT_SHAREDDATA ) + sizeof ( _MEMORY_OPERATION ) ) );

		if ( !this->pSharedMemData )
		{
#ifdef _DEBUG
			printf ( "MapViewOfFile error %d\n", GetLastError ( ) );
			char szDst[256] { };
			sprintf_s ( szDst, "MapViewOfFile err %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#else
			char szDst[256] { };
			sprintf_s ( szDst, "FAILED MAP (1) %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#endif
			CloseHandle ( this->hMapping );
			return;
		}

		memset ( this->pSharedMemData, 0, sizeof ( _INOUT_SHAREDDATA ) + sizeof ( _MEMORY_OPERATION ) );

		this->pMemData = reinterpret_cast<PMEMORY_OPERATION>( this->pSharedMemData + 1 );
		this->hEventUser = CreateEvent ( nullptr, false, false, nullptr );

		if ( !this->hEventUser )
		{
#ifdef _DEBUG
			printf ( "CreateEvent error %d\n", GetLastError ( ) );
			char szDst[256] { };
			sprintf_s ( szDst, "CreateEvent err %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#else
			char szDst[256] { };
			sprintf_s ( szDst, "FAILED EVENT %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#endif
			UnmapViewOfFile ( this->pSharedMemData );
			CloseHandle ( this->hMapping );

			return;
		}

		this->hEventDriver = CreateEvent ( nullptr, false, false, nullptr );

		if ( !this->hEventDriver )
		{
#ifdef _DEBUG
			printf ( "CreateEvent error %d\n", GetLastError ( ) );
			char szDst[256] { };
			sprintf_s ( szDst, "CreateEvent err %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#else
			char szDst[256] { };
			sprintf_s ( szDst, "FAILED EVENT (1) %d\n", GetLastError ( ) );
			MessageBoxA ( 0, szDst, 0, 0 );
#endif
			UnmapViewOfFile ( this->pSharedMemData );
			CloseHandle ( this->hMapping );
			CloseHandle ( this->hEventUser );

			return;
		}

		*reinterpret_cast<DWORD*>( &this->pSharedMemData->ProcessId ) = GetCurrentProcessId ( );
		this->pSharedMemData->hEventDriver = this->hEventDriver;
		this->pSharedMemData->hEventUser = this->hEventUser;
#ifdef _DEBUG
		printf ( "Shared memory created\n" );
#endif
	}

	~CSharedMemComms ( )
	{
		if ( this->Initialized )
		{
			this->pSharedMemData->Finish = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			UnmapViewOfFile ( this->pSharedMemData );
			CloseHandle ( this->hMapping );
			CloseHandle ( this->hEventUser );
			CloseHandle ( this->hEventDriver );
		}
	}

	void Initialize ( )
	{
		if ( WaitForSingleObject ( this->hEventUser, 60000 ) == WAIT_TIMEOUT )
		{
			UnmapViewOfFile ( this->pSharedMemData );
			CloseHandle ( this->hMapping );
			CloseHandle ( this->hEventUser );
			CloseHandle ( this->hEventDriver );
		}
		else
			this->Initialized = true;

		start = std::chrono::steady_clock::now ( );
	}

	template <typename t>
	t read ( const uint64_t address )
	{
		//static_assert( std::is_trivially_copyable_v<t> );

		if ( address < 0xffffff || address > 0x7fffffff0000 )
			return t();

		if ( bMutex ) {
			const br::locker noob ( this->guard );

			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return t ( );

			t buffer;

			this->pMemData->control_code = control_codes::read;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( address );
			pRead->buffer = &buffer;
			pRead->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );
			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return buffer;
		}
		else
		{
			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return t ( );

			t buffer;

			this->pMemData->control_code = control_codes::read;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( address );
			pRead->buffer = &buffer;
			pRead->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );
			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return buffer;
		}
	}

	template <typename t>
	bool read ( const uintptr_t address, const t buffer, const size_t size )
	{
		//static_assert( std::is_trivially_copyable_v<t> );

		if ( address < 0xffffff || address > 0x7fffffff0000 )
			return false;

		if ( bMutex )
		{
			const br::locker noob ( this->guard );

			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::read;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( address );
			pRead->buffer = reinterpret_cast<void*>( buffer );
			pRead->size = size;

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
		else
		{
			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::read;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( address );
			pRead->buffer = reinterpret_cast<void*>( buffer );
			pRead->size = size;

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
	}

	template <typename t>
	bool write ( const uintptr_t address, t buffer )
	{
		//static_assert( std::is_trivially_copyable_v<t> );

		if ( address < 0xffffff || address > 0x7fffffff0000 )
			return false;

		if ( bMutex )
		{
			const br::locker noob ( this->guard );

			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::write;

			auto pWrite = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pWrite->address = reinterpret_cast<void*>( address );
			pWrite->buffer = reinterpret_cast<void*>( &buffer );
			pWrite->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
		else
		{
			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::write;

			auto pWrite = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pWrite->address = reinterpret_cast<void*>( address );
			pWrite->buffer = reinterpret_cast<void*>( &buffer );
			pWrite->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
	}

	template <typename t>
	bool write ( const uintptr_t address, const t buffer, const size_t size )
	{
		//static_assert( std::is_trivially_copyable_v<t> );

		if ( address < 0xffffff || address > 0x7fffffff0000 )
			return false;

		if ( bMutex )
		{
			const br::locker noob ( this->guard );

			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::write;

			auto pWrite = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pWrite->address = reinterpret_cast<void*>( address );
			pWrite->buffer = reinterpret_cast<void*>( buffer );
			pWrite->size = size;

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
		else
		{
			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return false;

			this->pMemData->control_code = control_codes::write;

			auto pWrite = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pWrite->address = reinterpret_cast<void*>( address );
			pWrite->buffer = reinterpret_cast<void*>( buffer );
			pWrite->size = size;

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			const auto result = this->pMemData->status;

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return NT_SUCCESS ( result );
		}
	}

	bool attach ( const uint32_t hash )
	{
		const br::locker noob ( this->guard );

		if ( !this->Initialized || this->pSharedMemData->DataWritten )
			return false;

		this->pMemData->control_code = control_codes::attach_process;

		auto pAttach = reinterpret_cast<c_attach_process_request*>( this->pMemData->Data );
		pAttach->hash = hash;

		this->pSharedMemData->DataWritten = true;
		SetEvent ( this->hEventDriver );

		WaitForSingleObject ( this->hEventUser, INFINITE );

		const auto result = this->pMemData->status;

		memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

		this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
		iCounter++;
#endif
		return NT_SUCCESS ( result );
	}

	bool detach ( )
	{
		const br::locker noob ( this->guard );

		if ( !this->Initialized || this->pSharedMemData->DataWritten )
			return false;

		this->pMemData->control_code = control_codes::detach_process;

		this->pSharedMemData->DataWritten = true;
		SetEvent ( this->hEventDriver );

		WaitForSingleObject ( this->hEventUser, INFINITE );

		const auto result = this->pMemData->status;

		memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

		this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
		iCounter++;
#endif
		return NT_SUCCESS ( result );
	}

	uintptr_t find ( const uint32_t hash )
	{
		uintptr_t result = NULL;
		const br::locker noob ( this->guard );

		if ( !this->Initialized || this->pSharedMemData->DataWritten )
			return NULL;

		this->pMemData->control_code = control_codes::find_module;

		auto pFindModule = reinterpret_cast<c_find_module_request*>( this->pMemData->Data );
		pFindModule->hash = hash;

		this->pSharedMemData->DataWritten = true;
		SetEvent ( this->hEventDriver );

		WaitForSingleObject ( this->hEventUser, INFINITE );

		result = pFindModule->res;

		memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

		this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
		iCounter++;
#endif
		return result;
	}

	bool get_key ( const int key )
	{

		bool result = false;

		if ( bMutex )
			const br::locker noob ( this->guard );
		if ( !this->Initialized || this->pSharedMemData->DataWritten )
			return false;

		this->pMemData->control_code = control_codes::key_state;

		auto pGetKey = reinterpret_cast<c_key_state_request*>( this->pMemData->Data );
		pGetKey->key = key;

		this->pSharedMemData->DataWritten = true;
		SetEvent ( this->hEventDriver );

		WaitForSingleObject ( this->hEventUser, INFINITE );

		result = pGetKey->res;

		memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

		this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
		iCounter++;
#endif
		return result;
	}

	bool hide_overlay ( uintptr_t window_handle )
	{
		bool result = false;

		if ( bMutex )
			const br::locker noob ( this->guard );
		if ( !this->Initialized || this->pSharedMemData->DataWritten )
			return false;

		this->pMemData->control_code = control_codes::hide_window;

		auto pHideOverlay = reinterpret_cast<c_hide_overlay_request*>( this->pMemData->Data );
		pHideOverlay->window_handle = window_handle;

		this->pSharedMemData->DataWritten = true;
		SetEvent ( this->hEventDriver );

		WaitForSingleObject ( this->hEventUser, INFINITE );

		result = pHideOverlay->res;

		memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );

		this->pSharedMemData->DataWritten = false;
		return result;
	}

	template <typename t>
	t get_position_injected ( const uintptr_t transform )
	{
		if ( transform < 0xffffff || transform > 0x7fffffff0000 )
			return t ( );

		if ( bMutex ) {
			const br::locker noob ( this->guard );

			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return t ( );

			t buffer;

			this->pMemData->control_code = control_codes::read_injected;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( transform );
			pRead->buffer = &buffer;
			pRead->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );
			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return buffer;
		}
		else
		{
			if ( !this->Initialized || this->pSharedMemData->DataWritten )
				return t ( );

			t buffer;

			this->pMemData->control_code = control_codes::read_injected;

			auto pRead = reinterpret_cast<c_read_write_request*>( this->pMemData->Data );
			pRead->address = reinterpret_cast<void*>( transform );
			pRead->buffer = &buffer;
			pRead->size = sizeof ( t );

			this->pSharedMemData->DataWritten = true;
			SetEvent ( this->hEventDriver );

			WaitForSingleObject ( this->hEventUser, INFINITE );

			memset ( this->pMemData, 0, sizeof ( _MEMORY_OPERATION ) );
			this->pSharedMemData->DataWritten = false;
#ifdef _DEBUG
			iCounter++;
#endif
			return buffer;
		}
	}

	std::string read_ascii ( const uintptr_t address, const size_t size )
	{
		std::unique_ptr<char[]> buffer ( new char[size] );
		if ( !read<char*> ( address, buffer.get ( ), size ) )
			return std::string ( );
		return std::string ( buffer.get ( ) );
	}

	std::string read_unicode ( const uintptr_t address, const int stringLength )
	{
		char16_t wcharTemp[128] = { '\0' };

		if ( !read<char16_t*> ( address, wcharTemp, stringLength * 2 ) )
			return std::string ( "" );

		std::string u8_conv = utf16_to_utf8 ( wcharTemp );

		return u8_conv;
	}

	std::wstring wread_unicode ( const uintptr_t address, const int stringLength )
	{
		char16_t wcharTemp[128] = { '\0' };

		if ( !read<char16_t*> ( address, wcharTemp, stringLength * 2 ) )
			return std::wstring ( L"" );

		return std::wstring ( reinterpret_cast<wchar_t*>( wcharTemp ) );
	}
};

namespace mist
{
	extern std::unique_ptr< CSharedMemComms > esp;
	extern std::unique_ptr< CSharedMemComms > aim;
	extern std::unique_ptr< CSharedMemComms > obj;
	extern std::unique_ptr< CSharedMemComms > misc;
	extern std::unique_ptr< CSharedMemComms > other;
	extern std::unique_ptr< CSharedMemComms > wep;
};