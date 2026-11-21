
#pragma once
#include <Render/Graphs/Mui_Render.h>
#include <Window/Mui_Windows.h>
#include <comdef.h>
#include <gdiplus.h>
#pragma comment(lib, "GdiPlus.lib")
namespace Mui::Render
{
	using namespace Def;
	class MBitmap_GDIP : public MBitmap
	{
	public:
		UISize GetSize() override { return m_size; }
		int GetWidth()	override { return m_size.width; }
		int GetHeight() override { return m_size.height; }
	protected:
		void ReleaseThis() override;
		UISize m_size;
		HBITMAP m_bitmap = nullptr;
		HDC m_hdc = nullptr;
		friend class MRender_GDIP;
	};
	class MCanvas_GDIP : public MCanvas
	{
	public:
		UISize GetSize() override { return m_size; }
		int GetWidth() override { return m_size.width; }
		int GetHeight() override { return m_size.height; }
		_m_param GetFlag() override { return 0; }
		_m_rect GetSubRect() override;
	protected:
		void ReleaseThis() override;
		UISize m_size;
		HBITMAP m_bitmap = nullptr;
		HDC m_hdc = nullptr;
		friend class Mui::Window::UIWindowsWnd;
		friend class MRender_GDIP;
	};
	class MPen_GDIP : public MPen
	{
	public:
		void SetColor(_m_color color) override;
		void SetWidth(_m_uint width) override;
		void SetOpacity(_m_byte alpha) override;
		void SetWidthAndColor(_m_uint width, _m_color color) override;
		_m_color GetColor() override;
		_m_uint GetWidth() override;
		_m_byte GetOpacity() override;
	protected:
		void ReleaseThis() override;
		_m_color m_colorSrc = 0;
		_m_byte m_alpha = 255;
		Gdiplus::Pen* m_pen = nullptr;
		friend class MRender_GDIP;
		friend class MFont_GDIP;
	};
	class MBrush_GDIP : public MBrush
	{
	public:
		void SetColor(_m_color color) override;
		void SetOpacity(_m_byte alpha) override;
		_m_color GetColor() override;
		_m_byte GetOpacity() override;
	protected:
		void ReleaseThis() override;
		_m_color m_colorSrc = 0;
		_m_byte m_alpha = 255;
		Gdiplus::SolidBrush* m_brush = nullptr;
		friend class MRender_GDIP;
		friend class MFont_GDIP;
	};
	class MGradientBrush_GDIP : public MGradientBrush
	{
	public:
		_m_uint GetColorPosCount() override;
		_m_color GetPosColor(_m_uint index) override;
		void SetOpacity(_m_byte alpha) override;
		_m_byte GetOpacity() override;
		UIPoint GetStartPoint() override;
		void SetStartPoint(UIPoint start) override;
		UIPoint GetEndPoint() override;
		void SetEndPoint(UIPoint end) override;
	protected:
		void ReleaseThis() override;
		_m_byte m_alpha = 255;
		UIPoint m_start, m_end;
		Gdiplus::LinearGradientBrush* m_brush = nullptr;
		std::vector<std::pair<_m_color, float>> m_vertex;
		friend class MRender_GDIP;
	};
	class MFont_GDIP : public MFont
	{
	public:
		void SetFontName(std::wstring_view name) override;
		void SetFontSize(_m_uint size, std::pair<_m_uint, _m_uint> range) override;
		void SetFontStyle(UIFontStyle style, std::pair<_m_uint, _m_uint> range) override;
		void SetFontColor(MBrush* brush, std::pair<_m_uint, _m_uint> range) override;
		void SetText(std::wstring_view text) override;
		UIRect GetMetrics() override;
		const UIString& GetFontName() override;
		_m_uint GetFontSize() override;
		UIFontStyle GetFontStyle() override;
		_m_color GetFontColor() override;
		const UIString& GetText() override;
	protected:
		void ReleaseThis() override;
		void CalcMetrics();
		void UpdateFont();
		Gdiplus::Font* m_fontObj = nullptr;
		UIFontStyle m_style;
		_m_color m_color = Color::M_Black;
		UIString m_text;
		UIString m_font;
		UIRect m_metrics;
		_m_uint m_fontsize = 12;
		float m_baseOffset = 0;
		friend class MRender_GDIP;
	};
	class MGeometry_GDIP : public MGeometry
	{
	public:
		MGeometryTypes GetGeometryType() override;
	protected:
		void ReleaseThis() override;
		MGeometryTypes m_type;
		Gdiplus::GraphicsPath* m_geometry = nullptr;
		friend class MRender_GDIP;
	};
	class MRgn_GDIP : public RAII::MBasicObj
	{
	public:
		MRgn_GDIP(int width, int height);
		void Set(HRGN hRgn);
		void ReleaseProc() override;
		UISize GetSize() { return m_size; }
		_m_lpbyte GetBits() { return (_m_lpbyte)m_bits; }
		_m_byte GetBit(int x, int y);
		int GetExAlign() { return m_align; }
	protected:
		HBITMAP m_bitmap = nullptr;
		HDC m_hdc = nullptr;
		void* m_bits = nullptr;
		UISize m_size;
		int m_align = 0;
		friend class MRender_GDIP;
	};
}