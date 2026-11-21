
#pragma once
#include <Control/Mui_Label.h>
#include <Control/Mui_Scroll.h>
#ifdef _WIN32
#include <Richedit.h>
#include <TextServ.h>
#include <RichOle.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#elif __ANDROID__
#include <Control/Mui_Panel.h>
#endif
namespace Mui::Ctrl
{
#ifndef UIEDITATTRIB
#define UIEDITATTRIB struct Attribute { \
	UIStylePtr style;				 \
	UILabel::Attribute fontStyle;	 \
	bool multiline = false;          \
	bool password = false;           \
	wchar_t passChar = L'*';		 \
	bool readOnly = false;           \
	bool isRich = false;             \
	bool wordWrap = false;           \
	bool wordAutoSel = false;        \
	bool number = false;             \
	bool scroll = false;             \
	bool autoBar = true;             \
									 \
	int limitText = 0x7fffffff;		 \
	_m_word editAlign = 0;			 \
	_m_color caretColor = 0xff000000;\
									 \
	UIString pholderText;			 \
	_m_color pholderTextColor = 0xFF606060;\
	}
#endif
#ifdef _WIN32
	class UIEditBox : public UIScroll
	{
	public:
		MCTRL_DEFINE_EX
		(
			L"UIEditBox",
			MCTRL_MAKE_ATTRIB
			(
				{ CtrlMgr::AttribType::UIStyle, L"style" },
				{ CtrlMgr::AttribType::labelStyle, L"fontStyle" }
			),
			MCTRL_BASE_ATTRIB(UIScroll)
		);
		MCTRL_DEFSLOT
		(
			CtrlMgr::EventSlot<std::wstring_view> textChanged;
		);
		UIEDITATTRIB;
		UIEditBox(UIControl* parent, Attribute attrib, UIScroll::Attribute scrollAttrib = {});
		~UIEditBox() override;
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
		void SetSel(int first, int second, bool noScroll = false);
		void ReplaceSel(std::wstring_view text, bool canUndo = true);
		bool GetWordWrap() const;
		void SetWordWrap(bool wrap);
		bool GetReadOnly() const;
		void SetReadOnly(bool read);
		int GetLimitText() const;
		void SetLimitText(int lenght);
		void AppendText(std::wstring_view text, bool canUndo = true);
		_m_word GetEditAlign() const;
		void SetEditAlign(_m_word align);
		bool IsRichMode() const;
		void SetRichMode(bool rich);
		void SetMultiline(bool multiline);
		void SetNumber(bool number);
		void SetTextColor(_m_color color);
		void SetCurText(std::wstring_view text);
		std::wstring GetCurText() const;
		std::wstring GetCurSelText() const;
		int GetCurTextLength(_m_ulong flag) const;
		void SelAllText();
		void SetFontStyle(const UILabel::Attribute& fontStyle);
		void AddColorText(std::wstring_view text, _m_color color);
		void SetPassword(bool password);
		void SetPasswordChar(wchar_t ch = '*');
		bool Redo();
		bool Undo();
		void Clear();
		void Copy() const;
		void Cut();
		void Paste();
		void SetEnabled(bool enable, bool draw = true) override;
	protected:
		explicit UIEditBox(Attribute attrib, UIScroll::Attribute scrollAttrib = {});
		void OnLoadResource(MRenderCmd* render, bool recreate) override;
		void OnPaintProc(MPCPaintParam param) override;
		bool OnWindowMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam) override;
		bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam) override;
		bool OnSetCursor(_m_param hCur, _m_param lParam) override;
		void OnTimer(_m_ptrv idTimer) override;
		void OnScale(_m_scale scale) override;
		void OnTextChanged(std::wstring str);
		void CreateCaret(int cx, int cy);
		void ShowCaret(bool show);
		void SetCaretPos(int x, int y);
		void SetCaretColor(_m_color color, MRenderCmd* render);
		_m_rect GetCaretRect();
		void OnScrollView(UIScroll*, int dragValue, bool horizontal);
		HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT* plresult) const;
		void TxServices(std::function<void(ITextServices2*)> task);
		class MTextHost : public ITextHost2
		{
			friend class UIEditBox;
		public:
			virtual ~MTextHost() = default;
			ITextServices2* GetServices();
			void SetClientRect(UIRect* prc);
			void SetWordWrap(bool warp);
			void SetReadOnly(bool read);
			void SetFont(HFONT hFont);
			void SetColor(COLORREF color);
			void LimitText();
			WORD GetAlign();
			void SetAlign(WORD align);
			void SetRichTextFlag(bool rich);
			void SetPasswordChar(WCHAR chars);
			void SetMultiline(bool multiline);
			void SetBarWidth(_m_ushort width);
			bool IsITextHost2() { return !IsOldVer; }
			HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
			ULONG __stdcall AddRef() override;
			ULONG __stdcall Release() override;
			HDC TxGetDC() override { return m_editBox->m_oldPaint.PaintDC; }
			INT TxReleaseDC(HDC hdc) override { return TRUE; }
			BOOL TxShowScrollBar(INT fnBar, BOOL fShow) override;
			BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags) override { return TRUE; }
			BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw) override;
			BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw) override;
			void TxInvalidateRect(LPCRECT prc, BOOL fMode) override;
			void TxViewChange(BOOL fUpdate) override;
			BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) override;
			BOOL TxShowCaret(BOOL fShow) override { return TRUE; }
			BOOL TxSetCaretPos(INT x, INT y) override;
			BOOL TxSetTimer(UINT idTimer, UINT uTimeout) override { return TRUE; }
			void TxKillTimer(UINT idTimer) override { }
			void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll) override {}
			void TxSetCapture(BOOL fCapture) override;
			void TxSetFocus() override;
			void TxSetCursor(HCURSOR hcur, BOOL fText) override;
			BOOL TxScreenToClient(LPPOINT lppt) override;
			BOOL TxClientToScreen(LPPOINT lppt) override;
			HRESULT TxActivate(LONG* plOldState) override { return S_OK; }
			HRESULT TxDeactivate(LONG lNewState) override { return S_OK; }
			HRESULT TxGetClientRect(LPRECT prc) override;
			HRESULT TxGetViewInset(LPRECT prc) override;
			HRESULT TxGetCharFormat(const CHARFORMATW** ppCF) override;
			HRESULT TxGetParaFormat(const PARAFORMAT** ppPF) override;
			COLORREF TxGetSysColor(int nIndex) override;
			HRESULT TxGetBackStyle(TXTBACKSTYLE* pstyle) override;
			HRESULT TxGetMaxLength(DWORD* plength) override;
			HRESULT TxGetScrollBars(DWORD* pdwScrollBar) override;
			HRESULT TxGetPasswordChar(TCHAR* pch) override;
			HRESULT TxGetAcceleratorPos(LONG* pcp) override { return S_OK; }
			HRESULT TxGetExtent(LPSIZEL lpExtent) override;
			HRESULT OnTxCharFormatChange(const CHARFORMATW* pcf) override { return S_OK; }
			HRESULT OnTxParaFormatChange(const PARAFORMAT* ppf) override { return S_OK; }
			HRESULT TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits) override;
			HRESULT TxNotify(DWORD iNotify, void* pv) override { return S_OK; }
			HIMC TxImmGetContext() override { return nullptr; }
			void TxImmReleaseContext(HIMC himc) override { }
			HRESULT TxGetSelectionBarWidth(LONG* lSelBarWidth) override;
			HRESULT TxDestroyCaret() override { return S_OK; }
			void TxFreeTextServicesNotification() override {  }
			HRESULT TxGetEastAsianFlags(LONG* pFlags) override { return S_OK; }
			HRESULT TxGetEditStyle(DWORD dwItem, DWORD* pdwData) override { return S_OK; }
			HRESULT TxGetHorzExtent(LONG* plHorzExtent) override { return S_OK; }
			HPALETTE TxGetPalette() override { return nullptr; }
			HRESULT TxGetWindow(HWND* phwnd) override { return S_OK; }
			HRESULT TxGetWindowStyles(DWORD* pdwStyle, DWORD* pdwExStyle) override { return S_OK; }
			BOOL TxIsDoubleClickPending() override { return FALSE; }
			HCURSOR TxSetCursor2(HCURSOR hcur, BOOL bText) override { return nullptr; }
			HRESULT TxSetForegroundWindow() override { return S_OK; }
			HRESULT TxShowDropCaret(BOOL fShow, HDC hdc, LPCRECT prc) override { return S_OK; }
			BOOL Init(UIEditBox* m_pEdit);
			HRESULT InitDefaultCharFormat(CHARFORMAT2W* pcf, HFONT hFont);
			HMODULE m_hDll = nullptr;
			LONG m_dwRef = 0;
			ITextServices2* m_services = nullptr;
			SIZEL m_sizelExtent = { 0,0 };
			CHARFORMAT2W m_charFormat = {};
			PARAFORMAT2	m_paraFormat = {};
			UIEditBox* m_editBox = nullptr;
			bool IsOldVer = false;
		};
		MTextHost* m_txtHost = nullptr;
		void TextHost(std::function<void(MTextHost*)> task) const;
		friend class MTextHost;
		UIRect GetInsetFrame(UIRect src);
		struct OldPaintData
		{
			UISize bufferSize;
			HBITMAP PaintBMP;
			HDC PaintDC;
		} m_oldPaint;
		void GDIPaint(MRenderCmd* render, MPCRect destRect, MPCRect insetRect, bool cacheCanvas);
		void OnInitTextSer();
		std::wstring GetCurTextInternal() const;
		HFONT m_font = nullptr;
		HFONT GetFont();
		HBRUSH m_ClearBrush = nullptr;
		mutable CtrlMgr::UIAttribute<Attribute, UIEditBox*> m_attrib;
		static void BindAttribute();
		UISize CaretSize;
		UIPoint CaretPos;
		_m_rect CaretRect;
		_m_color CaretColor = 0xff000000;
		MBrushPtr CaretBrush = nullptr;
		bool CaretVisiable = false;
		MTimers::ID CaretTimer = 0;
		UIControlStatus m_controlState = UIControlStatus_Normal;
		bool m_mouseInScroll = false;
		bool m_mouseDown = false;
		bool m_autoWordSel = false;
	};
