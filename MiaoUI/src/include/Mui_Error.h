
#pragma once
#include <Mui_TypeDef.h>
#include <Mui_Framework.h>
namespace Mui
{
	using _m_error = _m_param;
	enum class MErrorCode : _m_error
	{
		OK,
		False,
		InvalidParameter,
		NotSupported,
		InvalidData,
		DeviceLost = 0x10001,
		InternalDeviceError,
		OutOfMemory,
		IndexOutOfRange,
		TheThreadIsBusy,
		BufferTooSmall,
		AccessDenied = 0x20001,
		IOError,
		OutdatedInterface,
		RenderInternalError = 0x30001,
		STDError = 0xA00001,
		WinCOMError,
		MUIError,
		Unknown = 0xFFFFFF
	};
	class MError final : std::exception
	{
	public:
		explicit MError(MErrorCode err, _m_error ext = 0) noexcept;
		explicit MError(const char* what) noexcept;
		MError(_m_error hresult, std::wstring_view err);
		MError(std::wstring_view info);
		[[nodiscard]] const char* what() const noexcept override;
		[[nodiscard]] std::wstring whatW() const;
		[[nodiscard]] MErrorCode errCode() const noexcept;
		[[nodiscard]] _m_error extCode() const noexcept;
		static const char* toMessage(MErrorCode errCode);
		static std::wstring toMessageW(MErrorCode errCode);
		[[nodiscard]] std::wstring fullText() const;
	private:
		MErrorCode m_errCode = MErrorCode::Unknown;
		_m_error m_extCode = 0;
		std::wstring m_extInfo;
	};
}