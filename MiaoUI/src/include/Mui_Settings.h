
#pragma once
#include <Mui_Base.h>
namespace Mui::Settings
{
	extern const wchar_t* MuiLogoText;
	extern const wchar_t* MuiEngineVer;
	extern void UIMessageLoop();
	enum ExSection
	{
		Render_D2D_Factory_Flag_Options,
		Render_D2D_D3DCreateDeviceFlag,
		Render_D2D_D3DCreateDeviceDriverType,
		Render_D2D_RenderNoDXGI,
		Graphic_Atlas_Size_Width,
		Graphic_Atlas_Size_Height,
		Graphic_Atlas_MaxCachePixel,
		MXML_SVG_Rasterized_Width,
		MXML_SVG_Rasterized_Height,
		Test_DeviceLost
	};
	extern void SetExtensionFlag(ExSection section, _m_param flag, bool override);
	extern void ResExtensionFlag(ExSection section);
	using ExFlag = std::pair<_m_param, bool>;
	extern bool GetExtensionFlag(ExSection section, ExFlag& param);
}