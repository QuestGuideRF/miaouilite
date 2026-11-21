
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UIScroll : public UIControl
	{
	public:
		MCTRL_DEFINE
		(
			L"UIScroll",
			{ CtrlMgr::AttribType::UIStyle, L"styleV" },
			{ CtrlMgr::AttribType::UIStyle, L"styleH" }
		);
		using ScrollCallBack = std::function<void(UIScroll*, int, bool)>;
		struct Attribute
		{
			UIStylePtr styleV = nullptr;
			UIStylePtr styleH = nullptr;
			ScrollCallBack callback = nullptr;
			bool vertical = false;
			bool horizontal = false;
			bool active = true;
			bool animate = true;
			bool button = true;
			UISize range;
			UISize dragValue;
			int barWidth = 8;
			int barMinHeight = 20;
			int btnHeight = 8;
			_m_rect_t<int> inset = { 0 };
		};
		UIScroll(UIControl* parent, Attribute attrib);
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
		[[nodiscard]] int GetRange(bool horizontal) const;
		[[nodiscard]] UISize GetRange() const;
		void SetRange(bool horizontal, int range, bool draw = true);
		void SetRange(UISize range, bool draw = true);
		[[nodiscard]] int GetDragValue(bool horizontal) const;
		[[nodiscard]] UISize GetDragValue() const;
		void SetDragValue(bool horizontal, int value, bool draw = true);
		void SetDragValueNoAni(bool horizontal, int value, bool draw);
		int CalcOffsetDragValue(bool horizontal, int dragValue, int offset);
	protected:
		enum ScrollButtonType
		{
			ButtonNull,
			TopButton,
			BottomButton,
			ThumbButton,
			Track,
			Intersect
		};
		explicit UIScroll(Attribute attrib);
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMove(_m_uint flag, const UIPoint& point) override;
		bool OnMouseExited(_m_uint flag, const UIPoint& point) override;
		bool OnLButtonDown(_m_uint flag, const UIPoint& point) override;
		bool OnLButtonUp(_m_uint flag, const UIPoint& point) override;
		bool OnMouseWheel(_m_uint flag, short delta, const UIPoint& point) override;
		ScrollButtonType CalcPointIn(const UIPoint& pt);
		void CalcControlRect();
		void CalcThumbBtnPos(bool horizontal);
		void CalcDragValue(bool horizontal);
		void PaintControl(MRenderCmd* render, UIStyle* style, UIControlStatus status,
		                  ScrollButtonType type, UIRect& drawRect, _m_byte& alpha, _m_scale scale);
		bool m_ALLWheel = false;
		void SetCallback(const ScrollCallBack& callback)
		{
			m_attrib.Set().callback = callback;
		}
	private:
		int GetScrollPage(bool up, bool horz, float count);
		CtrlMgr::UIAttribute<Attribute, UIScroll*> m_attrib;
		static void BindAttribute();
		UISize m_size;
		struct ScrollData
		{
			UIRect scrollBtn[2];
			UIRect thumbButton;
			UIRect track;
		};
		UIRect m_intersect;
		ScrollData m_scroll[2];
		UIPoint m_clickPos;
		UIControlStatus m_status = UIControlStatus_Normal;
		ScrollButtonType m_btnType = ButtonNull;
		bool m_btnTypeH = false;
		bool m_isClick = false;
	};
}