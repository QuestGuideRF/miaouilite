
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UILabel : public UIControl
	{
	public:
		MCTRL_DEFINE
		(
			L"UILabel",
			{ CtrlMgr::AttribType::labelStyle, L"fontStyle" }
		);
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<UIPoint> clicked;
		);
		struct Attribute
		{
			UIString text;
			UIString font = M_DEF_SYSTEM_FONTNAME;
			UIString url;
			_m_color fontColor = Color::M_Black;
			_m_color urlColor = 0xffa7e236;
			_m_color shadowColor = Color::M_Black;
			_m_ushort fontSize = 12;
			_m_ptrv fontCustom = 0;
			bool hyperlink = false;
			bool shadowUse = false;
			bool shadowLow = false;
			float shadowBlur = 1.f;
			UIPoint shadowOffset = { 1,1 };
			UIFontStyle fontStyle;
			TextAlign textAlign = TextAlign_Top;
		};
		UILabel(UIControl* parent, Attribute attrib);
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			return m_attrib.SetAttribute<T>(attribName, std::forward<T>(value), { this, draw });
		}
		[[nodiscard]] const Attribute& GetAttribute() const { return m_attrib.Get(); }
		[[nodiscard]] UISize GetTextMetric(bool dpi = true) const;
	protected:
		explicit UILabel(Attribute attrib);
		UISize CalcSize();
		void OnLoadResource(MRenderCmd* render, bool recreate) override;
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		bool OnSetCursor(_m_param hCur, _m_param lParam) override;
		void OnScale(_m_scale scale) override;
		_m_sizef GetContentSize() override;
		bool OffsetDraw = false;
		_m_rect OffsetDrawRc = { 0,0,0,0 };
		MFontPtr m_font = nullptr;
		MBrushPtr m_brush = nullptr;
		MEffectPtr m_effect = nullptr;
		bool m_mouseIn = false;
		bool m_isClick = false;
		CtrlMgr::UIAttribute<Attribute, std::pair<UILabel*, bool>> m_attrib;
		static void BindAttribute();
		void updateStyle(UIFontStyle& style);
		bool updateRender(bool draw, bool layout = false);
	};
}