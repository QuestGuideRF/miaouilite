
#pragma once
#ifdef _WIN32
#include <Window/Mui_BasicWnd.h>
namespace Mui::Window
{
	class UIWindowsWnd : public UIWindowBasic
	{
	public:
		UIWindowsWnd(Render::Def::MRender* render);
		UIWindowsWnd(UIWindowsWnd&&) = delete;
		UIWindowsWnd(const UIWindowsWnd&) = delete;
		UIWindowsWnd& operator=(const UIWindowsWnd&) = delete;
		UIWindowsWnd& operator=(UIWindowsWnd&&) = delete;
		~UIWindowsWnd() override;
		bool Create(_m_param parent, std::wstring_view title, UIRect rect, std::function<bool()>&& afterCreated, _m_param style, _m_param exStyle);
		bool Create(_m_param parent, std::wstring_view title, UIRect rect, std::function<bool()>&& afterCreated, int wndType, _m_param exStyle = 0);
		void SetWindowTitle(std::wstring_view title) override;
		std::wstring GetWindowTitle() override;
		void ShowWindow(bool show, bool focus = true) override;
		bool IsShowWindow() override;
		void EnableWindow(bool enable) override;
		bool IsEnableWindow() override;
		bool IsMinimize() override;
		bool IsMaximize() override;
		void CenterWindow() override;
		void CloseWindow() override;
		void SetWindowRect(UIRect rect) override;
		UIRect GetWindowRect(bool client = false) override;
		void SetWindowAlpha(_m_byte alpha, bool draw = true) override;
		_m_byte GetWindowAlpha() override;
		_m_ptrv GetWindowHandle() override;
		void SetMinimSize(UISize min) override;
		void SetVerticalSync(bool sync);
		MEventCodeEnum ConvertEventCode(_m_uint src);
		_m_uint ConvertEventCode(MEventCodeEnum src);
	protected:
		UISize GetWindowSrcSize() override;
		void Present(Render::MRenderCmd* render, const _m_rect_t<int>* rcPaint) override;
		bool InitRender(Render::MRenderCmd* render) override;
	private:
		std::wstring_view M_DEF_CLSNAME = L"MiaoUI_Windows";
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL RegisterWindowClass();
		HWND m_hWnd = nullptr;
		_m_byte m_alpha = 255;
		std::wstring m_title;
		UISize m_srcSize;
		UISize m_minSize;
		bool m_layerWnd = false;
		bool m_notitleWnd = false;
		std::atomic_bool m_VSync = false;
		_m_param m_cachePos = 0;
	};
}
#endif