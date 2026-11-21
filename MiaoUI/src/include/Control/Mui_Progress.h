
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UIProgressBar : public UIControl
	{
	public:
		MCTRL_DEFINE(L"UIProgBar", { CtrlMgr::AttribType::UIStyle, L"style" });
		struct Attribute
		{
			UIStylePtr style = nullptr;
			int maxValue = 100;
			int value = 0;
			bool vertical = false;
			bool leftShow = true;
			bool bottomShow = true;
		};
		UIProgressBar(UIControl* parent, Attribute attrib);
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
		void SetMaxValue(int value, bool draw = true);
		int GetMaxValue() const;
		void SetCurValue(int value, bool draw = true);
		int GetCurValue() const;
	protected:
		explicit UIProgressBar(Attribute attrib);
		void OnPaintProc(MPCPaintParam param) override;
		int Percentage(int value1, int percentValue, int percent);
		void CalcDrawRect(MPCRect dest);
	private:
		CtrlMgr::UIAttribute<Attribute> m_attrib;
		static void BindAttribute();
		UIRect m_drawRect;
	};
}