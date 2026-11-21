
#pragma once
#include <Mui_Base.h>
#include <Mui_Helper.h>
#include <Render/Node/Mui_UINodeBase.h>
#include <any>
#include <utility>
#include <array>
#define MCTRL_ATTRIB _mctrl_attrib_attrib
#define MCTRL_BASE_ATTRIB(base) base::MCTRL_ATTRIB
#define MCTRL_DEFNAME(x) static constexpr auto ClassName = x; \
virtual std::wstring GetClsName() const override { return ClassName; } \
static void Register()
#define MCTRL_MAKE_ATTRIB(...) Helper::ToArray<CtrlMgr::AttribType>({__VA_ARGS__})
#define MCTRL_DEFINE(x,...) MCTRL_DEFNAME(x); \
inline static constexpr std::array MCTRL_ATTRIB = Helper::MergeArrays(MCTRL_MAKE_ATTRIB(__VA_ARGS__))
#define MCTRL_DEFINE_EX(x,attrib,...) MCTRL_DEFNAME(x); \
inline static constexpr std::array MCTRL_ATTRIB = Helper::MergeArrays(attrib, __VA_ARGS__)
#define mslot mctrl_eventslot
#define MCTRL_DEFSLOT(...) public: struct { __VA_ARGS__ } mslot
#define MCTRL_REGISTER(method) CtrlMgr::RegisterControl(ClassName, method, Helper::ArrayToVector(MCTRL_ATTRIB))
namespace Mui::Ctrl
{
	template<typename T>
	constexpr bool mis_uinodecls = std::is_base_of_v<Render::UINodeBase, T>
		|| std::is_base_of_v<T, Render::UINodeBase>;
	template<typename Target, typename T>
	Target* mcast_control(T* control)
	{
		static_assert(mis_uinodecls<T>,
			"[MiaoUI cast]: The target<control param>type is not a base or derived class of UINodeBase.");
		static_assert(mis_uinodecls<Target>,
			"[MiaoUI cast]: The target<Target param>type is not a base or derived class of UINodeBase.");
		constexpr bool isBase = std::is_base_of_v<Target, T>;
		constexpr bool isDerived = std::is_base_of_v<T, Target>;
		static_assert(isBase || isDerived,
			"[MiaoUI cast]: There is no inheritance or derivation relationship between the parameters and the target to be converted.");
#ifdef _DEBUG
		Target* ptr = dynamic_cast<Target*>(control);
		if (!ptr)
			throw std::exception("[Приведение MiaoUI]: невозможно преобразовать в целевой тип.");
#endif
		return static_cast<Target*>(control);
	}
}
namespace Mui::CtrlMgr
{
	namespace Attrib
	{
		template<typename T>
		std::wstring Value_Make2x(T a, T b)
		{
			return std::to_wstring(a) + L"," + std::to_wstring(b);
		}
		template<typename T>
		std::wstring Value_Make4x(T a, T b, T c, T d)
		{
			return std::to_wstring(a) + L"," + std::to_wstring(b) + L","
				+ std::to_wstring(c) + L"," + std::to_wstring(d);
		}
		inline std::wstring Value_Make2x(UIPoint pt)
		{
			return Value_Make2x(pt.x, pt.y);
		}
		inline std::wstring Value_Make2x(UISize size)
		{
			return Value_Make2x(size.width, size.height);
		}
		inline std::wstring Value_Make4x(UIRect rect)
		{
			return Value_Make4x(rect.left, rect.top, rect.right, rect.bottom);
		}
		template<typename T>
		std::wstring Value_Make4x(_m_rect_t<T> rect)
		{
			return Value_Make4x<T>(rect.left, rect.top, rect.right, rect.bottom);
		}
	}
	typedef std::function<Ctrl::UIControl*(Ctrl::UIControl*)> RegMethod;
	struct AttribType
	{
		enum type
		{
			defaults,
			labelStyle,
			listfontStyle,
			UIStyle,
			UIResource,
			UIBitmap
		} attribType = defaults;
		std::wstring_view attribName;
	};
	class AttribConverter
	{
	public:
		AttribConverter();
		virtual ~AttribConverter() = default;
		template<typename T>
		T Convert(std::wstring_view value)
		try
		{
			auto iter = m_methodList.find(typeid(T).name());
			if (iter == m_methodList.end())
				return {};
			return std::any_cast<T>(iter->second.get(value));
		}
		catch(...)
		{
#ifdef _DEBUG
			_M_OutErrorDbg_(L"不支持的类型转换", false);
#endif
			return {};
		}
		template<typename T>
		std::wstring Convert(const T& value)
		try
		{
			auto iter = m_methodList.find(typeid(T).name());
			if (iter == m_methodList.end())
				return {};
			return iter->second.set(value);
		}
		catch(...)
		{
#ifdef _DEBUG
			_M_OutErrorDbg_(L"不支持的类型转换", false);
#endif
			return {};
		}
		using ConversionMethod_Get = std::function<std::any(std::wstring_view value)>;
		using ConversionMethod_Set = std::function<std::wstring(std::any value)>;
	protected:
		template<typename T>
		void AddMethod(ConversionMethod_Get get, ConversionMethod_Set set)
		{
			m_methodList.insert(std::make_pair((std::string)typeid(T).name(), data{ std::move(get), std::move(set)}));
		}
	private:
		struct data
		{
			ConversionMethod_Get get = nullptr;
			ConversionMethod_Set set = nullptr;
		};
		static std::unordered_map<std::string, data> m_methodList;
	};
#define MakeUIAttrib(__struct, ...) decltype(__struct)::MakeAttrib(__VA_ARGS__)
#define MakeUIAttribEx(__struct, member, offset, ...) decltype(__struct)::MakeAttribAtOffset(member, decltype(__struct)::MOffsetOf(offset), __VA_ARGS__)
#define MakeUIAttrib_AfterSetOnly(__struct, member, name, paramname, code) decltype(__struct)::MakeAttrib(member, name, \
	[](decltype(__struct)::SetData param) \
	{ \
		param.Assign(member); \
		return [](decltype(__struct)::SetData& paramname) -> bool \
		code(paramname); \
	})
