#include "stdafx.hpp"
#include "overlay.hpp"
#include "math.hpp"

std::unique_ptr< Renderer > renderer{ };

std::string correct_non_utf_8 ( const std::string& str )
{
	int i, f_size = str.size ( );
	unsigned char c, c2, c3, c4;
	std::string to;
	to.reserve ( f_size );

	for ( i = 0; i < f_size; i++ ) {
		c = (unsigned char)( str )[i];
		if ( c < 32 ) {//control char
			continue;
		}
		else if ( c < 127 ) {//normal ASCII
			to.append ( 1, c );
			continue;
		}
		else if ( c < 160 ) {//control char (nothing should be defined here either ASCI, ISO_8859-1 or UTF8, so skipping)
			if ( c2 == 128 ) {//fix microsoft mess, add euro
				to.append ( 1, 226 );
				to.append ( 1, 130 );
				to.append ( 1, 172 );
			}
			if ( c2 == 133 ) {//fix IBM mess, add NEL = \n\r
				to.append ( 1, 10 );
				to.append ( 1, 13 );
			}
			continue;
		}
		else if ( c < 192 ) {//invalid for UTF8, converting ASCII
			to.append ( 1, (unsigned char)194 );
			to.append ( 1, c );
			continue;
		}
		else if ( c < 194 ) {//invalid for UTF8, converting ASCII
			to.append ( 1, (unsigned char)195 );
			to.append ( 1, c - 64 );
			continue;
		}
		else if ( c < 224 && i + 1 < f_size ) {//possibly 2byte UTF8
			c2 = (unsigned char)( str )[i + 1];
			if ( c2 > 127 && c2 < 192 ) {//valid 2byte UTF8
				if ( c == 194 && c2 < 160 ) {//control char, skipping
					;
				}
				else {
					to.append ( 1, c );
					to.append ( 1, c2 );
				}
				i++;
				continue;
			}
		}
		else if ( c < 240 && i + 2 < f_size ) {//possibly 3byte UTF8
			c2 = (unsigned char)( str )[i + 1];
			c3 = (unsigned char)( str )[i + 2];
			if ( c2 > 127 && c2 < 192 && c3>127 && c3 < 192 ) {//valid 3byte UTF8
				to.append ( 1, c );
				to.append ( 1, c2 );
				to.append ( 1, c3 );
				i += 2;
				continue;
			}
		}
		else if ( c < 245 && i + 3 < f_size ) {//possibly 4byte UTF8
			c2 = (unsigned char)( str )[i + 1];
			c3 = (unsigned char)( str )[i + 2];
			c4 = (unsigned char)( str )[i + 3];
			if ( c2 > 127 && c2 < 192 && c3>127 && c3 < 192 && c4>127 && c4 < 192 ) {//valid 4byte UTF8
				to.append ( 1, c );
				to.append ( 1, c2 );
				to.append ( 1, c3 );
				to.append ( 1, c4 );
				i += 3;
				continue;
			}
		}
		//invalid UTF8, converting ASCII (c>245 || string too short for multi-byte))
		to.append ( 1, (unsigned char)195 );
		to.append ( 1, c - 64 );
	}
	return to;
}

ImVec2 Renderer::measure_text ( const std::string& str )
{
	std::string sanitized = correct_non_utf_8 ( str );
	return ImGui::CalcTextSize ( sanitized.c_str ( ) );
}

void Renderer::draw_text ( float x, float y, const std::string& str, ImColor color, bool shadow, bool center )
{
	if ( str.empty ( ) )
		return;

	if ( x < 0 || y < 0 || x > 3840 || y > 2160 )
		return;

	ImVec2 origin{ };
	std::string sanitized = correct_non_utf_8 ( str );
	if ( center )
	{
		ImVec2 size = ImGui::CalcTextSize ( sanitized.c_str ( ) );
		origin = ImVec2 ( size.x / 2, size.y / 2 );
	}

	x = ( x - origin.x );
	y = ( y - origin.y );
	if ( shadow )
	{
		auto black = ImColor{ 0, 0, 0, 255 };

		m_pDrawList->AddText ( { x + 1, y }, black, sanitized.c_str ( ) );
		m_pDrawList->AddText ( { x - 1, y }, black, sanitized.c_str ( ) );
		m_pDrawList->AddText ( { x, y + 1 }, black, sanitized.c_str ( ) );
		m_pDrawList->AddText ( { x, y - 1 }, black, sanitized.c_str ( ) );
	}
	m_pDrawList->AddText ( { x, y }, color, sanitized.c_str ( ) );
}

void Renderer::draw_line ( ImVec2 pos1, ImVec2 pos2, ImU32 color )
{
	if ( pos1[0] < 0 || pos1[1] < 0 || pos1[0] > 3840 || pos1[1] > 2160 || pos2[0] < 0 || pos2[1] < 0 || pos2[0] > 3840 || pos2[1] > 2160 )
		return;

	m_pDrawList->AddLine ( pos1, pos2, color );
}

void Renderer::draw_circle ( float x, float y, float radius, ImU32 color )
{
	if ( x < 0 || y < 0 || x > 3840 || y > 2160 )
		return;

	m_pDrawList->AddCircle ( { x, y }, radius, color );
}

void Renderer::draw_rectangle ( float x, float y, float width, float height, ImU32 color )
{
	if ( x < 0 || y < 0 || x > 3840 || y > 2160 )
		return;

	m_pDrawList->AddRectFilled ( { x, y }, { x + width, y + height }, color );
}

void Renderer::draw_outline ( float x, float y, float width, float height, float thickness, ImU32 color )
{
	if ( x < 0 || y < 0 || x > 3840 || y > 2160 )
		return;

	thickness = max ( 1.f, thickness );

	this->draw_rectangle ( x, y - thickness, width + 1, thickness, color );
	this->draw_rectangle ( x, y + height, width + 1, thickness, color );

	this->draw_rectangle ( x - thickness, y - thickness, thickness, height + thickness * 2, color );
	this->draw_rectangle ( x + width, y - thickness, thickness, height + thickness * 2, color );
}

void Renderer::draw_filled_circle ( float x, float y, float radius, ImU32 color )
{
	if ( x < 0 || y < 0 || x > 3840 || y > 2160 )
		return;

	m_pDrawList->AddCircleFilled ( { x, y }, radius, color );
}