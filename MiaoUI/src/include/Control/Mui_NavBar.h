
#pragma once
#include <Control/Mui_Label.h>
namespace Mui::Ctrl
{
	class UINavBar : public UIControl
	{
	public:
		MCTRL_DEFINE
		(
			L"UINavBar",
			{ CtrlMgr::AttribType::labelStyle, L"fontStyle" }
		);
		struct Attribute
		{
			UIString font = M_DEF_SYSTEM_FONTNAME;
			_m_color fontColor = 0xff787878;
			_m_color fontHoverColor = Color::M_Black;
			_m_color fontPressColor = 0xff969696;
			_m_ushort fontSize = 12;
			UIFontStyle fontStyle;
			_m_ptrv fontCustom = 0;
			bool shadowUse = false;
			bool shadowLow = false;
			float shadowBlur = 1.f;
			UIPoint shadowOffset = { 1,1 };
			_m_color shadowColor = Color::M_Black;
			int itemSpace = 10;
			int barSpace = 5;
			int barHeight = 4;
			int barAnitime = 300;
			float barRound = 1.5f;
			_m_color barColor = 0xffffb375;
		};
		UINavBar(UIControl* parent, Attribute attrib);
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			if (m_attrib.SetAttribute(attribName, std::forward<T>(value), this))
			{
				m_cacheUpdate = true;
				if (draw) UpdateDisplay();
				return true;
			}
			return false;
		}
		[[nodiscard]] const Attribute& GetAttribute() const { return m_attrib.Get(); }
		void SetFontStyle(UILabel::Attribute& attrib, bool draw = true);
		int AddItem(std::wstring_view title, int index = -1, bool draw = true);
		void SetItemTitle(int index, std::wstring_view title, bool draw = true);
		[[nodiscard]] std::wstring GetItemTitle(int index) const;
		bool SetCurSelItem(int index, bool draw = true);
		[[nodiscard]] int GetCurSelItem() const;
		[[nodiscard]] int GetItemListCount() const;
		int DeleteItem(int index, bool draw = true);
		void DeleteAllItem(bool draw = true);
	protected:
		explicit UINavBar(Attribute attrib);
		void OnLoadResource(MRenderCmd* render, bool recreate) override;
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseEntered(_m_uint flag, const UIPoint& point) override;
		bool OnMouseExited(_m_uint flag, const UIPoint& point) override;
		bool OnLButtonDown(_m_uint flag, const UIPoint& point) override;
		bool OnLButtonUp(_m_uint flag, const UIPoint& point) override;
		bool OnMouseMove(_m_uint flag, const UIPoint& point) override;
		void OnScale(_m_scale scale) override;
		_m_sizef GetContentSize() override;
		void OnLayoutCalced() override;
		bool IndexCheck(int index) const;
	private:
		std::pair<std::wstring, UIRect> InitItemRect(std::wstring_view title, int index);
		void CalcItemRect();
		bool IsHitItem(const UIPoint& pt);
		CtrlMgr::UIAttribute<Attribute, std::pair<UINavBar*, bool>> m_attrib;
		static void BindAttribute();
		void updateStyle(UIFontStyle& style);
		bool updateRender(bool draw, bool layout = false);
		MFontPtr m_font = nullptr;
		MBrushPtr m_brush = nullptr;
		MEffectPtr m_effect = nullptr;
		UISize m_size;
		std::vector<std::pair<std::wstring, UIRect>> m_itemList;
		int m_curSelIndex = 0;
		int m_curHoverIndex = -1;
		UIControlStatus m_state = UIControlStatus_Normal;
		bool m_down = false;
		int m_lastHover = -1;
	protected:
		int m_baroffset = 0;
		int m_barwidth = 0;
	};
}