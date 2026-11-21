
#pragma once
#include <string>
#include <sstream>
#ifdef __ANDROID__
#include <android/log.h>
#endif
namespace Mui
{
#ifndef __m_dbg_info__
#if (MUI_CFG_ENABLE_DBGSOURCE && MUI_CFG_ENABLE_DBGFUNNAME)
	#define __m_dbg_info__ __FILE__, __FUNCTION__
	extern std::wstring _m_dbg_format_(std::string_view __FILE, std::string_view __FUN);
#elif (MUI_CFG_ENABLE_DBGSOURCE || MUI_CFG_ENABLE_DBGFUNNAME)
	#if MUI_CFG_ENABLE_DBGSOURCE
		#define __m_dbg_info__ __FILE__
	#else
		#define __m_dbg_info__ __FUNCTION__
	#endif
	extern std::wstring _m_dbg_format_(std::string_view __INFO);
#else
	#define __m_dbg_info__
	extern std::wstring _m_dbg_format_();
#endif
#endif
#ifndef _M_OutErrorDbg_
#define _M_OutErrorDbg_(__error, __exit) M_OutError(_m_dbg_format_(__m_dbg_info__), __error, __exit)
#endif
#ifndef _M_OutMErrorDbg_
#define _M_OutMErrorDbg_(__error) M_OutError(__error, _m_dbg_format_(__m_dbg_info__))
#endif
#ifndef M_ASSERT
#define M_ASSERT(x) if(!(x)) _M_OutErrorDbg_(L"Ссылка на нулевой указатель", true);
#endif
	extern std::exception_ptr _g_last_merror_exception;
	extern void _g_last_exception_callback(const MError& ex);
	extern void M_OutError(std::wstring_view cls, std::wstring_view error, bool exit = false);
	extern void M_OutError(const MError& err, std::wstring_view cls = L"");
	template<typename T>
	std::wstring _m_dbg_cast_(T type)
	{
		std::wostringstream stream;
		stream << type;
		return stream.str();
	}
	template<typename T>
	void M_DbgOutInfo(T info)
	{
#ifdef _WIN32
		OutputDebugStringW(_m_dbg_cast_(info).c_str());
#elif __ANDROID__
		__android_log_print(ANDROID_LOG_INFO, "[Отлаживать]", "%s", M_WStringToString(info).c_str());
#endif
	}
	template<typename T, typename... Args>
	void M_DbgOutInfo(T info, Args... args)
	{
		M_DbgOutInfo(_m_dbg_cast_(info).c_str());
		M_DbgOutInfo(args...);
	}
	template<typename ...Args>
	void MErrorThrow(Args&&... args)
	{
		auto err = MError(std::forward<Args>(args)...);
#ifdef _DEBUG
		M_OutError(err);
#endif
		_g_last_merror_exception = std::make_exception_ptr(err);
		throw std::move(err);
	}
	void M_SetLastExceptionNotify(const std::function<void(const MError&)>& callback = _g_last_exception_callback);
	void M_ThreadPostException(std::exception_ptr ex);
}