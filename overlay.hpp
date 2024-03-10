#pragma once
#ifndef GETREKT
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#else
#include "imguirekt/imgui.h"
#include "imguirekt/imgui_impl_win32.h"
#include "imguirekt/imgui_impl_dx11.h"
#endif
namespace ImColors {
    inline ImColor AliceBlue = { 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f };
    inline ImColor AntiqueWhite = { 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f };
    inline ImColor Aqua = { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor Aquamarine = { 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f };
    inline ImColor Azure = { 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor Beige = { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f };
    inline ImColor Bisque = { 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f };
    inline ImColor Black = { 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor BlanchedAlmond = { 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f };
    inline ImColor Blue = { 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor BlueViolet = { 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f };
    inline ImColor Brown = { 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f };
    inline ImColor BurlyWood = { 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f };
    inline ImColor CadetBlue = { 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f };
    inline ImColor Chartreuse = { 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor Chocolate = { 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f };
    inline ImColor Coral = { 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
    inline ImColor CornflowerBlue = { 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f };
    inline ImColor Cornsilk = { 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f };
    inline ImColor Crimson = { 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f };
    inline ImColor Cyan = { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor DarkBlue = { 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f };
    inline ImColor DarkCyan = { 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
    inline ImColor DarkGoldenrod = { 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f };
    inline ImColor DarkGray = { 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f };
    inline ImColor DarkGreen = { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f };
    inline ImColor DarkKhaki = { 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f };
    inline ImColor DarkMagenta = { 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f };
    inline ImColor DarkOliveGreen = { 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f };
    inline ImColor DarkOrange = { 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
    inline ImColor DarkOrchid = { 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f };
    inline ImColor DarkRed = { 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor DarkSalmon = { 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f };
    inline ImColor DarkSeaGreen = { 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f };
    inline ImColor DarkSlateBlue = { 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f };
    inline ImColor DarkSlateGray = { 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f };
    inline ImColor DarkTurquoise = { 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f };
    inline ImColor DarkViolet = { 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f };
    inline ImColor DeepPink = { 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f };
    inline ImColor DeepSkyBlue = { 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f };
    inline ImColor DimGray = { 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f };
    inline ImColor DodgerBlue = { 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f };
    inline ImColor Firebrick = { 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f };
    inline ImColor FloralWhite = { 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f };
    inline ImColor ForestGreen = { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f };
    inline ImColor Fuchsia = { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor Gainsboro = { 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f };
    inline ImColor GhostWhite = { 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f };
    inline ImColor Gold = { 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f };
    inline ImColor Goldenrod = { 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f };
    inline ImColor Gray = { 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f };
    inline ImColor Green = { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f };
    inline ImColor GreenYellow = { 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f };
    inline ImColor Honeydew = { 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f };
    inline ImColor HotPink = { 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f };
    inline ImColor IndianRed = { 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f };
    inline ImColor Indigo = { 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f };
    inline ImColor Ivory = { 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f };
    inline ImColor Khaki = { 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f };
    inline ImColor Lavender = { 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f };
    inline ImColor LavenderBlush = { 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f };
    inline ImColor LawnGreen = { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f };
    inline ImColor LemonChiffon = { 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f };
    inline ImColor LightBlue = { 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f };
    inline ImColor LightCoral = { 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f };
    inline ImColor LightCyan = { 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor LightGoldenrodYellow = { 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f };
    inline ImColor LightGreen = { 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f };
    inline ImColor LightGray = { 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f };
    inline ImColor LightPink = { 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f };
    inline ImColor LightSalmon = { 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f };
    inline ImColor LightSeaGreen = { 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f };
    inline ImColor LightSkyBlue = { 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f };
    inline ImColor LightSlateGray = { 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f };
    inline ImColor LightSteelBlue = { 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f };
    inline ImColor LightYellow = { 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f };
    inline ImColor Lime = { 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor LimeGreen = { 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f };
    inline ImColor Linen = { 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f };
    inline ImColor Magenta = { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor Maroon = { 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor MediumAquamarine = { 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f };
    inline ImColor MediumBlue = { 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f };
    inline ImColor MediumOrchid = { 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f };
    inline ImColor MediumPurple = { 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f };
    inline ImColor MediumSeaGreen = { 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f };
    inline ImColor MediumSlateBlue = { 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f };
    inline ImColor MediumSpringGreen = { 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f };
    inline ImColor MediumTurquoise = { 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f };
    inline ImColor MediumVioletRed = { 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f };
    inline ImColor MidnightBlue = { 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f };
    inline ImColor MintCream = { 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f };
    inline ImColor MistyRose = { 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f };
    inline ImColor Moccasin = { 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f };
    inline ImColor NavajoWhite = { 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f };
    inline ImColor Navy = { 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f };
    inline ImColor OldLace = { 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f };
    inline ImColor Olive = { 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f };
    inline ImColor OliveDrab = { 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f };
    inline ImColor Orange = { 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f };
    inline ImColor OrangeRed = { 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f };
    inline ImColor Orchid = { 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f };
    inline ImColor PaleGoldenrod = { 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f };
    inline ImColor PaleGreen = { 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f };
    inline ImColor PaleTurquoise = { 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f };
    inline ImColor PaleVioletRed = { 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f };
    inline ImColor PapayaWhip = { 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f };
    inline ImColor PeachPuff = { 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f };
    inline ImColor Peru = { 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f };
    inline ImColor Pink = { 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f };
    inline ImColor Plum = { 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f };
    inline ImColor PowderBlue = { 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f };
    inline ImColor Purple = { 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f };
    inline ImColor Red = { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor RosyBrown = { 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f };
    inline ImColor RoyalBlue = { 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f };
    inline ImColor SaddleBrown = { 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f };
    inline ImColor Salmon = { 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f };
    inline ImColor SandyBrown = { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f };
    inline ImColor SeaGreen = { 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f };
    inline ImColor SeaShell = { 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f };
    inline ImColor Sienna = { 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f };
    inline ImColor Silver = { 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f };
    inline ImColor SkyBlue = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
    inline ImColor SlateBlue = { 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f };
    inline ImColor SlateGray = { 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f };
    inline ImColor Snow = { 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f };
    inline ImColor SpringGreen = { 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f };
    inline ImColor SteelBlue = { 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f };
    inline ImColor Tan = { 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f };
    inline ImColor Teal = { 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f };
    inline ImColor Thistle = { 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f };
    inline ImColor Tomato = { 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f };
    inline ImColor Transparent = { 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f };
    inline ImColor Turquoise = { 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f };
    inline ImColor Violet = { 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f };
    inline ImColor Wheat = { 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f };
    inline ImColor White = { 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
    inline ImColor WhiteSmoke = { 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f };
    inline ImColor Yellow = { 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
    inline ImColor YellowGreen = { 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f };
}

class Renderer
{
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceCtx = nullptr;

public:
	ImDrawList* m_pDrawList;
	ImFont noob;
	Renderer( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceCtx, int iWidth, int iHeight ) :
		m_pDevice( pDevice ), 
		m_pDeviceCtx( pDeviceCtx ) {};

	~Renderer() = default;

    ImVec2 measure_text ( const std::string& str );
	void draw_text( float x, float y, const std::string& str, ImColor color, bool shadow = true, bool center = false );
	void draw_line( ImVec2 pos1, ImVec2 pos2, ImU32 color );
	void draw_rectangle( float x, float y, float width, float height, ImU32 color );
	void draw_outline( float x, float y, float width, float height, float thickness, ImU32 color );
	void draw_circle( float x, float y, float radius, ImU32 color );
	void draw_filled_circle( float x, float y, float radius, ImU32 color );
};

extern std::unique_ptr< Renderer > renderer;