
#pragma once
#include <Mui_DefUIStyle.h>
#include <Mui_Settings.h>
#include <Control/Mui_Button.h>
#include <Control/Mui_CheckBox.h>
#include <Control/Mui_ComBox.h>
#include <Control/Mui_EditBox.h>
#include <Control/Mui_ImgBox.h>
#include <Control/Mui_Label.h>
#include <Control/Mui_ListBox.h>
#include <Control/Mui_Progress.h>
#include <Control/Mui_Scroll.h>
#include <Control/Mui_Slider.h>
#include <Control/Mui_ColorPicker.h>
#include <Control/Mui_NavBar.h>
#include <Manager/Mui_ControlMgr.h>
#include <Window/Mui_Windows.h>
#include <Render/Graphs/Mui_GdipRender.h>
#include <User/Mui_Engine.h>
#include <FileSystem/Mui_FileSystem.h>
#ifdef _DEBUG
#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "MiaoUILite64d.lib")
#else
#pragma comment(lib, "MiaoUILite32d.lib")
#endif
#endif
#else
#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "MiaoUILite64.lib")
#else
#pragma comment(lib, "MiaoUILite32.lib")
#endif
#endif
#endif