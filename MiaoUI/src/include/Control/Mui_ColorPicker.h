
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UIColorPicker : public UIControl
	{
	public:
		MCTRL_DEFNAME(L"UIColorPicker");
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<_m_color> changed;
		);
		UIColorPicker(UIControl* parent);
		struct HSV
		{
			_m_ushort hue = 0;
			_m_ushort sat = 0;
			_m_ushort val = 0;
		};
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		void SetHSVColor(HSV hsv, bool draw = true);
		HSV GetHSVColor();
		void SetRGBAColor(_m_color color, bool draw = true);
		_m_color GetRGBAColor();
	protected:
		UIColorPicker() { m_cacheSupport = true; }
		void OnLoadResource(MRenderCmd* render, bool recreate) override;
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		bool OnMouseMove(_m_uint flag, const UIPoint& point) override;
		bool OnLButtonUp(_m_uint flag, const UIPoint& point) override;
		bool OnSetCursor(_m_param hCur, _m_param lParam) override;
	private:
		float m_hue = 359.f;
		float m_sat = 0.f;
		float m_val = 0.f;
		float m_hue_old = m_hue;
		_m_color m_rgb = 0;
		_m_color m_rgb_old = 0;
		_m_byte m_alpha = 255;
		void UpdateRGB();
		void UpdateHSV();
		MGradientBrushPtr m_colorBrush = nullptr;
		MGradientBrushPtr m_bottomBrush = nullptr;
		bool m_down = false;
	};
}