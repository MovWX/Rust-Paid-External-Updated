#pragma once
#ifndef _STDAFX_GUARD
#define _STDAFX_GUARD

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"


/* BASE LIBS */
#include <iostream>
#include <string>
#include <vector>
#include <comdef.h>
#include <mutex>
#include <array>

/* MATH */
#include <algorithm>
#include <cmath>

/* THREADING */
#include <future>
#include <thread>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <directxmath.h>
#define NOMINMAX

using namespace DirectX;

#include <d3d11.h>
#include <cassert>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <windowsx.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <d3d9types.h>
#include <d3d11.h>
#include <tchar.h>

//#include "themida/ThemidaSDK.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

#include <wrl/client.h>
#include <dcomp.h>
#include <dxgi.h>
#include <dxgi1_2.h>

#pragma comment( lib, "dxgi" )
#pragma comment( lib, "dcomp" )

inline HWND window_handle = nullptr, target_window = nullptr;

#define DBG_BREAK [] ( ) { }
#endif

#define CURL_STATICLIB

#include "curl/curl.h"
#include "curl/easy.h"

#include "json.hpp"
using json = nlohmann::json;