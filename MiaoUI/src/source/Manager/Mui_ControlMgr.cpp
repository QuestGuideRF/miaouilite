
#include <Manager/Mui_ControlMgr.h>
#include <Control/Mui_Button.h>
#include <Control/Mui_CheckBox.h>
#include <Control/Mui_Combox.h>
#include <Control/Mui_EditBox.h>
#include <Control/Mui_ImgBox.h>
#include <Control/Mui_Label.h>
#include <Control/Mui_ListBox.h>
#include <Control/Mui_Progress.h>
#include <Control/Mui_Scroll.h>
#include <Control/Mui_Slider.h>
#include <Control/Mui_ColorPicker.h>
#include <Control/Mui_NavBar.h>
namespace Mui::CtrlMgr
{
	using namespace Ctrl;
	struct RegData
	{
		RegMethod method;
		std::vector<AttribType> resAttrib;
	};
	bool g_isRegMui = false;
	std::unordered_map<std::wstring, RegData> g_regList;
	bool RegisterControl(std::wstring_view name, RegMethod&& method, const std::vector<AttribType>& ptrAttrib)
	{
		auto iter = g_regList.find(name.data());
		if (iter != g_regList.end())
			return false;
		g_regList.insert(std::make_pair(name, RegData{ method, ptrAttrib }));
		return true;
	}
	void RegisterMuiControl()
	{
		if (!g_isRegMui)
		{
			UIButton::Register();
			UICheckBox::Register();
			UIComBox::Register();
			UIControl::Register();
			UIEditBox::Register();
			UIImgBox::Register();
			UILabel::Register();
			UIListBox::Register();
			UIProgressBar::Register();
			UIScroll::Register();
			UISlider::Register();
			UIColorPicker::Register();
			UINavBar::Register();
		}
	}
	UIControl* CreateControl(std::wstring_view name, UIControl* parent)
	{
		UIControl* ret = nullptr;
		auto iter = g_regList.find(name.data());
		if (iter != g_regList.end() && iter->second.method)
		{
			ret = iter->second.method(parent);
		}
		return ret;
	}
	AttribType::type GetAttributeType(std::wstring_view name, std::wstring_view attribName)
	{
		auto iter = g_regList.find(name.data());
		if (iter != g_regList.end())
		{
			for (auto& type : iter->second.resAttrib)
			{
				if (type.attribName == attribName)
					return type.attribType;
			}
		}
		return AttribType::defaults;
	}
#define DEF_ConverGet [](std::wstring_view value) -> std::any
#define DEF_ConverSet [](std::any value) -> std::wstring
#define DEF_NumberConver(type, func) AddMethod<type>( \
	DEF_ConverGet{ return std::make_any<type>(func(value.data())); }, \
		DEF_ConverSet{ return std::to_wstring(std::any_cast<type>(value)); } \
	)
