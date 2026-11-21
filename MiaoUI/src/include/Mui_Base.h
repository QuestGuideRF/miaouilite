
#pragma once
#include <Mui_Config.h>
#include <Mui_Error.h>
#include <Mui_Debug.h>
#include <Mui_RAIIHelper.h>
namespace Mui
{
	using namespace std::chrono;
	using MTimerCallback = std::function<void(_m_ptrv, _m_ptrv, _m_ulong)>;
	struct _m_scale
	{
		float cx = 0.f;
		float cy = 0.f;
	};
	struct _m_rcscale
	{
		float xs = 0.f;
		float ys = 0.f;
		float ws = 0.f;
		float hs = 0.f;
		[[nodiscard]] _m_scale scale() const
		{
			return { ws, hs };
		}
	};
	template<typename T>
	T _scale_to(T src, float s)
	{
		if constexpr(std::is_floating_point_v<T>)
			return src * s;
		else
			return (T)round(s * (float)src);
	}
	union _m_color
	{
		struct
		{
			_m_byte r;
			_m_byte g;
			_m_byte b;
			_m_byte a;
		};
		_m_uint argb;
		constexpr _m_color(_m_uint color) : argb(color) {}
		_m_color() = default;
		operator _m_uint() const { return argb; }
		_m_color(std::wstring_view hex, bool argb = false, bool alpha = true);
		std::wstring HEX(bool argb, bool alpha = true);
		static _m_byte AlphaBlend(_m_byte dst, _m_byte src);
	};
	using UIColor = _m_color;
	template<typename T>
	struct _m_rect_t
	{
		T left;
		T top;
		T right;
		T bottom;
		_m_rect_t(T x, T y, T x2, T y2)
		{
			left = x;
			top = y;
			right = x2;
			bottom = y2;
		}
		_m_rect_t(T xywh)
		{
			left = top = right = bottom = xywh;
		}
		_m_rect_t()
		{
			left = top = right = bottom = {};
		}
		[[nodiscard]] _m_rect_t scale(float x, float y) const
		{
			return
			{
				_scale_to(left, x),
				_scale_to(top, y),
				_scale_to(right, x),
				_scale_to(bottom, y)
			};
		}
		[[nodiscard]] _m_rect_t scale(_m_scale _scale) const
		{
			return scale(_scale.cx, _scale.cy);
		}
		template<typename Target>
		[[nodiscard]] _m_rect_t<Target> ToRectT() const
		{
			return { (Target)left, (Target)top, (Target)right, (Target)bottom };
		}
		template<typename Target>
		[[nodiscard]] _m_rect_t<Target> ToRectTAsRound() const
		{
			return { (Target)round(left), (Target)round(top), (Target)round(right), (Target)round(bottom) };
		}
		[[nodiscard]] T GetWidth() const
		{
			return right - left;
		}
		[[nodiscard]] T GetWidthAsFloor() const
		{
			return floor(right) - floor(left);
		}
		[[nodiscard]] T GetHeight() const
		{
			return bottom - top;
		}
		[[nodiscard]] T GetHeightAsFloor() const
		{
			return floor(bottom) - floor(top);
		}
		bool IsEmpty()
		{
			return left == 0 && top == 0 && right == 0 && bottom == 0;
		}
	};
	using _m_rect = _m_rect_t<int>;
	using _m_rectf = _m_rect_t<float>;
	template<typename T>
	struct _m_point_t
	{
		T x = 0;
		T y = 0;
		[[nodiscard]] _m_point_t scale(float _x, float _y) const
		{
			return { _scale_to(x, _x), _scale_to(y, _y) };
		}
		[[nodiscard]] _m_point_t scale(_m_scale _scale) const
		{
			return scale(_scale.cx, _scale.cy);
		}
		bool operator==(const _m_point_t& pt) const
		{
			if constexpr(std::is_floating_point_v<T>)
				return std::abs(x - pt.x) < std::numeric_limits<T>::epsilon() && std::abs(y - pt.y) < std::numeric_limits<T>::epsilon();
			else
				return x == pt.x && y == pt.y;
		}
		bool operator!=(const _m_point_t& pt) const
		{
			return !operator==(pt);
		}
	};
	using _m_pointf = _m_point_t<float>;
	template<typename T>
	struct _m_size_t
	{
		T width = 0;
		T height = 0;
		[[nodiscard]] _m_size_t scale(float _ws, float _hs) const
		{
			return { _scale_to(width, _ws), _scale_to(height, _hs) };
		}
		[[nodiscard]] _m_size_t scale(_m_scale _scale) const
		{
			return scale(_scale.cx, _scale.cy);
		}
		bool operator==(const _m_size_t& pt) const
		{
			if constexpr (std::is_floating_point_v<T>)
				return std::abs(width - pt.width) < std::numeric_limits<T>::epsilon() && std::abs(height - pt.height) < std::numeric_limits<T>::epsilon();
			else
				return width == pt.width && height == pt.height;
		}
		bool operator!=(const _m_size_t& pt) const
		{
			return !operator==(pt);
		}
	};
	using _m_sizef = _m_size_t<float>;
	class UIRect : public _m_rect_t<int>
	{
	public:
		UIRect();
		UIRect(const _m_rect& src);
		UIRect(int x, int y, int width, int height);
		[[nodiscard]] _m_rect ToRect() const;
		template <typename T>
		[[nodiscard]] _m_rect_t<T> ToRectT() const
		{
			return { (T)left, (T)top, (T)right, (T)bottom };
		}
		void SetWidth(int width);
		void SetHeight(int height);
		[[nodiscard]] bool IsEmpty() const;
		void Empty();
		void Join(const UIRect& rc);
		void ResetOffset();
		void Normalize();
		void Offset(int cx, int cy);
		void Inflate(int cx, int cy);
		void Deflate(int cx, int cy);
		void Union(const UIRect& rc);
		bool operator==(const UIRect& rc) const;
		bool operator!=(const UIRect& rc) const;
	private:
		int offsetx = 0;
		int offsety = 0;
	};
	using UISize = _m_size_t<int>;
	using UIPoint = _m_point_t<int>;
	using MPCRect = const _m_rect*;
	using MPCSize = const UISize*;
	using MPCPoint = const UIPoint*;
	struct UIResource
	{
		_m_byte* data = nullptr;
		_m_size size = 0;
		UIResource() = default;
		UIResource(_m_byte* p, _m_size s) { data = p; size = s; }
		void Release() { delete[] data; size = 0; data = nullptr; }
		explicit operator bool() const noexcept
		{
			return data && size;
		}
		[[nodiscard]] UIResource Copy() const
		{
			if (!operator bool()) return {};
			const UIResource ret(new _m_byte[size], size);
			memcpy(ret.data, data, size);
			return ret;
		}
	};
	struct UIFontStyle
	{
		bool bold = false;
		bool italics = false;
		bool underline = false;
		bool strikeout = false;
		bool operator== (const UIFontStyle& font) const;
		bool operator!= (const UIFontStyle& font) const;
	};
	class UIString
	{
	public:
		UIString();
		UIString(_m_lpcwstr pstr);
		UIString(std::wstring_view strview);
		UIString(const std::wstring& string);
		UIString(UIString&&) noexcept;
		UIString(const UIString&);
		UIString& operator=(const UIString&);
		UIString& operator=(UIString&&) noexcept;
		~UIString();
		bool operator== (_m_lpcwstr other) const;
		bool operator!= (_m_lpcwstr other) const;
		bool operator== (const UIString& other) const;
		bool operator!= (const UIString& other) const;
		bool operator== (std::wstring_view other) const;
		bool operator!= (std::wstring_view other) const;
		[[nodiscard]] _m_size length() const { return m_length; }
		[[nodiscard]] bool empty() const { return m_length == 0; }
		[[nodiscard]] _m_lpcwstr cstr() const { return m_string.get(); }
		[[nodiscard]] std::wstring_view view() const { return m_string.get(); }
	private:
		void CopyForm(_m_lpcwstr pstr);
		std::unique_ptr<wchar_t> m_string = nullptr;
		_m_size m_length = 0;
	};
	namespace Ctrl { class UIControl; }
	struct UIFocus
	{
		Ctrl::UIControl* curFocus = nullptr;
	};
	template<class T>
	class MThreadT
	{
	public:
		MThreadT(std::function<void()> threadproc)
		{
			m_stop = true;
			m_proc = std::move(threadproc);
		}
		virtual ~MThreadT() { Stop(); }
		void Start(bool pause = false)
		{
			if (!m_stop) return;
			m_pause = pause;
			m_stop = false;
			m_thread = new std::thread(&MThreadT::Thread, this);
		}
		void Pause(bool now = false)
		{
			std::unique_lock lock(m_mutex);
			if (!IsRuning()) return;
			m_pause.store(true, std::memory_order_relaxed);
			if (now && std::this_thread::get_id() == GetID())
			{
				Wait(lock);
			}
		}
		void Pause(std::unique_lock<T>& lock)
		{
			if (!IsRuning()) return;
			m_pause.store(true, std::memory_order_relaxed);
			if (std::this_thread::get_id() == GetID())
				Wait(lock);
		}
		void Resume(bool wait = false)
		{
			auto lock = GetLock();
			if (!IsPause() || !IsRuning()) return;
			m_pause = false;
			if (wait)
			{
				std::promise<void> notify;
				auto waitnotify = notify.get_future();
				{
					m_waitList.emplace_back(&notify);
					lock.unlock();
				}
				m_condition.notify_all();
				waitnotify.wait();
			}
			else
				m_condition.notify_all();
		}
		void Stop()
		{
			if (!m_thread) return;
			auto lock = GetLock();
			m_stop = true;
			m_pause = false;
			m_condition.notify_all();
			lock.unlock();
			m_thread->join();
			delete m_thread;
			m_thread = nullptr;
		}
		std::thread::id GetID() const
		{
			if (m_thread)
				return m_thread->get_id();
			return {};
		}
		bool IsPause() const
		{
			return m_pause;
		}
		bool IsRuning() const
		{
			return !m_stop;
		}
		auto GetLock()
		{
			return std::unique_lock<T>(m_mutex);
		}
	private:
		void Thread()
		{
			while (IsRuning())
			{
				{
					std::unique_lock lock(m_mutex);
					Wait(lock);
				}
				m_proc();
			}
		}
		void Wait(std::unique_lock<T>& lock)
		{
			while(IsPause())
				m_condition.wait(lock);
			for (auto& w : m_waitList)
				w->set_value();
			m_waitList.clear();
		}
		std::function<void()> m_proc = nullptr;
		std::thread* m_thread = nullptr;
		std::condition_variable_any m_condition;
		T m_mutex;
		std::atomic_bool m_pause, m_stop;
		std::vector<std::promise<void>*> m_waitList;
	};
	using MThread = MThreadT<std::mutex>;
	class MTimers : MThread
	{
	public:
		MTimers();
		~MTimers() override;
		using CallBack = std::function<void(_m_ptrv id, _m_ulong time)>;
		using ID = _m_ptrv;
		ID AddTimer(_m_uint elapse, CallBack callback);
		bool DelTimer(ID id);
	private:
		struct timer
		{
			_m_ptrv tid = 0;
			_m_ulong time = 0;
			_m_uint elapse = 0;
			CallBack callback;
		};
		using timerID = std::pair<_m_param, ID>;
		std::map<timerID, timer> m_timerList;
		std::unordered_map<ID, timerID> m_idList;
		std::mutex m_lock;
		_m_param GetMilliSeconds();
		_m_param GetSleepTime();
		void ThreadProc();
	};
	class MFPSCounter
	{
	public:
		_m_uint CalcFPS();
		void SetMaxFPS(float fps);
		float GetMaxFPS();
		void LimitFPS();
	private:
		float fps = 0.0f;
		float fpstime = -1.f;
		_m_uint frameCount = 0;
		steady_clock::time_point lastTime = steady_clock::now();
		steady_clock::time_point curTime = steady_clock::now();
		steady_clock::time_point m_BeginFrame = steady_clock::now();
		steady_clock::time_point m_EndFrame = steady_clock::now();
		_m_uint frame_count_per_second = 0;
		time_point<steady_clock, seconds>
			prev_time_in_seconds = time_point_cast<seconds>(m_BeginFrame);
		steady_clock::duration m_fpsLimit = {};
	};
	template <typename T>
	class MQueue
	{
		std::deque<T> m_queue;
		std::mutex m_mutex;
	public:
		MQueue() = default;
		bool empty()
		{
			std::unique_lock lock(m_mutex);
			return m_queue.empty();
		}
		auto size()
		{
			std::unique_lock lock(m_mutex);
			return m_queue.size();
		}
		void insertquque(T& t)
		{
			std::unique_lock lock(m_mutex);
			m_queue.push_front(t);
		}
		void enqueue(T& t)
		{
			std::unique_lock lock(m_mutex);
			m_queue.push_back(t);
		}
		bool dequeue(T& t)
		{
			std::unique_lock lock(m_mutex);
			if (m_queue.empty())
				return false;
			t = std::move(m_queue.front());
			m_queue.pop_front();
			return true;
		}
		void clear()
		{
			std::unique_lock lock(m_mutex);
			m_queue.clear();
		}
	};
}