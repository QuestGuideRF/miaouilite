
#include <Mui_Settings.h>
#include <Mui_Base.h>
namespace Mui::Settings
{
	_m_lpcwstr MuiLogoText = LR"(
___  ___ _                _   _  _____
| .\/. |(_)  __ _   ___  | | | ||_   _|
| |\/| || | / _` | / _ \ | | | |  | |
| |  | || || (_| || (_) || |_| | _| |_
\_|  |_/|_| \__,_| \___/  \___/  \___/
)";
#ifdef _WIN32
	_m_lpcwstr MuiEngineVer = L"v2.0.8 (Лайт)";
#endif
	void UIMessageLoop()
	{
#ifdef _WIN32
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif
	}
	std::unordered_map<ExSection, ExFlag> g_exflagList;
	std::mutex g_exflagLock;
	void SetExtensionFlag(ExSection section, _m_param flag, bool override)
	{
		std::lock_guard lock{ g_exflagLock };
		g_exflagList[section] = { flag, override };
	}
	void ResExtensionFlag(ExSection section)
	{
		std::lock_guard lock{ g_exflagLock };
		if (auto iter = g_exflagList.find(section); iter != g_exflagList.end())
			g_exflagList.erase(iter);
	}
	bool GetExtensionFlag(ExSection section, std::pair<_m_param, bool>& param)
	{
		std::lock_guard lock{ g_exflagLock };
		if (const auto iter = g_exflagList.find(section); iter != g_exflagList.end())
		{
			param = iter->second;
			return true;
		}
		return false;
	}
};