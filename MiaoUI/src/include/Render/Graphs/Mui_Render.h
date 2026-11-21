
#pragma once
#include <Render/Graphs/Mui_RenderDef.h>
namespace Mui
{
	namespace Color
	{
		inline _m_color M_RGB(_m_byte r, _m_byte g, _m_byte b)
		{
			return { (_m_byte(r) | (_m_word(g) << 8)) | (_m_ulong(b) << 16) };
		}
		inline _m_color M_RGBA(_m_byte r, _m_byte g, _m_byte b, _m_byte a)
		{
			return M_RGB(r, g, b) | (a << 24);
		}
		inline _m_byte M_GetAValue(_m_color rgba)
		{
			return _m_byte(_m_ulong(rgba >> 24) & 0xff);
		}
		inline _m_byte M_GetRValue(_m_color rgb)
		{
			return _m_byte(_m_ulong(rgb) & 0xff);
		}
		inline _m_byte M_GetGValue(_m_color rgb)
		{
			return _m_byte(_m_ulong(_m_word(rgb) >> 8) & 0xff);
		}
		inline _m_byte M_GetBValue(_m_color rgb)
		{
			return _m_byte(_m_ulong(rgb >> 16) & 0xff);
		}
		inline std::wstring M_RGBA_STR(_m_byte r, _m_byte g, _m_byte b, _m_byte a)
		{
			std::wstring ret = std::to_wstring(r);
			ret += L"," + std::to_wstring(g);
			ret += L"," + std::to_wstring(b);
			ret += L"," + std::to_wstring(a);
			return ret;
		}
		inline std::wstring M_RGBA_STR(_m_color color)
		{
			return M_RGBA_STR(M_GetRValue(color), M_GetGValue(color),
				M_GetBValue(color), M_GetAValue(color));
		}
		constexpr _m_color M_None = 0x00000000;
		constexpr _m_color M_Black = 0xFF000000;
		constexpr _m_color M_White = 0xFFFFFFFF;
		constexpr _m_color M_RED = 0xFF0000FF;
		constexpr _m_color M_GREEN = 0xFF00FF00;
		constexpr _m_color M_BLUE = 0xFFFF0000;
	}
	template<class Interface>
	void MSafeRelease(Interface*& Release)
	{
		if (Release != nullptr)
		{
			Release->Release();
			Release = nullptr;
		}
	}
	namespace Render
	{
		extern void NinePalaceDraw(const std::function<void(_m_rect, _m_rect)>& callback, _m_rect dest, _m_rect src, _m_rect margin);
	}
}