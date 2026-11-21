
#pragma once
#include <Mui_Framework.h>
#include <Mui_Debug.h>
namespace Mui::RAII
{
	template <typename TLambda>
	class lambda_call
	{
	public:
		lambda_call(const lambda_call&) = delete;
		lambda_call& operator=(const lambda_call&) = delete;
		lambda_call& operator=(lambda_call&& other) = delete;
		explicit lambda_call(TLambda&& lambda) noexcept : m_lambda(std::move(lambda))
		{
			static_assert(std::is_same_v<decltype(lambda()), void>, "scope_exit的lambda函数一定不能有返回值");
			static_assert(!std::is_lvalue_reference_v<TLambda> && !std::is_rvalue_reference_v<TLambda>, "scope_exit只能直接与lambda函数一起使用");
		}
		lambda_call(lambda_call&& other) noexcept : m_lambda(std::move(other.m_lambda)), m_call(other.m_call)
		{
			other.m_call = false;
		}
		~lambda_call() noexcept
		{
			reset();
		}
		void release() noexcept
		{
			m_call = false;
		}
		void reset() noexcept
		{
			if (m_call)
			{
				m_call = false;
				m_lambda();
			}
		}
		[[nodiscard]] explicit operator bool() const noexcept
		{
			return m_call;
		}
	protected:
		TLambda m_lambda;
		bool m_call = true;
	};
	template <typename TLambda>
	[[nodiscard]] inline auto scope_exit(TLambda&& lambda) noexcept
	{
		return lambda_call<TLambda>(std::forward<TLambda>(lambda));
	}
	class MBasicObj
	{
	public:
		virtual ~MBasicObj() noexcept = default;
		MBasicObj(MBasicObj&&) = delete;
		MBasicObj(const MBasicObj&) = delete;
		MBasicObj& operator=(const MBasicObj&) = delete;
		MBasicObj& operator=(MBasicObj&&) = delete;
		void AddRef() noexcept { ++m_ref; }
		void Release()
		{
			if (m_ref > 0)
				--m_ref;
			if (m_ref != 0)
				return;
			try
			{
				ReleaseProc();
			}
			catch (...)
			{
#ifdef _DEBUG
				_M_OutErrorDbg_(L"对象释放发生异常", false);
#endif
			}
			delete this;
		}
	protected:
		MBasicObj() noexcept : m_ref(0) {}
		virtual void ReleaseProc() {}
	private:
		std::atomic_int m_ref;
	};
	template <typename T>
	class Mui_Ptr
	{
	public:
		Mui_Ptr() noexcept : m_ptr(nullptr) {}
		Mui_Ptr(T* other) noexcept : m_ptr(other)
		{
			if (m_ptr)
				m_ptr->AddRef();
		}
		Mui_Ptr(const Mui_Ptr& other) noexcept : m_ptr(other.m_ptr)
		{
			if (m_ptr)
				m_ptr->AddRef();
		}
		Mui_Ptr(Mui_Ptr&& other) noexcept : m_ptr(nullptr)
		{
			swap(other);
		}
		~Mui_Ptr() noexcept
		{
			if (m_ptr)
				m_ptr->Release();
		}
		Mui_Ptr& operator=(T* other) noexcept
		{
			if (m_ptr != other)
				Mui_Ptr(other).swap(*this);
			return *this;
		}
		Mui_Ptr& operator=(const Mui_Ptr& other) noexcept
		{
			if (this == &other)
				return *this;
			Mui_Ptr(other).swap(*this);
			return *this;
		}
		Mui_Ptr& operator=(Mui_Ptr&& other) noexcept
		{
			Mui_Ptr(std::move(other)).swap(*this);
			return *this;
		}
		bool operator!=(const Mui_Ptr& other) noexcept
		{
			return m_ptr != other.m_ptr;
		}
		bool operator!=(const T* other) noexcept
		{
			return m_ptr != other;
		}
		bool operator==(const Mui_Ptr& other) noexcept
		{
			return m_ptr == other.m_ptr;
		}
		bool operator==(const T* other) noexcept
		{
			return m_ptr == other;
		}
		[[nodiscard]] T* get() const noexcept
		{
			return m_ptr;
		}
		T** operator&() noexcept
		{
			return &m_ptr;
		}
		T* operator->() const noexcept
		{
			return m_ptr;
		}
		explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}
		void swap(Mui_Ptr& other) noexcept
		{
			std::swap(m_ptr, other.m_ptr);
		}
		template<typename CAST>
		Mui_Ptr<CAST> cast() noexcept
		{
			return reinterpret_cast<CAST*>(m_ptr);
		}
		void rest() noexcept
		{
			operator=(nullptr);
		}
		void detach() noexcept
		{
			m_ptr = nullptr;
		}
		using Type = T;
	private:
		T* m_ptr = nullptr;
	};
}