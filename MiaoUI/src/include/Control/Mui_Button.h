
#pragma once
#include <Control/Mui_Label.h>
namespace Mui::Ctrl
{
	class UIButton : public UILabel
	{
	public:
		MCTRL_DEFINE_EX
		(
			L"UIButton",
			MCTRL_MAKE_ATTRIB({ CtrlMgr::AttribType::UIStyle, L"style" }),
			MCTRL_BASE_ATTRIB(UILabel)
		);
		struct Attribute
		{
			UIStylePtr style = nullptr;
			bool animate = false;
			bool aniAlphaType = false;
			_m_rect_t<int> inset = { 2,2,2,2 };
		};
		UIButton(UIControl* parent, Attribute attrib, UILabel::Attribute fontStyle = {});
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			if (!m_attrib.SetAttribute<T>(attribName, std::forward<T>(value), draw))
				return UILabel::SetAttributeSrc<T>(attribName, std::forward<T>(value), draw);
			m_cacheUpdate = true;
			if (draw) UpdateDisplay();
			return true;
		}
		[[nodiscard]] const Attribute& GetAttribute() const;
	protected:
		explicit UIButton(Attribute attrib, UILabel::Attribute fontStyle = {});
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		void ChangeStatus(UIControlStatus&& state, UIControlStatus&& oldState);
		_m_sizef GetContentSize() override;
	private:
		CtrlMgr::UIAttribute<Attribute> m_attrib;
		UIControlStatus m_state = UIControlStatus_Normal;
	};
}