#define DEF_MPtrConver(type) AddMethod<type>( \
	DEF_ConverGet{ return std::make_any<type>(type((type::Type*)std::stoull(value.data()))); }, \
		DEF_ConverSet{ return std::to_wstring((_m_ptrv)std::any_cast<type>(value).get()); } \
	)
	std::unordered_map<std::string, AttribConverter::data> AttribConverter::m_methodList;
	template<typename T, int _count>
	std::any AttribInt_ConvGetMethod(std::wstring_view value)
	{
		std::vector<int> dstValue;
		Helper::M_GetAttribValueInt(value, dstValue, _count);
		if constexpr (_count == 4)
		{
			return std::make_any<T>(T{ dstValue[0], dstValue[1], dstValue[2], dstValue[3] });
		}
		else
			return std::make_any<T>(T{ dstValue[0], dstValue[1] });
	}
	template<typename T>
	std::wstring RectInt_ConvSetMethod(std::any value)
	{
		auto rect = std::any_cast<T>(value);
		return Attrib::Value_Make4x(rect.left, rect.top, rect.right, rect.bottom);
	}
	AttribConverter::AttribConverter()
	{
		AddMethod<bool>(
			DEF_ConverGet{ return std::make_any<bool>(value == L"true"); },
			DEF_ConverSet{ return std::any_cast<bool>(value) ? L"true" : L"false"; }
		);
		DEF_NumberConver(float, std::stof);
		DEF_NumberConver(double, std::stod);
		AddMethod<std::wstring>(
			DEF_ConverGet{ return std::make_any<std::wstring>(value); },
			DEF_ConverSet{ return std::any_cast<std::wstring>(value); }
		);
		AddMethod<std::wstring_view>(
			DEF_ConverGet{ return std::make_any<std::wstring_view>(value); },
			DEF_ConverSet{ return std::any_cast<std::wstring_view>(value).data(); }
		);
		AddMethod<wchar_t>(
			DEF_ConverGet{ return std::make_any<wchar_t>(value[0]);},
			DEF_ConverSet{ std::wstring str; str = std::any_cast<wchar_t>(value); return str; }
		);
		DEF_NumberConver(_m_uchar, (_m_uchar)std::stoi);
		DEF_NumberConver(_m_short, (_m_short)std::stoi);
		DEF_NumberConver(_m_ushort, (_m_ushort)std::stoi);
		DEF_NumberConver(_m_int, (_m_int)std::stoi);
		DEF_NumberConver(_m_uint, (_m_uint)std::stoul);
		DEF_NumberConver(_m_param, std::stoll);
		DEF_NumberConver(_m_ulong64, std::stoull);
		AddMethod<void*>(
			DEF_ConverGet{ return std::make_any<void*>((void*)std::stoull(value.data())); },
			DEF_ConverSet{ return std::to_wstring((_m_ptrv)std::any_cast<void*>(value)); }
		);
		AddMethod<UIStyle*>(
			DEF_ConverGet{ return std::make_any<UIStyle*>((UIStyle*)std::stoull(value.data())); },
			DEF_ConverSet{ return std::to_wstring((_m_ptrv)std::any_cast<UIStyle*>(value)); }
		);
		AddMethod<UIBitmap*>(
			DEF_ConverGet{ return std::make_any<UIBitmap*>((UIBitmap*)std::stoull(value.data())); },
			DEF_ConverSet{ return std::to_wstring((_m_ptrv)std::any_cast<UIBitmap*>(value)); }
		);
		AddMethod<_m_color>(
			DEF_ConverGet{ return std::make_any<_m_color>(Helper::M_GetAttribValueColor(value)); },
			DEF_ConverSet
			{
				_m_color color = std::any_cast<_m_color>(value);
				return Color::M_RGBA_STR(color.r, color.g, color.b, color.a);
			}
		);
		AddMethod<_m_rect>(AttribInt_ConvGetMethod<_m_rect, 4>, RectInt_ConvSetMethod<_m_rect>);
		AddMethod<_m_rect_t<int>>(AttribInt_ConvGetMethod<_m_rect_t<int>, 4>, RectInt_ConvSetMethod<_m_rect_t<int>>);
		AddMethod<UIRect>(AttribInt_ConvGetMethod<UIRect, 4>, RectInt_ConvSetMethod<UIRect>);
		AddMethod<UIPoint>(
			AttribInt_ConvGetMethod<UIPoint, 2>,
			DEF_ConverSet
			{
				auto pt = std::any_cast<UIPoint>(value);
				return Attrib::Value_Make2x(pt.x, pt.y);
			}
		);
		AddMethod<UISize>(
			AttribInt_ConvGetMethod<UISize, 2>,
			DEF_ConverSet
			{
				auto size = std::any_cast<UISize>(value);
				return Attrib::Value_Make2x(size.width, size.height);
			}
		);
		DEF_MPtrConver(UIStylePtr);
		DEF_MPtrConver(UIBitmapPtr);
	}
 }