#endif
#ifdef __ANDROID__
        class UIEditBox : public UILabel
        {
        public:
            UIEditBox();
            UIEditBox(UIControl* parent, UIEditBoxConfig config);
            virtual ~UIEditBox();
            M_DEF_CTRL(L"UIEditBox");
             void SetProperty(UIEditBoxConfig config, bool draw = true);
            UIEditBoxConfig GetProperty();
            virtual void SetAttribute(std::wstring attribName, std::wstring attrib, bool draw = true) override;
            virtual std::wstring GetAttribute(std::wstring attribName) override;
            void SetSel(_m_ulong dwSelection, bool bNoScroll = false) { }
            void ReplaceSel(std::wstring text, bool bCanUndo = true) { }
            bool GetWordWrap() { return m_config.WordWrap; }
            void SetWordWrap(bool fWordWrap) { m_config.WordWrap = fWordWrap; }
            bool GetReadOnly() { return m_config.ReadOnly; }
            bool SetReadOnly(bool bReadOnly) { return m_config.ReadOnly = bReadOnly; }
            _m_long GetLimitText() { return m_config.LimitText; }
            bool SetLimitText(_m_long nLength) { m_config.LimitText = nLength; }
            void AppendText(std::wstring text, bool bCanUndo = true);
            _m_word GetDefaultAlign() { return m_align; }
            void SetDefaultAlign(_m_word wNewAlign);
            bool GetRichTextFlag() { return false; }
            void SetRichTextFlag(bool fRich) { m_config.IsRich = fRich; }
            _m_color SetDefaultTextColor(_m_color cr);
            void SetCurText(std::wstring text);
            std::wstring GetCurText() { return m_config.m_textConfig.Text; }
            std::wstring GetCurSelText() { return L""; }
            _m_long GetCurTextLength(_m_ulong flag) { return m_config.m_textConfig.Text.length(); }
            void SelAllText() {}
            void SetMultiline(bool multiline);
            void SetNumber(bool number);
            void SetFontStyle(UILabel::Property fontStyle);
            void AddColorText(std::wstring text, _m_color color);
            int GetScrollShift(bool hBar = false) { return 0; };
            int GetScrollDragPos(bool hBar = false) { return 0; };
            void SetScrollDragPos(int pos, bool hBar = false) {  };
            void SetPassword(bool password);
            bool Redo() { return true; }
            bool Undo() { return true; }
            void Clear() { SetCurText(L""); }
            void Copy() {}
            void Cut() {}
            void Paste() {}
            virtual void OnPaintProc(MRenderCmd* render, MPCRect clipRect, MPCRect destRect, bool cacheCanvas) override;
        protected:
            virtual bool OnMouseEntered(_m_uint flag, const UIPoint& point) override;
            virtual bool OnMouseExited(_m_uint flag, const UIPoint& point) override;
            virtual bool OnLButtonDown(_m_uint flag, const UIPoint& point) override;
            virtual bool OnLButtonUp(_m_uint flag, const UIPoint& point) override;
            UIEditBoxConfig m_config;
            std::wstring  m_passStr;
            _m_word m_align = 0;
            UIControlStatus m_status = UIControlStatus_Normal;
        };
#endif
}