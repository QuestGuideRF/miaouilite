
#pragma once
#include <Mui_Base.h>
namespace Mui::Helper
{
	inline _m_long M_MAKELONG(_m_word a, _m_word b)
	{
		return _m_long(_m_ushort(a & 0xffff) | _m_ulong(_m_ushort(b & 0xffff)) << 16);
	}
	inline _m_word M_LOWORD(_m_long l)
	{
		return _m_word(_m_uint(l) & 0xffff);
	}
	inline _m_word M_HIWORD(_m_long l)
	{
		return _m_word(_m_uint(l) >> 16) & 0xffff;
	}
	inline UIPoint M_GetMouseEventPt(_m_param lParam)
	{
		UIPoint pt;
		pt.x = static_cast<int>((short)M_LOWORD((_m_long)lParam));
		pt.y = static_cast<int>((short)M_HIWORD((_m_long)lParam));
		return pt;
	}
	template <typename T>
	T M_MAX(T a, T b)
	{
		return a > b ? a : b;
	}
	template <typename T>
	T M_MIN(T a, T b)
	{
		return a < b ? a : b;
	}
	template <typename T>
	T M_Clamp(T min, T max, T value)
	{
		if (value > max)
			return max;
		if (value < min)
			return min;
		return value;
	}
	template <typename T>
	bool M_DecimalEquals(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
	{
		return std::fabs(a - b) < epsilon;
	}
	template <typename T, std::size_t N1, std::size_t N2, std::size_t... I1, std::size_t... I2>
	constexpr std::array<T, N1 + N2> MergeArrayImpl(const std::array<T, N1>& a, const std::array<T, N2>& b,
		std::index_sequence<I1...>, std::index_sequence<I2...>)
	{
		return { a[I1]..., b[I2]... };
	}
	template <typename T, std::size_t N>
	constexpr std::array<T, N> MergeArrays(const std::array<T, N>& a)
	{
		return a;
	}
	template <typename T, std::size_t N1, std::size_t N2>
	constexpr std::array<T, N1 + N2> MergeArrays(const std::array<T, N1>& a, const std::array<T, N2>& b)
	{
		return MergeArrayImpl(a, b, std::make_index_sequence<N1>{}, std::make_index_sequence<N2>{});
	}
	template <typename T, std::size_t N1, std::size_t N2, typename... Rest>
	constexpr auto MergeArrays(const std::array<T, N1>& a, const std::array<T, N2>& b, const Rest&... rest)
	{
		return MergeArrays(MergeArrays(a, b), rest...);
	}
	template <class T, size_t Size, size_t... Idx>
	constexpr std::array<std::remove_cv_t<T>, Size> ToArrayImpl(T(&& Array)[Size], std::index_sequence<Idx...>)
	{
		return { {std::move(Array[Idx])...} };
	}
	template <class T, size_t Size>
	constexpr std::array<std::remove_cv_t<T>, Size> ToArray(T(&& Array)[Size])
	{
		return ToArrayImpl(std::move(Array), std::make_index_sequence<Size>{});
	}
	template <typename T, std::size_t Size>
	constexpr std::vector<T> ArrayToVector(const std::array<T, Size>& a)
	{
		std::vector<T> vec(Size);
		for (size_t i = 0; i < Size; ++i)
		{
			vec[i] = a[i];
		}
		return vec;
	}
	extern int M_CalcAspectRatio(int fromWidth, int fromHeight, int toWidthOrHeight, bool isWidth);
	extern std::string M_WStringToString(std::wstring_view width);
	extern std::wstring M_StringToWString(std::string_view str);
	extern std::wstring M_ReplaceString(std::wstring str, std::wstring_view old_value, std::wstring_view new_value);
	extern _m_uint M_GetTextCount(std::wstring_view src, std::wstring_view text);
	extern std::wstring M_GetTextLine(std::wstring_view src, _m_uint line);
	extern _m_int M_StoInt(std::wstring_view str);
	extern _m_long M_StoLong(std::wstring_view str);
	extern _m_long64 M_StoLong64(std::wstring_view str);
	extern _m_ulong M_StoULong(std::wstring_view str);
	extern _m_ulong64 M_StoULong64(std::wstring_view str);
	extern float M_StoFloat(std::wstring_view str);
	extern double M_StoDouble(std::wstring_view str);
	extern void M_GetAttribValue(std::wstring_view value, std::vector<std::wstring>& destValue, _m_uint count);
	extern void M_GetAttribValueInt(std::wstring_view value, std::vector<int>& destValue, _m_uint count);
	extern _m_color M_GetAttribValueColor(std::wstring_view value);
	namespace Rect
	{
		extern bool IsCross(const _m_rect& rect1, const _m_rect& rect2);
		extern bool IsPtInside(const _m_rect& rect, const UIPoint& point);
		extern void Offset(_m_rect* rect, int dx, int dy);
		extern void Inflate(_m_rect* rect, int dx, int dy);
		extern void Union(_m_rect* dst, const _m_rect* rect1, const _m_rect* rect2);
		extern void Intersect(_m_rect* dst, const _m_rect* rect1, const _m_rect* rect2);
	}
	extern void M_Sleep(_m_uint ms);
}