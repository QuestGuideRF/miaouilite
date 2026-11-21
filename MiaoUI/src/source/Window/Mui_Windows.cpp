
#ifdef _WIN32
#include <Window/Mui_Windows.h>
#include <Render/Graphs/Mui_GdipRender.h>
#include <Control/Mui_Control.h>
#include <Mui_Settings.h>
#include <windowsx.h>
#include <Uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "Msimg32.lib")
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
namespace Mui::Window
{
	using namespace std::chrono;
	using namespace Render;
	bool regwindow = false;
	UIWindowsWnd::UIWindowsWnd(MRender* render)
		: UIWindowBasic(render)
	{
		SetInited(false);
	}
	UIWindowsWnd::~UIWindowsWnd()
	{
		if (!m_hWnd)
			return;
		SetWindowLongPtrW(m_hWnd, GWLP_USERDATA, 0);
		if (UIWindowBasic::IsMainWindow())
		{
			UnregisterClassW(M_DEF_CLSNAME.data(), (HINSTANCE)&__ImageBase);
			regwindow = false;
		}
	}
	bool UIWindowsWnd::Create(_m_param parent, std::wstring_view title, UIRect rect,
	                          std::function<bool()>&& afterCreated, _m_param style, _m_param exStyle)
	{
		if (IsWindow(m_hWnd)) return false;
		if (!regwindow)
		{
			regwindow = RegisterWindowClass();
			if (!regwindow)
				return false;
		}
		if (exStyle & WS_EX_LAYERED)
			m_layerWnd = true;
		int width = rect.GetWidth();
		int height = rect.GetHeight();
		m_hWnd = CreateWindowExW((DWORD)exStyle, M_DEF_CLSNAME.data(), title.data(), (DWORD)style, rect.left, rect.top, width,
		                         height, (HWND)parent, 0, 0, (LPVOID)this);
		if (m_hWnd)
		{
			m_srcSize = { width, height };
			m_title = title;
			bool ret = InitRender(GetRender());
			if (ret)
				ret = afterCreated();
			if (ret)
			{
				SetInited(true);
				UpdateLayout(nullptr);
			}
			return ret;
		}
		return false;
	}
	bool UIWindowsWnd::Create(_m_param parent, std::wstring_view title, UIRect rect, std::function<bool()>&& afterCreated, int wndType, _m_param exStyle)
	{
		DWORD style = 0;
		if (wndType == 0) style = WS_OVERLAPPEDWINDOW;
		else if (wndType == 1)
		{
			style = WS_MINIMIZEBOX;
			m_notitleWnd = true;
		}
		else if (wndType == 2) style = WS_POPUP;
		else if (wndType == 3)
		{
			style = WS_POPUP;
			exStyle |= WS_EX_LAYERED;
		}
		return Create(parent, title, rect, std::move(afterCreated), (_m_param)style, (_m_param)exStyle);
	}
	void UIWindowsWnd::SetWindowTitle(std::wstring_view title)
	{
		m_title = title;
		::SetWindowTextW(m_hWnd, title.data());
	}
	std::wstring UIWindowsWnd::GetWindowTitle()
	{
		return m_title;
	}
	void UIWindowsWnd::ShowWindow(bool show, bool focus)
	{
		::ShowWindow(m_hWnd, show ? (focus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
	}
	bool UIWindowsWnd::IsShowWindow()
	{
		return ::IsWindowVisible(m_hWnd) ? true : false;
	}
	void UIWindowsWnd::EnableWindow(bool enable)
	{
		::EnableWindow(m_hWnd, enable);
	}
	bool UIWindowsWnd::IsEnableWindow()
	{
		return ::IsWindowEnabled(m_hWnd) ? true : false;
	}
	bool UIWindowsWnd::IsMinimize()
	{
		return IsIconic(m_hWnd);
	}
	bool UIWindowsWnd::IsMaximize()
	{
		return IsZoomed(m_hWnd);
	}
	void UIWindowsWnd::CenterWindow()
	{
		MONITORINFO monitor_info;
		monitor_info.cbSize = sizeof(monitor_info);
		GetMonitorInfoW(MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST),
		                &monitor_info);
		RECT& rcMonitor = monitor_info.rcMonitor;
		RECT rcWindow { 0 };
		::GetWindowRect(m_hWnd, &rcWindow);
		int pos[2]
		{
			int((rcMonitor.right - rcMonitor.left) - (rcWindow.right - rcWindow.left)) / 2,
			int((rcMonitor.bottom - rcMonitor.top) - (rcWindow.bottom - rcWindow.top)) / 2
		};
		SetWindowPos(m_hWnd, nullptr, rcMonitor.left + pos[0], rcMonitor.top + pos[1], -1, -1,
		             SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	void UIWindowsWnd::CloseWindow()
	{
		::PostMessageW(m_hWnd, WM_CLOSE, 0, 0);
	}
	void UIWindowsWnd::SetWindowRect(UIRect rect)
	{
		::MoveWindow(m_hWnd, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), TRUE);
	}
	UIRect UIWindowsWnd::GetWindowRect(bool client)
	{
		RECT rect = { 0 };
		if (client)
			::GetClientRect(m_hWnd, &rect);
		else
			::GetWindowRect(m_hWnd, &rect);
		return UIRect(_m_rect(rect.left, rect.top, rect.right, rect.bottom));
	}
	void UIWindowsWnd::SetWindowAlpha(_m_byte alpha, bool draw)
	{
		m_alpha = alpha;
		if(!m_layerWnd)
			SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), alpha, 3UL);
	}
	_m_byte UIWindowsWnd::GetWindowAlpha()
	{
		return m_alpha;
	}
	_m_ptrv UIWindowsWnd::GetWindowHandle()
	{
		return (_m_ptrv)m_hWnd;
	}
	void UIWindowsWnd::SetMinimSize(UISize min)
	{
		m_minSize = min;
	}
	void UIWindowsWnd::SetVerticalSync(bool sync)
	{
		m_VSync = sync;
		auto render = GetRender();
#if MUI_CFG_ENABLE_OPENGL
		if (std::wstring_view(render->GetRenderName()) == L"OpenGL")
		{
			render->RunTask([&]
			{
				render->GetBase<GL::MRender_GL>()->VerticalSync(sync);
			});
		}
#endif
	}
	UISize UIWindowsWnd::GetWindowSrcSize()
	{
		return m_srcSize;
	}
	void UIWindowsWnd::Present(MRenderCmd* render, const _m_rect_t<int>* rcPaint)
	{
		std::wstring_view renderName = render->GetRenderName();
		HDC hdc = GetDC(m_hWnd);
		if (renderName == L"GDIPlus" && hdc)
		{
			auto _render = render->GetBase<MRender_GDIP>();
			auto customCmd = [&_render, this, &hdc, &rcPaint]()
			{
				HDC pDC = (HDC)_render->GetDC();
				BLENDFUNCTION bf = { AC_SRC_OVER, 0, m_alpha, AC_SRC_ALPHA };
				UIRect rect = GetWindowRect(true);
				int WndWidth = rect.GetWidth();
				int WndHeight = rect.GetHeight();
				if (m_layerWnd)
				{
					rect = GetWindowRect();
					POINT ptDest = { rect.left, rect.top };
					POINT point = { 0,0 };
					SIZE pSize = { WndWidth, WndHeight };
					RECT pDirty = { rcPaint->left, rcPaint->top, rcPaint->right, rcPaint->bottom };
					UPDATELAYEREDWINDOWINFO info;
					info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
					info.crKey = 0;
					info.dwFlags = ULW_ALPHA;
					info.hdcDst = nullptr;
					info.hdcSrc = pDC;
					info.pblend = &bf;
					info.pptDst = &ptDest;
					info.pptSrc = &point;
					info.prcDirty = &pDirty;
					info.psize = &pSize;
					UpdateLayeredWindowIndirect(m_hWnd, &info);
				}
				else
					AlphaBlend(hdc, 0, 0, WndWidth, WndHeight, pDC, 0, 0, WndWidth, WndHeight, bf);
				_render->ReleaseDC();
			};
			render->RunTask(customCmd);
			render->EndDraw();
		}
		ReleaseDC(m_hWnd, hdc);
		ValidateRect(m_hWnd, nullptr);
	}
	bool UIWindowsWnd::InitRender(MRenderCmd* render)
	{
		if (render)
		{
			UIRect rcWindow = GetWindowRect();
			return render->InitRender((_m_uint)rcWindow.GetWidth(), (_m_uint)rcWindow.GetHeight());
		}
		return false;
	}
	MEventCodeEnum UIWindowsWnd::ConvertEventCode(_m_uint src)
	{
		switch (src)
		{
		case WM_MOVE:
			return M_WND_MOVE;
		case WM_SIZE:
			return M_WND_SIZE;
		case WM_SETFOCUS:
			return M_WND_SETFOCUS;
		case WM_KILLFOCUS:
			return M_WND_KILLFOCUS;
		case WM_PAINT:
			return M_WND_PAINT;
		case WM_CLOSE:
			return M_WND_CLOSE;
		case WM_KEYDOWN:
			return M_WND_KEYDOWN;
		case WM_KEYUP:
			return M_WND_KEYUP;
		case WM_COMMAND:
			return M_WND_COMMAND;
		case WM_SYSCOMMAND:
			return M_WND_SYSCOMMAND;
		case WM_TIMER:
			return M_WND_TIMER;
		case WM_MOUSEMOVE:
			return M_MOUSE_MOVE;
		case WM_MOUSEWHEEL:
			return M_MOUSE_WHEEL;
		case WM_MOUSEHOVER:
			return M_MOUSE_HOVER;
		case WM_MOUSELEAVE:
			return M_MOUSE_LEAVE;
		case WM_LBUTTONDOWN:
			return M_MOUSE_LBDOWN;
		case WM_LBUTTONUP:
			return M_MOUSE_LBUP;
		case WM_LBUTTONDBLCLK:
			return M_MOUSE_LBDBCLICK;
		case WM_RBUTTONDOWN:
			return M_MOUSE_RBDOWN;
		case WM_RBUTTONUP:
			return M_MOUSE_RBUP;
		case WM_RBUTTONDBLCLK:
			return M_MOUSE_RBDBCLICK;
		case WM_MBUTTONDOWN:
			return M_MOUSE_MBDOWN;
		case WM_MBUTTONUP:
			return M_MOUSE_MBUP;
		case WM_MBUTTONDBLCLK:
			return M_MOUSE_MBDBCLICK;
		case WM_SETCURSOR:
			return M_SETCURSOR;
		default:
			break;
		}
		return MEventCodeEnum(src);
	}
	_m_uint UIWindowsWnd::ConvertEventCode(MEventCodeEnum src)
	{
		switch (src)
		{
		case M_WND_MOVE:
			return WM_MOVE;
		case M_WND_SIZE:
			return WM_SIZE;
		case M_WND_SETFOCUS:
			return WM_SETFOCUS;
		case M_WND_KILLFOCUS:
			return WM_KILLFOCUS;
		case M_WND_PAINT:
			return WM_PAINT;
		case M_WND_CLOSE:
			return WM_CLOSE;
		case M_WND_KEYDOWN:
			return WM_KEYDOWN;
		case M_WND_KEYUP:
			return WM_KEYUP;
		case M_WND_COMMAND:
			return WM_COMMAND;
		case M_WND_SYSCOMMAND:
			return WM_SYSCOMMAND;
		case M_WND_TIMER:
			return WM_TIMER;
		case M_MOUSE_MOVE:
			return WM_MOUSEMOVE;
		case M_MOUSE_WHEEL:
			return WM_MOUSEWHEEL;
		case M_MOUSE_HOVER:
			return WM_MOUSEHOVER;
		case M_MOUSE_LEAVE:
			return WM_MOUSELEAVE;
		case M_MOUSE_LBDOWN:
			return WM_LBUTTONDOWN;
		case M_MOUSE_LBUP:
			return WM_LBUTTONUP;
		case M_MOUSE_LBDBCLICK:
			return WM_LBUTTONDBLCLK;
		case M_MOUSE_RBDOWN:
			return WM_RBUTTONDOWN;
		case M_MOUSE_RBUP:
			return WM_RBUTTONUP;
		case M_MOUSE_RBDBCLICK:
			return WM_RBUTTONDBLCLK;
		case M_MOUSE_MBDOWN:
			return WM_MBUTTONDOWN;
		case M_MOUSE_MBUP:
			return WM_MBUTTONUP;
		case M_MOUSE_MBDBCLICK:
			return WM_MBUTTONDBLCLK;
		case M_SETCURSOR:
			return WM_SETCURSOR;
		default:
			break;
		}
		return static_cast<_m_uint>(src);
	}
	LRESULT UIWindowsWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto window = (UIWindowsWnd*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		auto param = std::make_pair((_m_param)wParam, (_m_param)lParam);
		if (message == WM_NCCREATE)
		{
			CREATESTRUCT* lpcs = (CREATESTRUCT*)lParam;
			window = (UIWindowsWnd*)lpcs->lpCreateParams;
			window->m_hWnd = hWnd;
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LPARAM)window);
		}
		else if (message == WM_DESTROY)
		{
			if (window)
			{
				LRESULT result = DefWindowProcW(hWnd, message, wParam, lParam);
				SetWindowLongPtrW(window->m_hWnd, GWLP_USERDATA, 0L);
				if (window->IsMainWindow())
					PostQuitMessage(0);
				window->EventSource(window->ConvertEventCode(message), (_m_param)&param);
				return result;
			}
		}
		if (window)
		{
			if (message == WM_MOUSEMOVE)
				window->m_cachePos = (_m_param)lParam;
			else if (message == WM_MOUSEWHEEL)
				param.second = window->m_cachePos;
			else if(message == WM_GETMINMAXINFO)
			{
				auto pMinMaxInfo = (MINMAXINFO*)lParam;
				auto scale = window->GetWindowScale();
				pMinMaxInfo->ptMinTrackSize.x = _scale_to(window->m_minSize.width, scale.cx);
				pMinMaxInfo->ptMinTrackSize.y = _scale_to(window->m_minSize.height, scale.cy);
			}
			if (window->m_notitleWnd)
			{
				switch (message)
				{
				case WM_NCCALCSIZE:
					{
						typedef void (WINAPI* PGetNTVer)(DWORD*, DWORD*, DWORD*);
						static HMODULE hModule = GetModuleHandleW(L"ntdll.dll");
						static auto GetNTVer = (PGetNTVer)GetProcAddress(hModule, "RtlGetNtVersionNumbers");
						DWORD Major = 0;
						GetNTVer(&Major, nullptr, nullptr);
						if (wParam && Major < 10)
							return 1;
						if (!lParam)
						{
							if (const auto ret = window->EventSource(window->ConvertEventCode(message), (_m_param)&param))
								return ret;
							return DefWindowProcW(hWnd, message, wParam, lParam);
						}
						int frameX = GetThemeSysSize(NULL, SM_CXFRAME);
						int frameY = GetThemeSysSize(NULL, SM_CYFRAME);
						int padding = GetThemeSysSize(NULL, SM_CXPADDEDBORDER);
						NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
						RECT* rgrc = params->rgrc;
						rgrc->right -= frameX + padding;
						rgrc->left += frameX + padding;
						rgrc->bottom -= frameY + padding;
						WINDOWPLACEMENT placement = { 0 };
						placement.length = sizeof(WINDOWPLACEMENT);
						if (GetWindowPlacement(hWnd, &placement))
						{
							if (placement.showCmd == SW_SHOWMAXIMIZED)
								rgrc->top += padding;
						}
						return true;
					}
					break;
				case WM_CREATE:
					{
						auto rect = window->GetWindowRect();
						SetWindowPos(hWnd, nullptr, rect.left, rect.top,
						             rect.GetWidth(), rect.GetHeight(), SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
					}
					break;
				default:
					break;
				}
			}
			if (const auto ret = window->EventSource(window->ConvertEventCode(message), (_m_param)&param))
				return ret;
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	BOOL UIWindowsWnd::RegisterWindowClass()
	{
		WNDCLASSEX wcx;
		wcx.cbSize = sizeof(WNDCLASSEX);
		wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcx.lpfnWndProc = UIWindowsWnd::WndProc;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = 0;
		wcx.hInstance = (HINSTANCE)&__ImageBase;
		wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcx.lpszMenuName = NULL;
		wcx.lpszClassName = M_DEF_CLSNAME.data();
		wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		return RegisterClassExW(&wcx) != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	}
}
#endif