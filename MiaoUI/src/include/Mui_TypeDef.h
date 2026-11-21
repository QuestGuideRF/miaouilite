
#pragma once
#include <cstdint>
namespace Mui
{
	using _m_uchar = std::uint8_t;
	using _m_short = std::int16_t;
	using _m_ushort = std::uint16_t;
	using _m_int = std::int32_t;
	using _m_uint = std::uint32_t;
	using _m_param = std::int64_t;
	using _m_ptr = void*;
	using _m_ptrv = std::uintptr_t;
	using _m_long = long;
	using _m_long64 = _m_param;
	using _m_ulong = _m_uint;
	using _m_ulong64 = std::uint64_t;
	using _m_result = _m_long;
	using _m_byte = std::uint8_t;
	using _m_lpbyte = _m_byte*;
	using _m_word = _m_ushort;
	using _m_size = _m_ulong64;
	using _m_lpcwstr = const wchar_t*;
	using _m_msg = _m_int;
	constexpr auto M_DEF_SYSTEM_FONTNAME =
#ifdef _WIN32
		L"Пользовательский интерфейс Microsoft YaHey";
#elif __ANDROID__
		L"Ното Санс CJK TC";
#else
#error __TODO__
#endif
	enum MEventCodeEnum
	{
		M_WND_MOVE = -0x0ffffff,
		M_WND_SIZE,
		M_WND_SETFOCUS,
		M_WND_KILLFOCUS,
		M_WND_PAINT,
		M_WND_CLOSE,
		M_WND_KEYDOWN,
		M_WND_KEYUP,
		M_WND_COMMAND,
		M_WND_SYSCOMMAND,
		M_WND_TIMER,
		M_MOUSE_MOVE,
		M_MOUSE_WHEEL,
		M_MOUSE_HOVER,
		M_MOUSE_LEAVE,
		M_MOUSE_LBDOWN,
		M_MOUSE_LBUP,
		M_MOUSE_LBDBCLICK,
		M_MOUSE_RBDOWN,
		M_MOUSE_RBUP,
		M_MOUSE_RBDBCLICK,
		M_MOUSE_MBDOWN,
		M_MOUSE_MBUP,
		M_MOUSE_MBDBCLICK,
		M_SETCURSOR
	};
	enum TextAlign
	{
		TextAlign_Left = 0x0000,
		TextAlign_Center = 0x0001,
		TextAlign_Right = 0x0002,
		TextAlign_Top = 0x0000,
		TextAlign_VCenter = 0x0004,
		TextAlign_Bottom = 0x0008,
	};
	enum UINotifyEvent
	{
		Event_Mouse_Hover,
		Event_Mouse_Exited,
		Event_Mouse_Move,
		Event_Mouse_LDown,
		Event_Mouse_LUp,
		Event_Mouse_LClick,
		Event_Mouse_LDoubleClicked,
		Event_Mouse_MDown,
		Event_Mouse_MUp,
		Event_Mouse_MDoubleClicked,
		Event_Mouse_RDown,
		Event_Mouse_RUp,
		Event_Mouse_RDoubleClicked,
		Event_Mouse_Wheel,
		Event_Control_SetCursor,
		Event_Control_OnTimer,
		Event_Focus_True,
		Event_Focus_False,
		Event_Key_Down,
		Event_Key_Up,
		Event_Edit_TextChanged,
		Event_ListBox_ItemLClick,
		Event_ListBox_ItemLDBClick,
		Event_ListBox_ItemChanging,
		Event_ListBox_ItemChanged,
		Event_Slider_Change,
		Event_ColorPicker_Change,
		Event_NavBar_ItemChange,
		Event_Menu_ItemLClick
	};
	enum UIControlStatus
	{
		UIControlStatus_Normal,
		UIControlStatus_Hover,
		UIControlStatus_Pressed,
		UIControlStatus_Disable,
	};
	enum UIAlignment
	{
		UIAlignment_Block,
		UIAlignment_LinearV,
		UIAlignment_LinearVR,
		UIAlignment_LinearVB,
		UIAlignment_LinearVBR,
		UIAlignment_LinearH,
		UIAlignment_LinearHB,
		UIAlignment_LinearHL,
		UIAlignment_LinearHLB,
		UIAlignment_Absolute,
		UIAlignment_Center,
		UIAlignment_Grid
	};
}