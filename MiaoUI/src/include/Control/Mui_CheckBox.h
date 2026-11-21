
#pragma once
#include <Control/Mui_Label.h>
namespace Mui::Ctrl
{
	class UICheckBox : public UILabel
	{
	public:
		MCTRL_DEFINE_EX
		(
			L"UICheckBox",
			MCTRL_MAKE_ATTRIB({ CtrlMgr::AttribType::UIStyle, L"style" }),
			MCTRL_BASE_ATTRIB(UILabel)
		);
		struct Attribute
		{
			UIStylePtr style = nullptr;
			bool isSel = false;
			bool allowClick = true;
			bool animate = false;
			bool aniAlphaType = false;
			int textOffset = 10;
		};
		UICheckBox(UIControl* parent, Attribute attrib, UILabel::Attribute fontStyle = {});
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
		virtual void SetSel(bool sel, bool draw = true);
		virtual bool GetSel() const;
	protected:
		explicit UICheckBox(Attribute attrib, UILabel::Attribute fontStyle = {});
		enum Status
		{
			UICheckBoxNormal,
			UICheckBoxHover,
			UICheckBoxPressed,
			UICheckBoxDisbale,
			UICheckBoxNormalSel,
			UICheckBoxHoverSel,
			UICheckBoxPressedSel,
			UICheckBoxDisbaleSel
		};
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		_m_sizef GetContentSize() override;
		void ChangeStatus(Status&& state, Status&& oldState);
	private:
		CtrlMgr::UIAttribute<Attribute> m_attrib;
		Status m_state = UICheckBoxNormal;
	};
}