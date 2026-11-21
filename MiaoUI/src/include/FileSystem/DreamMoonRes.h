
#pragma once
#include <Mui_Base.h>
#if MUI_CFG_ENABLE_V1DMRES
#define DMResVer L"v1.0.1";
namespace Mui {
	struct DMResKey
	{
		UIResource res;
		bool block = false;
		unsigned long blockSize = 0;
		std::wstring resText;
	};
	enum ResType
	{
		AllRes,
		DataRes,
		StringRes
	};
	struct DMEncBlockCallback
	{
		std::function<void(_m_size encsize, _m_size allsize, _m_param param)> callback = nullptr;
		_m_param param = 0;
	};
	class DMResources
	{
	public:
		~DMResources();
		bool LoadResource(std::wstring resfiles, bool mem = false);
#ifdef _WIN32
		static UIResource ReadPEResource(DWORD resID, LPCWSTR type);
		bool LoadResource(_m_ulong resid, _m_lpcwstr restype);
#endif
		bool LoadResource(UIResource memres);
		_m_size GetResourceCount();
		DMResKey ReadResource(std::wstring resname, std::wstring reskey, ResType type = AllRes);
		UIResource ReadResourceBlock(std::wstring resname, std::wstring reskey, _m_uint offset, _m_size len, _m_size* retlen);
		_m_size ReadResourceSize(std::wstring resname, ResType type);
		bool ReadResBlockInfo(std::wstring resname, _m_size* blocksize);
		bool EnumResourceName(std::vector<std::wstring>& nameList);
		_m_lpcwstr GetResClassName();
		bool CreateResource(std::wstring classname);
		bool AddResource(DMResKey res, std::wstring resname, std::wstring reskey, DMEncBlockCallback callback = DMEncBlockCallback());
		bool RenameResource(std::wstring resname, std::wstring newname);
		void RenameClassName(std::wstring newname);
		bool ChangeResource(std::wstring resname, DMResKey newres, std::wstring reskey,
			ResType type = AllRes, DMEncBlockCallback callback = DMEncBlockCallback());
		bool DeleteResource(std::wstring resname);
		bool SaveResource(std::wstring filename);
		bool CloseResource();
		UIResource ReadFiles(std::wstring filepath, bool string = false);
		bool WriteFiles(std::wstring filepath, UIResource res);
	private:
		_m_byte* HexStringToBytes(std::string str);
		std::string Sha256StringHex(std::wstring str);
        std::wstring wchar16Towchar32(char16_t * wchar16, _m_uint len);
        char16_t * wchar32Towchar16(wchar_t  * wchar32, _m_uint len);
		UIResource Enciphering(UIResource res, std::wstring key);
		UIResource EncipheringText(std::wstring text, std::wstring key);
		UIResource Deciphering(UIResource res, std::wstring key);
		std::wstring DecipheringText(UIResource res, std::wstring key);
		void EncBlockProc(UIResource res, std::wstring resname, std::wstring key, _m_uint blocksize, UIResource* ret, DMEncBlockCallback callback);
		UIResource EncBlockResource(UIResource res, std::wstring resname, std::wstring key, _m_uint blocksize, DMEncBlockCallback callback);
		bool LoadResList(bool file = false);
		struct DMResHeader
		{
			std::wstring classname;
			std::wstring ressign;
			std::wstring resver;
			_m_uint rescount = 0;
		};
		struct DMResItem
		{
			std::wstring resname;
			UIResource res;
			bool block = false;
			_m_uint blockSize = 0;
			UIResource text;
			std::pair<_m_ulong, _m_uint> resPos;
			std::pair<_m_ulong, _m_uint> strPos;
		};
		std::vector<DMResItem> dmResList;
		DMResHeader m_resheader;
		UIResource m_resource;
		FILE* m_files = nullptr;
		std::mutex mx;
	};
}
#endif