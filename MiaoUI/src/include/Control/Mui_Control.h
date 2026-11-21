
#pragma once
#include <Manager/Mui_ControlMgr.h>
#include <Render/Node/Mui_UINodeBase.h>
#include <Render/Graphs/Mui_Render.h>
#include <Mui_Helper.h>
namespace Mui::Ctrl
{
	using namespace Render;
	class UIControl : public UINodeBase
	{
	public:
		UIControl();
		~UIControl() override;
		static constexpr auto ClassName = L"UIControl";
		static void Register();
		virtual std::wstring GetClsName() const { return ClassName; }
		virtual void SetUserData(_m_param data);
		[[nodiscard]] virtual _m_param GetUserData() const;
		virtual void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true);
		virtual std::wstring GetAttribute(std::wstring_view attribName);
		virtual void SetEnabled(bool enabled, bool draw = true);
		[[nodiscard]] virtual bool IsEnabled() const;
		[[nodiscard]] virtual Window::UIWindowBasic* GetParentWin() const;
		template<typename T>
		T* FindChildren(const UIString& name) const
		{
			return mcast_control<T>(Child(name));
		}
		[[nodiscard]] UIControl* Child(const UIString& name) const
		{
			return mcast_control<UIControl>(UINodeBase::FindChildren(name));
		}
		template<typename T>
		T* Child(const UIString& name) const
		{
			return FindChildren<T>(name);
		}
		void AddChildren(UINodeBase* UINode) override;
		virtual void SetFocus(bool focus);
		virtual void SetEnableFocus(bool enable);
		virtual void SetMsgFilter(bool filter);
		virtual void SetMsgIgnore(bool ignore, bool child = true);
		virtual void ScaleControl(_m_uint width, _m_uint height, bool child);
		virtual void ScaleControl(_m_scale scale, bool child);
	protected:
		void SetCapture();
		void ReleaseCapture();
		bool isCapture();
		MTimers::ID SetTimer(_m_uint uTimeout);
		void KillTimer(MTimers::ID idTimer);
		virtual UIControl* FindMouseControl(const UIPoint& point);
		UIControl* GetWindowTopCtrl() const;
		UIControl* GetWindowTopCtrl(Window::UIWindowBasic* wnd) const;
		virtual void SetCursor(_m_lpcwstr cursor_name);
		virtual bool OnMouseMessage(MEventCodeEnum message, _m_param wParam, _m_param lParam);
		virtual bool OnWindowMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam);
		virtual bool OnMouseEntered(_m_uint flag, const UIPoint& point);
		virtual bool OnMouseExited(_m_uint flag, const UIPoint& point);
		virtual bool OnLButtonDown(_m_uint flag, const UIPoint& point);
		virtual bool OnLButtonUp(_m_uint flag, const UIPoint& point);
		virtual bool OnLButtonDoubleClicked(_m_uint flag, const UIPoint& point);
		virtual bool OnRButtonDown(_m_uint flag, const UIPoint& point);
		virtual bool OnRButtonUp(_m_uint flag, const UIPoint& point);
		virtual bool OnRButtonDoubleClicked(_m_uint flag, const UIPoint& point);
		virtual bool OnMButtonDown(_m_uint flag, const UIPoint& point);
		virtual bool OnMButtonUp(_m_uint flag, const UIPoint& point);
		virtual bool OnMButtonDoubleClicked(_m_uint flag, const UIPoint& point);
		virtual bool OnMouseWheel(_m_uint flag, short delta, const UIPoint& point);
		virtual bool OnMouseMove(_m_uint flag, const UIPoint& point);
		virtual bool OnSetCursor(_m_param hCur, _m_param lParam);
		virtual void OnTimer(MTimers::ID idTimer);
		virtual bool SendEvent(UINotifyEvent event, _m_param param = 0);
		[[nodiscard]] MPenPtr GetDbgFramePen() const;
		struct ControlData
		{
			_m_param UserData = 0;
			bool Enabled = true;
			bool ParentEnabled = true;
			bool MsgFilter = false;
			bool MsgIgnore = false;
			bool MsgIgnoreChild = false;
			bool IsFocus = true;
		} m_data;
	private:
		bool DispatchMouseMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam);
		bool DispatchWindowMessage(MEventCodeEnum code, _m_param wParam, _m_param lParam);
		friend class Window::UIWindowBasic;
	};
}