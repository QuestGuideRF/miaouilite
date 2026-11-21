
#pragma once
#include <Control/Mui_Label.h>
#include <Control/Mui_ListBox.h>
namespace Mui::Ctrl
{
	class UIComBox : public UILabel
	{
		class UIListBoxCom : public UIListBox
		{
		public:
			UIListBoxCom(Attribute attrib, UIScroll::Attribute scrollAttrib = {})
			: UIListBox(std::move(attrib), std::move(scrollAttrib))
			{
				HideNode(true, false);
			}
		protected:
			bool OnWindowMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam) override;
			bool SendEvent(UINotifyEvent event, _m_param param) override;
			friend class UIComBox;
			UIComBox* combox = nullptr;
		};
	public:
		MCTRL_DEFINE_EX
		(
			L"UIComBox",
			MCTRL_MAKE_ATTRIB
			(
				{ CtrlMgr::AttribType::UIStyle, L"style" },
				{ CtrlMgr::AttribType::UIStyle, L"listStyle" },
				{ CtrlMgr::AttribType::UIBitmap, L"dropIcon" }
			),
			MCTRL_BASE_ATTRIB(UILabel),
			MCTRL_BASE_ATTRIB(UIListBox)
		);
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<int> itemClicked;
			CtrlMgr::EventSlot<int> itemDBClicked;
			CtrlMgr::EventSlot<int, bool&> itemChanging;
			CtrlMgr::EventSlot<int> itemChanged;
		);
		struct Attribute
		{
			UIStylePtr style = nullptr;
			UIBitmapPtr dropIcon = nullptr;
			int dropIconXPos = 0;
			bool dropIconAutoPos = true;
			bool popTop = false;
			int menuHeight = 300;
			float listShadowBlur = 6.f;
			_m_color listShadowColor = 0x32000000;
			UIPoint listShadowOffset = { 0, 6 };
			int listShadowExtend = 0;
		};
		UIComBox(UIControl* parent, Attribute attrib, UIListBox::Attribute listAttrib = {},
			UIScroll::Attribute scrollAttrib = {}, UILabel::Attribute fontStyle = {});
		~UIComBox() override;
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			if (!m_attrib.SetAttribute<T>(attribName, std::forward<T>(value), this))
			{
				if (!UILabel::SetAttributeSrc<T>(attribName, std::forward<T>(value), draw))
					return m_popList->SetAttributeSrc<T>(attribName, std::forward<T>(value), draw);
				return true;
			}
			m_cacheUpdate = true;
			if (draw) UpdateDisplay();
			return true;
		}
		[[nodiscard]] const Attribute& GetAttribute() const;
		int AddItem(ListItem* item, int index, bool draw = true);
		ListItem* GetItem(int index) const;
		bool SetCurSelItem(int index, bool draw = true);
		int GetCurSelItem() const;
		int GetItemListCount() const;
		int DeleteItem(int index, bool draw = true);
		int DeleteItem(int index, bool delItem, bool draw = true);
		void DeleteAllItem(bool delItem = true, bool draw = true);
	protected:
		explicit UIComBox(Attribute attrib, UIListBox::Attribute listAttrib = {},
			UIScroll::Attribute scrollAttrib = {}, UILabel::Attribute fontStyle = {});
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		bool OnWindowMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam) override;
		void OnScale(_m_scale scale) override;
		void OnLayoutCalced() override;
	private:
		void ShowMenu(bool show);
		void CalcMenuFrame();
		void SetText(std::wstring_view text);
		bool IsListAttrib(std::wstring_view name);
		CtrlMgr::UIAttribute<Attribute, UIComBox*> m_attrib;
		static void BindAttribute();
		bool m_isHover = false;
		UIControl* m_root = nullptr;
		UIListBoxCom* m_popList = nullptr;
		UIControlStatus m_state = UIControlStatus_Normal;
		friend class UIListBoxCom;
	};
}