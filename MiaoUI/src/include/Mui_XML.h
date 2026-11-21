
#pragma once
#include <Mui_Base.h>
#include <Window/Mui_BasicWnd.h>
#include <Control/Mui_Label.h>
#ifndef MUIEVENT
#define MUIEVENT(_event, _name) (event == (_event) && control->GetName() == (_name))
#endif
#ifndef MUIEVENTS
#define MUIEVENTS(_event, _control) (event == (_event) && control == (_control))
#endif
#ifndef MXMLCODE
#define MXMLCODE(code) L#code
#endif
namespace Mui::XML
{
	class MuiXML
	{
	public:
		UIResourceMgr* Mgr() { return m_window->GetResourceMgr(); }
		bool CreateUIFromXML(Ctrl::UIControl* parents, std::wstring xmlDoc);
		bool AddFontStyle(std::wstring_view name, const Ctrl::UILabel::Attribute& style);
#if MUI_MXML_ENABLE_DEFSTYLE
		void LoadDefaultStyle();
#endif
		Ctrl::UILabel::Attribute GetFontStyle(std::wstring_view name);
		bool DeleteFontStyle(std::wstring_view name);
		bool CreatePropGroup(std::wstring_view id);
		bool DeletePropGroup(std::wstring_view id);
		bool DeletePropGroupAttrib(std::wstring_view id, std::wstring_view attribName);
		bool SetPropGroupAttrib(std::wstring_view id, std::wstring_view attribName, std::wstring_view attribValue);
		bool AddPropGroupAttrib(std::wstring_view id, std::wstring_view attribName, std::wstring_view attribValue);
		bool ApplyPropGroup(Ctrl::UIControl* dst, std::wstring_view id, bool draw = true);
		bool AddDefPropGroup(std::wstring_view xml, bool replace);
		bool DeleteDefPropGroup(std::wstring_view name);
		bool SetDefPropGroupAttrib(std::wstring_view name,
			const std::vector<std::pair<std::wstring, std::wstring>>& attribList, bool draw = true);
		bool AddStringList(std::wstring_view name, std::wstring_view value);
		bool SetStringValue(std::wstring_view name, std::wstring_view value, bool draw = true);
		bool DeleteStringList(std::wstring_view name);
		std::wstring GetStringValue(std::wstring_view name);
		virtual void SetAttribute(Ctrl::UIControl* control, std::wstring_view name, std::wstring value);
	protected:
		MuiXML(Window::UIWindowBasic* window);
		virtual ~MuiXML();
		virtual Ctrl::UIControl* CreateControl(std::wstring_view controlName, Ctrl::UIControl* parent);
		void SetAttributeInternal(Ctrl::UIControl* control, std::wstring_view name, std::wstring value, bool strlist);
		Ctrl::UILabel::Attribute* FindFontStyle(std::wstring_view name);
		Ctrl::UILabel::Attribute defaultStyle;
		std::unordered_map<std::wstring, Ctrl::UILabel::Attribute> m_fontStyleList;
		struct PropGroup
		{
			std::unordered_map<std::wstring, std::wstring> prop;
			std::wstring id;
		};
		std::vector<PropGroup> m_propList;
		std::vector<PropGroup> m_defpropList;
		Window::UIWindowBasic* m_window = nullptr;
		struct bindInfo
		{
			struct strdata
			{
				std::pair<std::wstring, std::wstring> str;
				int menuIndex = -1;
			};
			std::vector<strdata> strList;
		};
		std::unordered_map<Ctrl::UIControl*, bindInfo> m_bindCtrlList;
		std::unordered_map<std::wstring, std::wstring> m_stringList;
#if MUI_MXML_ENABLE_DEFSTYLE
		bool m_defStyleInited = false;
#endif
		friend class Window::UIWindowBasic;
	};
}