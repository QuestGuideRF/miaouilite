
#pragma once
#include <Render/Mui_RenderMgr.h>
#include <Render/Node/Mui_RenderNode.h>
#include <Manager/Mui_ResourceMgr.h>
namespace Mui
{
	namespace XML { class MuiXML; }
	namespace Render { class UINodeBase; }
	class MiaoUI;
	class MWindowCtx;
	namespace Window::IWindow
	{
		class UINativeWindow
		{
		public:
			UINativeWindow() = default;
			virtual ~UINativeWindow() = default;
			UINativeWindow(UINativeWindow&&) = delete;
			UINativeWindow(const UINativeWindow&) = delete;
			UINativeWindow& operator=(const UINativeWindow&) = delete;
			UINativeWindow& operator=(UINativeWindow&&) = delete;
			virtual void SetWindowTitle(std::wstring_view title) = 0;
			virtual std::wstring GetWindowTitle() = 0;
			virtual void ShowWindow(bool show, bool focus = true) = 0;
			virtual bool IsShowWindow() = 0;
			virtual void EnableWindow(bool enable) = 0;
			virtual bool IsEnableWindow() = 0;
			virtual bool IsMinimize() = 0;
			virtual bool IsMaximize() = 0;
			virtual void CenterWindow() = 0;
			virtual void CloseWindow() = 0;
			virtual void SetWindowRect(UIRect rect) = 0;
			virtual UIRect GetWindowRect(bool client = false) = 0;
			virtual void SetWindowAlpha(_m_byte alpha, bool draw = true) = 0;
			virtual _m_byte GetWindowAlpha() = 0;
			virtual _m_ptrv GetWindowHandle() = 0;
			template<typename T>
			T GetWindowHandle() { return reinterpret_cast<T>(GetWindowHandle()); }
			virtual void SetMinimSize(UISize min) = 0;
		protected:
			virtual UISize GetWindowSrcSize() = 0;
		};
	}
	namespace Window
	{
		class UIWindowBasic : public IWindow::UINativeWindow, MThreadT<std::recursive_mutex>
		{
		public:
			UIWindowBasic(Render::Def::MRender* render);
			~UIWindowBasic() override;
			UIWindowBasic(UIWindowBasic&&) = delete;
			UIWindowBasic(const UIWindowBasic&) = delete;
			UIWindowBasic& operator=(const UIWindowBasic&) = delete;
			UIWindowBasic& operator=(UIWindowBasic&&) = delete;
			virtual Ctrl::UIControl* GetRootControl() const;
			virtual void UpdateLayout(MPCRect rect);
			virtual void UpdateDisplay(MPCRect rect);
			virtual void SetRenderMode(bool active);
			virtual bool GetRenderMode() const;
			virtual void ScaleWindow(_m_uint newWidth, _m_uint newHeight);
			virtual void ScaleWindow(_m_scale scale);
			virtual _m_scale GetWindowScale() const;
			virtual MTimers::ID SetTimer(_m_uint elapse, const MTimerCallback& callback = nullptr);
			virtual void KillTimer(MTimers::ID id);
			virtual void SetMaxFPSLimit(int fps);
			virtual _m_uint GetLastFPS() const;
			virtual void SetMainWindow(bool main);
			virtual bool IsMainWindow() const;
			virtual void SetCacheMode(bool cache);
			virtual bool GetResMode();
			Render::MRenderCmd* GetRender() const;
			UIResourceMgr* GetResourceMgr() const;
			virtual UIFocus GetFocusControl() const;
			Ctrl::UIControl* FindTopLevelControl(std::wstring_view name) const;
			void ShowDebugRect(bool show);
			void ShowHighlight(bool show);
			virtual void UpdateCache();
			XML::MuiXML* XMLUI() { return m_xmlUI; }
			struct renderData
			{
			};
		protected:
			virtual void SetFocusControl(Ctrl::UIControl* control);
			virtual void SetCapture(Ctrl::UIControl* control);
			virtual void ReleaseCapture();
			virtual _m_result EventSource(MEventCodeEnum code, _m_param param);
			bool DispatchControlMessage(Ctrl::UIControl* control, MEventCodeEnum code, _m_param param, bool mouse);
			bool EventMouseProc(MEventCodeEnum code, _m_param param);
			virtual bool EventProc(UINotifyEvent event, Ctrl::UIControl* contorl, _m_param param) = 0;
			virtual void Present(Render::MRenderCmd* render, const _m_rect_t<int>* dirtyArea) = 0;
			virtual bool InitRender(Render::MRenderCmd* render) = 0;
			void SetInited(bool inited);
			void UpdateScale();
			void LoadNodeResource(bool recreate);
		private:
			Render::Def::MRender* m_render = nullptr;
			Render::MRenderCmd* m_renderCmd = nullptr;
			UIResourceMgr* m_resourceMgr = nullptr;
			Ctrl::UIControl* m_root = nullptr;
			Ctrl::UIControl* m_toplay = nullptr;
			Ctrl::UIControl* m_rootBox = nullptr;
			_m_scale m_scale = { 1.f,1.f };
			_m_byte m_alpha = 255;
			MTimers* m_timer = nullptr;
			UIFocus m_focus = { nullptr };
			Ctrl::UIControl* m_capture = nullptr;
			Ctrl::UIControl* m_mouseCur = nullptr;
			bool m_mouseIn = false;
			MFPSCounter m_fpsCounter;
			_m_uint m_fpsCache = 0;
			bool m_isMainWnd = false;
			bool m_cacheRes = false;
			bool m_inited = false;
			Render::MNodeRoot* m_renderRoot;
			Render::MPenPtr m_dbgFrame = nullptr;
			bool m_highlight = false;
			bool m_updateCache = true;
			XML::MuiXML* m_xmlUI = nullptr;
			Render::MRingQueue<_m_rect_t<int>> m_drawCmdList;
			std::atomic_bool m_renderMode;
			void RenderControlTree(const _m_rect_t<int>* dirtyArea);
			void ThreadProc();
			void FreeCurMouseCtrl();
			struct taskParam
			{
				std::function<void()> task = nullptr;
				std::promise<void>& notification;
			};
			std::vector<taskParam> m_threadTaskList;
			void ExecuteThreadTask(const std::function<void()>& task);
			void ResumeThread();
			friend class Ctrl::UIControl;
			friend class Render::UINodeBase;
			friend class XML::MuiXML;
			friend class Mui::MiaoUI;
			friend class Mui::MWindowCtx;
		};
	}
}