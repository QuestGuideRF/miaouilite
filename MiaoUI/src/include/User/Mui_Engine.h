
#pragma once
#include <Window/Mui_BasicWnd.h>
#include <Mui_XML.h>
namespace Mui
{
	class MWindowCtx;
	class MiaoUI final
	{
	public:
		MiaoUI(MiaoUI&&) = delete;
		MiaoUI(const MiaoUI&) = delete;
		MiaoUI& operator=(const MiaoUI&) = delete;
		MiaoUI& operator=(MiaoUI&&) = delete;
		MiaoUI() = default;
		~MiaoUI();
		enum class Render
		{
			Auto,
			Direct2D_3D,
			OpenGL_Core,
			OpenGL_ES,
			Custom,
			Gdiplus
		};
		enum class MWindowType
		{
			Normal,
			NoTitleBar,
			Popup,
			Layer,
		};
		bool InitEngine(std::wstring& error, Render render = Render::Auto, _m_ptrv param = 0);
		void UnInitEngine();
		bool AddResourcePath(std::wstring_view path, std::wstring_view key);
		bool AddResourceMem(UIResource memfile, std::wstring_view key);
		bool RemoveResource(std::wstring_view path);
		MWindowCtx* CreateWindowCtx(UIRect rect, MWindowType type, std::wstring_view title,
			bool main, bool dpi = true, _m_param parent = 0, _m_ptrv exparam = 0);
	private:
		std::vector<MWindowCtx*> m_windowList{};
		std::unordered_map<std::wstring, std::wstring> m_res_pathList{};
		std::vector<std::pair<UIResource, std::wstring>> m_res_memList{};
		bool m_isinit = false;
		Render m_renderType = Render::Auto;
		_m_ptrv m_customRender = 0;
		Mui::Render::MRender* CreateRender();
		friend class MWindowCtx;
	};
	using MWndEventCallback = std::function<bool(MWindowCtx*, UINotifyEvent, Ctrl::UIControl*, _m_param)>;
	using MWndDefEventSource = std::function<_m_result(MEventCodeEnum, _m_param)>;
	using MWndSourceCallback = std::function<_m_result(MWindowCtx*, MWndDefEventSource, MEventCodeEnum, _m_param)>;
	using MWndInitUICallback = std::function<bool(MWindowCtx*, Ctrl::UIControl*, XML::MuiXML*)>;
	class MWindowCtx
	{
	public:
		MWindowCtx(MWindowCtx&&) = delete;
		MWindowCtx(const MWindowCtx&) = delete;
		MWindowCtx& operator=(const MWindowCtx&) = delete;
		MWindowCtx& operator=(MWindowCtx&&) = delete;
		virtual ~MWindowCtx();
		[[nodiscard]] Window::UIWindowBasic* Base() const;
		[[nodiscard]] XML::MuiXML* XML() const;
		bool InitWindow(const MWndInitUICallback& callback, bool visible);
		void SetEventCallback(MWndEventCallback callback);
		void SetEventSourceCallback(MWndSourceCallback callback);
		void EventLoop();
	protected:
		MWindowCtx();
		Window::UIWindowBasic* m_base = nullptr;
		MiaoUI* m_engine = nullptr;
		bool m_isInit = false;
		friend class MiaoUI;
	};
}