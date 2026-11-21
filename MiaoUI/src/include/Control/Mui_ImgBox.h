
#pragma once
#include <Control/Mui_Control.h>
namespace Mui::Ctrl
{
	class UIImgBox : public UIControl
	{
	public:
		MCTRL_DEFINE
		(
			L"UIImgBox",
			{ CtrlMgr::AttribType::UIBitmap, L"img" }
		);
		UIImgBox(UIControl* parent);
		enum ImgStyle
		{
			ImageBoxStyle_Zoom,
			ImageBoxStyle_Original,
			ImageBoxStyle_Center,
			ImageBoxStyle_ZoomFill
		};
		UISize GetImageSize() const;
		void SetImage(UIBitmapPtr img, bool lowQuality = false, bool draw = true);
		void SetImageStyle(ImgStyle style, bool draw = true);
		void SetQuality(bool lowQuality, bool draw = true);
		void SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw = true) override;
		std::wstring GetAttribute(std::wstring_view attribName) override;
	protected:
		UIImgBox() { m_cacheSupport = true; }
		void OnPaintProc(MPCPaintParam param) override;
		_m_sizef GetContentSize() override;
		ImgStyle m_style = ImageBoxStyle_Zoom;
		UIBitmapPtr m_image = nullptr;
		bool m_isLowQuality = false;
	};
}