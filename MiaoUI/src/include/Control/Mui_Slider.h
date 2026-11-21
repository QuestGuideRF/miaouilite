
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UISlider : public UIControl
	{
	public:
		MCTRL_DEFINE
		(
			L"UISlider",
			{ CtrlMgr::AttribType::UIStyle, L"style" },
			{ CtrlMgr::AttribType::UIStyle, L"btnStyle" }
		);
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<int> valueChanged;
		);
		struct Attribute
		{
			UIStylePtr style = nullptr;
			UIStylePtr btnStyle = nullptr;
			int maxValue = 100;
			int minValue = 0;
			int value = 0;
			bool vertical = false;
			bool leftShow = true;
			bool bottomShow = true;
			_m_rect_t<int> trackInset = { 0 };
			UISize btnSize;
		};
		UISlider(UIControl* parent, Attribute attrib);
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			if (m_attrib.SetAttribute(attribName, std::forward<T>(value), draw))
			{
				m_cacheUpdate = true;
				if (draw) UpdateDisplay();
				return true;
			}
			return false;
		}
		[[nodiscard]] const Attribute& GetAttribute() const { return m_attrib.Get(); }
		void SetCurValue(int value, bool draw = true);
		int GetCurValue() const;
		void SetMaxValue(int max, bool draw = true);
		int GetMaxValue() const;
		void SetMinValue(int min, bool draw = true);
		int GetMinValue() const;
	protected:
		enum UISliderStatus
		{
			UISliderTrackNormal,
			UISliderTrackDisable = 2,
			UISliderBlockNormal = 1,
			UISliderBlockDisable = 3,
			UISliderBtnNormal = 0,
			UISliderBtnHover,
			UISliderBtnPressed,
			UISliderBtnDisable
		};
		explicit UISlider(Attribute attrib);
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		int Percentage(int value1, int percentValue, int percent);
		void CalcDrawRect();
		int CalcPtValue(UIPoint pt);
	private:
		CtrlMgr::UIAttribute<Attribute> m_attrib;
		static void BindAttribute();
		UISliderStatus btnStatus = UISliderBtnNormal;
		UIRect m_drawRect;
		UIRect m_drawBtnRect;
		UIRect m_drawBgRect;
		bool m_isClick = false;
		int m_oldValue = 0;
		UIPoint m_downpos;
		bool m_hitbtn = false;
	};
}