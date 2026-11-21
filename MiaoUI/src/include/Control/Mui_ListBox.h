
#pragma once
#include <Control/Mui_Scroll.h>
namespace Mui::Ctrl
{
	class UIListBox;
	class ListItem
	{
	public:
		ListItem() = default;
		ListItem(std::wstring_view title, _m_color color = 0, UIFontStyle style = {});
		virtual ~ListItem();
		virtual void SetText(std::wstring_view text, _m_color color = 0, UIFontStyle style = {}, bool updateStyle = false);
		virtual void SetFontSize(_m_uint size);
		virtual std::wstring_view GetText();
	protected:
		enum ItemStatus
		{
			UIListItemNormal,
			UIListItemHover,
			UIListItemPressed,
			UIListItemDisbale,
			UIListItemNormalSel,
			UIListItemHoverSel,
			UIListItemPressedSel,
			UIListItemDisbaleSel
		};
		struct PaintParam
		{
			MRenderCmd* render = nullptr;
			MPCRect dstRect = nullptr;
			UIStyle* style = nullptr;
			_m_byte dstAlpha = 0;
			_m_rcscale dstScale;
			UIPoint offset;
			_m_color defcolor = 0;
		};
		virtual void OnPaintProc(const PaintParam* param);
		virtual void PaintStyle(MRenderCmd* render, UIStyle* style, _m_rect frame,
			ItemStatus state, _m_byte alpha, _m_scale scale = { 1.f, 1.f });
		virtual bool OnMouseMessage(_m_uint message, _m_param wParam, _m_param lParam);
		bool IsSel();
		UIListBox* m_parent = nullptr;
		ItemStatus m_state = UIListItemNormal;
		bool m_isClick = false;
		UIString m_text;
		UIFontStyle m_style;
		_m_uint m_size = 12;
		_m_color m_color = 0;
		friend class UIListBox;
	};
	class UIListBox : public UIScroll
	{
	public:
		MCTRL_DEFINE_EX
		(
			L"UIListBox",
			MCTRL_MAKE_ATTRIB
			(
				{ CtrlMgr::AttribType::UIStyle, L"style" },
				{ CtrlMgr::AttribType::UIStyle, L"itemStyle" },
				{ CtrlMgr::AttribType::listfontStyle, L"iFontStyle" }
			),
			MCTRL_BASE_ATTRIB(UIScroll)
		);
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<UIPoint, int> itemClicked;
			CtrlMgr::EventSlot<UIPoint, int> itemDBClicked;
			CtrlMgr::EventSlot<int, bool&> itemChanging;
			CtrlMgr::EventSlot<int> itemChanged;
		);
		struct ItemFont
		{
			UIString font = M_DEF_SYSTEM_FONTNAME;
			_m_color fontColor = 0xff000000;
			_m_ushort fontSize = 12;
			_m_ptrv fontCustom = 0;
			UIFontStyle fontStyle;
			TextAlign textAlign = TextAlign_Top;
		};
		struct Attribute
		{
			UIStylePtr style = nullptr;
			UIStylePtr itemStyle = nullptr;
			int itemHeight = 30;
			int lineSpace = 0;
			bool allowRightSel = false;
			UIPoint drawOffset;
			ItemFont fontStyle;
		};
		UIListBox(UIControl* parent, Attribute attrib, UIScroll::Attribute scrollAttrib = {});
		~UIListBox() override;
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
		template<typename T>
		bool SetAttributeSrc(std::wstring_view attribName, T&& value, bool draw = true)
		{
			if (!m_attrib.SetAttribute<T>(attribName, std::forward<T>(value), this))
				return UIScroll::SetAttributeSrc<T>(attribName, std::forward<T>(value), draw);
			m_cacheUpdate = true;
			if (draw) UpdateDisplay();
			return true;
		}
		[[nodiscard]] const Attribute& GetAttribute() const;
		int AddItem(ListItem* item, int index, bool draw = true);
		[[nodiscard]] ListItem* GetItem(int index) const;
		bool SetCurSelItem(int index, bool draw = true);
		[[nodiscard]] int GetCurSelItem() const;
		[[nodiscard]] int GetItemListCount() const;
		int DeleteItem(int index, bool draw = true);
		int DeleteItem(int index, bool delItem, bool draw = true);
		void DeleteAllItem(bool delItem = true, bool draw = true);
	protected:
		explicit UIListBox(Attribute attrib, UIScroll::Attribute scrollAttrib = {});
		void OnLoadResource(MRenderCmd* render, bool recreate) override;
		void OnPaintProc(MPCPaintParam param) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		void OnScrollView(UIScroll*, int dragValue, bool horizontal);
		bool DispatchItemMsg(UIPoint& point, _m_uint msg, _m_param wParam, _m_param lParam);
		void OnLayoutCalced() override { CalcListView(); }
		void SetViewTop(int top);
		bool IndexCheck(int index) const;
		void CalcListView();
		void PaintListView(MRenderCmd* render, MPCRect destRect, bool cache);
		void SetSelItem(ListItem* item, UIPoint pt);
		void UpdateIndex();
		MFontPtr m_fontDef = nullptr;
		MBrushPtr m_brushDef = nullptr;
	private:
		CtrlMgr::UIAttribute<Attribute, UIListBox*> m_attrib;
		static void BindAttribute();
		void updateStyle(UIFontStyle& style);
	protected:
		UIControlStatus m_state = UIControlStatus_Normal;
		std::vector<ListItem*> m_itemList;
		struct ViewData
		{
			UIPoint offsetPos;
			int beginIndex = 0;
			int indexCount = 0;
			int top = 0;
			int bottom = 0;
			int viewHeight = 0;
			int itemHeight = 0;
			int lineSpace = 0;
			UIRect boxRect;
			_m_rcscale scale;
			std::vector<UIRect> viewItemRect;
		} m_view;
		int m_curSelIndex = -1;
		ListItem* m_hoverIndex = nullptr;
		ListItem* m_curSelItem = nullptr;
		UIRect m_curItemRect;
		int m_downViewY = 0;
		friend class ListItem;
	};
}