#define MakeUIAttrib_AfterGetOnly(__struct, member, name) decltype(__struct)::MakeAttrib(member, name, nullptr, \
	[](decltype(__struct)::GetData param) \
	{ \
		code \
	} \
	template<typename T>
	T* ConvertAttribPtr(std::wstring_view attrib)
	{
		return reinterpret_cast<T*>(Helper::M_StoULong64(attrib));
	}
	template<typename T>
	std::wstring ConvertAttribPtr(T* attrib)
	{
		return std::to_wstring((_m_ptrv)attrib);
	}
	template<typename T, typename Param = bool>
	class UIAttribute
	{
	public:
		struct SetData
		{
			T* data = nullptr;
			AttribConverter* converter = nullptr;
			std::wstring_view attribName;
			std::wstring_view attribValue;
			Param param;
			template<typename Type = std::wstring_view>
			inline Type GetValue()
			{
				if constexpr (std::is_pointer_v<Type>)
					return (Type)ConvertAttribPtr<Type>(attribValue);
				else if constexpr (std::is_same_v<Type, std::wstring>)
					return attribValue.data();
				else if constexpr (std::is_same_v<Type, std::wstring_view> || std::is_same_v<Type, UIString>)
					return attribValue;
				else if constexpr (std::is_enum_v<Type>)
					return (Type)converter->Convert<int>(attribValue);
				else
					return converter->Convert<Type>(attribValue);
			}
			template<typename Struct, typename Member>
			void Assign(Member Struct::* member)
			{
				auto pData = (Member*)(_m_ptrv(data) + MOffsetOf<Struct>(member));
				*pData = GetValue<Member>();
			}
			template<typename Struct, typename Member>
			void AssignAtOffset(Member Struct::* member, _m_ptrv offset)
			{
				auto pData = (Member*)(_m_ptrv(data) + MOffsetOf<Struct>(member) + offset);
				*pData = GetValue<Member>();
			}
		};
		struct GetData
		{
			T* data = nullptr;
			AttribConverter* converter = nullptr;
			std::wstring_view attribName;
			Param param;
		};
		using SetAttribCallback = std::function<bool(SetData set)>;
		using GetAttribCallback = std::function<void(GetData get)>;
		struct attribCallback
		{
			SetAttribCallback set = nullptr;
			std::function<std::wstring(GetData get)> get = nullptr;
		};
		UIAttribute(T attrib, const std::shared_ptr<AttribConverter>& converter = std::make_shared<AttribConverter>())
		: m_attribData(std::move(attrib)), m_converter(converter) {}
		template<typename Type = std::wstring_view>
		bool SetAttribute(std::wstring_view attribName, const Type& attrib, Param param = {})
		{
			auto iter = m_attribCallbackList.find(attribName.data());
			if (iter == m_attribCallbackList.end() || !iter->second.set)
				return false;
			bool ret;
			if constexpr (std::is_same_v<Type, std::wstring_view>)
				ret = iter->second.set({ &m_attribData, m_converter.get(), attribName, attrib, std::move(param) });
			else
			{
				std::wstring value = m_converter->Convert(attrib);
				ret = iter->second.set({ &m_attribData, m_converter.get(), attribName, value, std::move(param) });
			}
			return ret;
		}
		template <typename Type = std::wstring>
		bool GetAttribute(std::wstring_view attribName, Type& dstValue, Param param = {})
		{
			auto iter = m_attribCallbackList.find(attribName.data());
			if (iter == m_attribCallbackList.end() || !iter->second.get)
				return false;
			auto ret = iter->second.get({ &m_attribData, m_converter.get(), attribName, std::move(param) });
			if constexpr (std::is_same_v<Type, std::wstring>)
				dstValue = ret;
			dstValue = m_converter->Convert<Type>(ret);
			return true;
		}
		static void RegisterAttrib(const std::vector<std::pair<std::wstring, attribCallback>>& attribList)
		{
			for (auto& attrib : attribList)
				m_attribCallbackList.insert(std::make_pair(attrib.first, attrib.second));
		}
		template<typename... Args>
		static void RegisterAttrib(Args&&... args)
		{
			(void)std::initializer_list<int>{ (m_attribCallbackList.insert(std::forward<Args>(args)), 0) ... };
		}
		template<typename Args>
		static void RegisterAttrib(std::initializer_list<Args>& args)
		{
			for (const auto& arg : args)
			{
				m_attribCallbackList.insert(arg);
			}
		}
		template<typename Struct, typename Member>
		static auto MakeAttrib(Member Struct::* member, std::wstring_view name, SetAttribCallback set = nullptr, GetAttribCallback get = nullptr)
		{
			auto _set = [_callback = set, member](SetData data) -> bool
			{
				if (_callback)
					return _callback(std::move(data));
				data.template Assign<Struct, Member>(member);
				return true;
			};
			auto _get = [_callback = get, member](GetData data) -> std::wstring
			{
				auto pData = (Member*)(_m_ptrv(data.data) + MOffsetOf<Struct>(member));
				if (_callback)
					_callback(data);
				if constexpr (std::is_pointer_v<Member>)
					return data.converter->Convert((void*)*pData);
				else if constexpr (std::is_enum_v<Member>)
					return data.converter->Convert((int)*pData);
				else
					return data.converter->Convert(*pData);
			};
			return std::make_pair(std::wstring{ name }, attribCallback{ std::move(_set), std::move(_get) });
		}
		template<typename Struct, typename Member>
		static auto MakeAttribAtOffset(Member Struct::* member, _m_ptrv offset, std::wstring_view name, SetAttribCallback set = nullptr, GetAttribCallback get = nullptr)
		{
			auto _set = [_callback = set, member, offset](SetData data) -> bool
			{
				if (_callback)
					return _callback(std::move(data));
				data.template AssignAtOffset<Struct, Member>(member, offset);
				return true;
			};
			auto _get = [_callback = get, member, offset](GetData data) -> std::wstring
			{
				auto pData = (Member*)(_m_ptrv(data.data) + MOffsetOf<Struct>(member) + offset);
				if (_callback)
					_callback(data);
				if constexpr (std::is_pointer_v<Member>)
					return data.converter->Convert((void*)*pData);
				else if constexpr (std::is_enum_v<Member>)
					return data.converter->Convert((int)*pData);
				else
					return data.converter->Convert(*pData);
			};
			return std::make_pair(std::wstring{ name }, attribCallback{ std::move(_set), std::move(_get) });
		}
		template<typename Struct, typename Member>
		static constexpr _m_ptrv MOffsetOf(Member Struct::* member)
		{
			return _m_ptrv(&((Struct*)nullptr->*member));
		}
		[[nodiscard]] const T& Get() const { return m_attribData; }
		T& Set() { return m_attribData; }
		T* GetPtr() { return &m_attribData; }
		void Set(const T& data) { m_attribData = data; }
		void Set(T&& data) { m_attribData = std::move(data); }
		using DataType = T;
	private:
		DataType m_attribData;
		std::shared_ptr<AttribConverter> m_converter;
		static std::unordered_map<std::wstring, attribCallback> m_attribCallbackList;
	};
	template<typename T, typename Param>
	std::unordered_map<std::wstring, typename UIAttribute<T, Param>::attribCallback> UIAttribute<T, Param>::m_attribCallbackList;
	template<class... T>
	class EventSlot
	{
	public:
		using ProcType = std::function<void(T...)>;
		void Bind(ProcType slot)
		{
			m_slotList.emplace_back(std::move(slot));
		}
		template<typename... Args>
		void Emit(Args... args)
		{
			for (auto& slot : m_slotList)
				slot(std::forward<Args>(args)...);
		}
	private:
		std::vector<ProcType> m_slotList;
	};
	bool RegisterControl(std::wstring_view name, RegMethod&& method, const std::vector<AttribType>& ptrAttrib = {});
	void RegisterMuiControl();
	Ctrl::UIControl* CreateControl(std::wstring_view name, Ctrl::UIControl* parent);
	AttribType::type GetAttributeType(std::wstring_view name, std::wstring_view attribName);
}