
#pragma once
#include <Mui_Base.h>
namespace Mui::FS
{
	namespace UI
	{
		struct DlgFilter
		{
			std::wstring description, extension;
		};
		extern bool MBrowseForFile(bool isOpen, bool multiple, const std::vector<DlgFilter>& filter,
			_m_param parentWndHandle, std::vector<std::wstring>& selectedFiles, std::wstring_view defExtName = {});
	}
	std::wstring MGetCurrentDir();
	UIResource MReadFile(std::wstring_view filepath, bool string = false);
	_m_size MWriteFile(std::wstring_view filepath, UIResource res);
	FILE* MOpenFile(std::wstring_view filepath, const char* mode);
	void MSeekFile(FILE* file, _m_long64 offset, int origin = SEEK_SET);
	std::wstring MGetFileName(std::wstring_view path);
	void MEnumDirectory(std::wstring_view path, std::vector<std::wstring>& dirlist, bool recursive = false);
	void MEnumFiles(std::wstring_view path, const std::vector<std::wstring>& filter,
		std::vector<std::wstring>& dirlist, bool recursive = false);
	bool MRemoveFile(std::wstring_view path);
	bool MRemoveDirectory(std::wstring_view path, bool removeAll = false);
	bool MFileExists(std::wstring_view path, bool isdir = false);
	bool MCreateDirectory(std::wstring_view path, bool mult);
}