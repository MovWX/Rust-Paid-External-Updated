#ifndef _ESP_GUARD
#define _ESP_GUARD

#include "cheat.hpp"
#include "include.hpp"
#include <d3dx9core.h>
#include "overlay/Direct3DHandling.h"

#define D3DColor DIA4A::Direct3D::Direct3DColor
#define D3DFont LPD3DXFONT
#define D3DRenderRectangle DIA4A::Direct3D::RenderRectangle
#define D3DRenderTriangle DIA4A::Direct3D::RenderTriangle
#define D3DRenderLine DIA4A::Direct3D::RenderLine
#define D3DRenderCircle DIA4A::Direct3D::RenderCircle
#define D3DRenderFilledCircle DIA4A::Direct3D::RenderFilledCircle
#define D3DRenderText DIA4A::Direct3D::m_pRenderText
#define D3DGetTextSize DIA4A::Direct3D::m_pGetTextSize
#define D3DPostFunction , m_pDirect3DSystem.m_pDirect3DDevice

namespace renderer {
	void OverlayThread ( );
	void DrawESP ( );

	inline DIA4A::Direct3DHandlingSystem m_pDirect3DSystem;

	namespace fonts
	{
		inline D3DFont m_pFont = nullptr;
	}
};

#endif