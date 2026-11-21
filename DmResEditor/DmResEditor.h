#pragma once
#include "resource.h"
#include "Mui.h"
extern HINSTANCE m_hInstance;
using namespace Mui;
class MainWindow : public Window::UIWindowsWnd
{
public:
	MainWindow(Render::MRender* pRender) : UIWindowsWnd(pRender) {}
	bool AfterCreated();
	virtual bool EventProc(UINotifyEvent event, Ctrl::UIControl* control, _m_param param) override;
	virtual _m_result EventSource(MEventCodeEnum code, _m_param param) override;
private:
	void CreateControls();
	void WriteRes(Ctrl::UIControl* control);
	void WriteSuccess(_m_size ressize);
	void SaveRes(Ctrl::UIControl* control);
	void OnSelChange(Ctrl::UIControl* control);
	void ShowResSize(_m_size size);
	void ShowStrLength(_m_size lenght);
	int curSelItem = 0;
	friend void blockcallback(_m_size size, _m_size allsize, _m_param param);
};