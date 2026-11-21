
#include <Mui_Error.h>
#include <Mui_Helper.h>
#include <iomanip>
#include <sstream>
namespace Mui
{
	MError::MError(MErrorCode err, _m_error ext) noexcept
	{
		m_errCode = err;
		m_extCode = ext;
	}
	MError::MError(const char* what) noexcept
		: std::exception(what)
	{
		m_errCode = MErrorCode::STDError;
	}
	MError::MError(_m_error hresult, std::wstring_view err)
	{
		m_errCode = MErrorCode::WinCOMError;
		m_extCode = hresult;
		m_extInfo = err;
	}
	MError::MError(std::wstring_view info)
	{
		m_errCode = MErrorCode::MUIError;
		m_extInfo = info;
	}
	const char* MError::what() const noexcept
	{
		if (auto err = exception::what(); m_errCode == MErrorCode::STDError && err)
			return err;
		return toMessage(m_errCode);
	}
	std::wstring MError::whatW() const
	{
		if (auto err = exception::what(); m_errCode == MErrorCode::STDError && err)
			return Helper::M_StringToWString(err);
		if (m_errCode == MErrorCode::WinCOMError || m_errCode == MErrorCode::MUIError)
			return m_extInfo;
		return toMessageW(m_errCode);
	}
	MErrorCode MError::errCode() const noexcept
	{
		return m_errCode;
	}
	_m_error MError::extCode() const noexcept
	{
		return m_extCode;
	}
	const char* MError::toMessage(MErrorCode errCode)
	{
		switch (errCode)
		{
		case MErrorCode::OK:
			return "OK.";
		case MErrorCode::False:
			return "Failed.";
		case MErrorCode::InvalidParameter:
			return "Неверные параметры, один или несколько параметров недействительны.";
		case MErrorCode::NotSupported:
			return "Текущая операция или функция не поддерживается.";
		case MErrorCode::InvalidData:
			return "Неверные данные или неправильное форматирование.";
		case MErrorCode::DeviceLost:
			return "Устройство было утеряно или его статус изменился, удалите связанный ресурс устройства и создайте его заново.";
		case MErrorCode::InternalDeviceError:
			return "Внутренняя ошибка устройства. Проверьте соответствующую информацию об устройстве или драйвере.";
		case MErrorCode::OutOfMemory:
			return "Не удалось выделить память, или памяти недостаточно.";
		case MErrorCode::IndexOutOfRange:
			return "Индекс находится за пределами указанного массива или заданного диапазона.";
		case MErrorCode::TheThreadIsBusy:
			return "В настоящее время тема занята, пожалуйста, подождите или зайдите позже.";
		case MErrorCode::BufferTooSmall:
			return "Буфер слишком мал, рассмотрите возможность его увеличения.";
		case MErrorCode::AccessDenied:
			return "Доступ запрещен.";
		case MErrorCode::IOError:
			return "Ошибка ввода-вывода или ошибка файловой системы.";
		case MErrorCode::OutdatedInterface:
			return "Устаревшие интерфейсы использовать не рекомендуется, но если вы хотите их использовать, добавьте соответствующие флаги компиляции.";
		case MErrorCode::RenderInternalError:
			return "Внутри рендерера произошла аномалия. Пожалуйста, проверьте реализацию интерфейса рендеринга.";
		case MErrorCode::STDError:
			return "Исключение стандартной библиотеки C++, используйте What() для получения информации.";
		case MErrorCode::WinCOMError:
			return "Код ошибки Windows COM HRESULT, проверьте extCode.";
		case MErrorCode::MUIError:
			return "Другие ошибки MUI. Используйте WhatW() для получения подробной информации.";
		}
		return "Неизвестная ошибка";
	}
	std::wstring MError::toMessageW(MErrorCode errCode)
	{
		std::wstring ret;
		switch (errCode)
		{
		case MErrorCode::OK:
			ret = L"成功。";
			break;
		case MErrorCode::False:
			ret = L"已失败。";
			break;
		case MErrorCode::InvalidParameter:
			ret = L"无效的参数，一个或多个参数无效。";
			break;
		case MErrorCode::NotSupported:
			ret = L"不支持当前的操作或者功能。";
			break;
		case MErrorCode::InvalidData:
			ret = L"无效的数据，或格式不正确。";
			break;
		case MErrorCode::DeviceLost:
			ret = L"设备已丢失或状态发生改变，请丢弃关联的设备资源然后重新创建。";
			break;
		case MErrorCode::InternalDeviceError:
			ret = L"设备内部错误，请检查相关的设备信息或者驱动程序。";
			break;
		case MErrorCode::OutOfMemory:
			ret = L"内存分配失败，或内存不足。";
			break;
		case MErrorCode::IndexOutOfRange:
			ret = L"索引超出指定的数组或规定范围。";
			break;
		case MErrorCode::TheThreadIsBusy:
			ret = L"线程当前正忙，请等待或稍后再访问。";
			break;
		case MErrorCode::BufferTooSmall:
			ret = L"缓冲区过小，请考虑增大。";
			break;
		case MErrorCode::AccessDenied:
			ret = L"权限不足。";
			break;
		case MErrorCode::IOError:
			ret = L"I/O 错误，或文件系统错误。";
			break;
		case MErrorCode::OutdatedInterface:
			ret = L"已过时的接口，不建议使用，若要使用 请添加相应的编译Flag.";
			break;
		case MErrorCode::RenderInternalError:
			ret = L"渲染器内部异常，请检查渲染器接口实现。";
			break;
		case MErrorCode::STDError:
			ret = L"C++标准库异常，使用what()获取信息。";
			break;
		case MErrorCode::WinCOMError:
			ret = L"Код ошибки Windows COM HRESULT, проверьте extCode.";
			break;
		case MErrorCode::MUIError:
			ret = L"При других ошибках MUI используйте WhatW() для получения подробной информации.";
			break;
		case MErrorCode::Unknown:
		default:
			ret = L"未知错误。";
			break;
		}
		ret += L"\n(" + Helper::M_StringToWString(toMessage(errCode)) + L")";
		return ret;
	}
	std::wstring MError::fullText() const
	{
		std::wstringstream stream;
		stream << std::hex << std::uppercase << std::setfill(L'0') << std::setw(2);
		if (m_errCode == MErrorCode::WinCOMError)
			stream << (_m_long)m_extCode;
		else
			stream << (_m_error)m_errCode;
		std::wstring ret = L" (0x" + stream.str() + L"): ";
		ret += whatW();
		if (m_errCode == MErrorCode::WinCOMError)
			ret = L"Код ошибки HRESUL COM" + ret;
		else
			ret = L"Код ошибки MiaoUI" + ret;
		return ret;
	}
}