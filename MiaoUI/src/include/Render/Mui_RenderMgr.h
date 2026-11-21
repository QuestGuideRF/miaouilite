
#pragma once
#include <Mui_Base.h>
#include <Render/Graphs/Mui_RenderDef.h>
#include <functional>
#include <future>
namespace Mui::Render
{
	using namespace Def;
	typedef std::function<void()> MRenderProc;
	template<class T>
	class MRingQueue
	{
	public:
		MRingQueue(_m_int max)
		{
			m_maxSize = max;
			m_data = new T[max];
			m_front = 0;
			m_rear = 0;
		}
		~MRingQueue()
		{
			if (m_data)
				delete[] m_data;
		}
		bool isFull()
		{
			return m_front == (m_rear + 1) % m_maxSize;
		}
		bool isEmpty()
		{
			return m_front == m_rear;
		}
		_m_uint size()
		{
			if (m_rear >= m_front)
				return m_rear - m_front;
			return m_maxSize - (m_front - m_rear);
		}
		_m_uint maxSize()
		{
			return m_maxSize;
		}
		bool push(const T& p)
		{
			if (!isFull())
			{
				m_data[m_rear] = p;
				m_rear = (m_rear + 1) % m_maxSize;
				return true;
			}
			return false;
		}
		bool push(T&& p)
		{
			if (!isFull())
			{
				m_data[m_rear] = std::move(p);
				m_rear = (m_rear + 1) % m_maxSize;
				return true;
			}
			return false;
		}
		bool pop(T& p)
		{
			if (!isEmpty())
			{
				p = m_data[m_front];
				m_front = (m_front + 1) % m_maxSize;
				return true;
			}
			return false;
		}
	private:
		T* m_data = nullptr;
		_m_int m_maxSize = 0;
		std::atomic_int m_front;
		std::atomic_int m_rear;
	};
	class MRenderCmd
	{
	public:
		MRenderCmd(MRender* base);
		~MRenderCmd();
		_m_lpcwstr GetRenderName();
		bool InitRender(_m_uint width, _m_uint height);
		bool Resize(_m_uint width, _m_uint height);
		MCanvasPtr CreateCanvas(_m_uint width, _m_uint height, _m_param param = 0U);
		MBitmapPtr CreateBitmap(std::wstring_view path, _m_param param = 0U);
		MBitmapPtr CreateBitmap(UIResource resource, _m_param param = 0U);
		MBitmapPtr CreateBitmap(_m_uint width, _m_uint height, void* bit, _m_uint len, _m_uint stride);
		MBitmapPtr CreateSVGBitmap(std::wstring_view path, _m_uint width, _m_uint height, bool repColor = false, _m_color color = 0);
		MBitmapPtr CreateSVGBitmapFromXML(std::wstring_view xml, _m_uint width, _m_uint height, bool repColor = false, _m_color color = 0);
		MPenPtr CreatePen(_m_uint width, _m_color color);
		MBrushPtr CreateBrush(_m_color color);
		MGradientBrushPtr CreateGradientBrush(const std::pair<_m_color, float>* vertex, _m_ushort count, UIPoint start, UIPoint end);
		MFontPtr CreateFonts(std::wstring_view text, std::wstring_view fontName, _m_uint fontSize, _m_ptrv fontCollection = 0);
		MEffectPtr CreateEffects(MEffects::Types effect, float value);
		MGeometryPtr CreateRoundGeometry(_m_rect dest, float round);
		MGeometryPtr CreateEllipseGeometry(_m_rect dest);
		MCanvasPtr CreateSubAtlasCanvas(_m_uint width, _m_uint height);
		MBatchBitmapPtr CreateBatchBitmap();
		bool CopyBitmapContent(MBitmap* dst, MBitmap* src, UIPoint dstPt, _m_rect srcRect);
		bool CopyBitmapContent(MCanvas* dst, MCanvas* src, UIPoint dstPt, _m_rect srcRect);
		void BeginDraw();
		void SetCanvas(MCanvasPtr canvas);
		void ResetCanvas();
		void DrawBitmap(MBitmapPtr img, _m_byte alpha = 255, _m_rect dest = { 0,0,0,0 }, _m_rect src = { 0,0,0,0 }, bool highQuality = true);
		void DrawBitmap(MCanvasPtr canvas, _m_byte alpha = 255, _m_rect dest = { 0,0,0,0 }, _m_rect src = { 0,0,0,0 }, bool highQuality = true);
		void DrawBatchBitmap(MBatchBitmapPtr bmp, MBitmapPtr input, bool highQuality = true);
		void DrawBatchBitmap(MBatchBitmapPtr bmp, MCanvasPtr input, bool highQuality = true);
		void DrawNinePalacesImg(MBitmapPtr img, _m_byte alpha, _m_rect dest, _m_rect src, _m_rect margin, bool highQuality = true);
		void DrawNinePalacesImg(MCanvasPtr canvas, _m_byte alpha, _m_rect dest, _m_rect src, _m_rect margin, bool highQuality = true);
		void DrawRectangle(_m_rect dest, MPenPtr pen);
		void DrawRoundedRect(_m_rect dest, float round, MPenPtr pen);
		void FillRectangle(_m_rect dest, MBrushPtr brush);
		void FillRectangle(_m_rect dest, MGradientBrushPtr brush);
		void FillRoundedRect(_m_rect dest, float round, MBrushPtr brush);
		void DrawTextLayout(MFontPtr font, _m_rect dest, MBrushPtr brush, TextAlign alignment);
		void DrawBitmapEffects(MBitmapPtr img, MEffectPtr effect, _m_byte alpha = 255, _m_rect dest = { 0,0,0,0 }, _m_rect src = { 0,0,0,0 });
		void DrawBitmapEffects(MCanvasPtr canvas, MEffectPtr effect, _m_byte alpha = 255, _m_rect dest = { 0,0,0,0 }, _m_rect src = { 0,0,0,0 });
		void DrawLine(UIPoint x, UIPoint y, MPenPtr pen);
		void DrawEllipse(_m_rect dest, MPenPtr pen);
		void FillEllipse(_m_rect dest, MBrushPtr brush);
		void PushClipRect(_m_rect rect);
		void PopClipRect();
		void PushClipGeometry(MGeometryPtr geometry);
		void PopClipGeometry();
		void Clear(_m_color color = 0);
		_m_result EndDraw();
		MCanvas* GetCanvas();
		MCanvas* GetRenderCanvas();
		void* Get();
		void Flush();
		MCanvas* GetSharedCanvas();
		bool SaveMBitmap(MBitmapPtr bitmap, std::wstring path, MImgFormat format);
		bool SaveMCanvas(MCanvasPtr canvas, std::wstring path, MImgFormat format);
		UIResource SaveMBitmap(MBitmapPtr bitmap, MImgFormat format);
		UIResource SaveMCanvas(MCanvasPtr canvas, MImgFormat format);
		bool CheckSubAtlasSource(MCanvasPtr canvas, MCanvasPtr canvas1);
		void RunTask(std::function<void()>&& task)
		{
			Task(std::move(task));
		}
		template<typename T>
		T* GetBase() const
		{
			return dynamic_cast<T*>(m_base);
		}
		bool IsTaskThread();
	private:
		void ThreadProc();
		void Task(std::function<void()>&& task);
		MRender* m_base = nullptr;
		std::mutex m_tasklock;
		std::condition_variable m_signal;
		std::atomic_bool m_stop;
		std::thread m_thread;
		struct taskParam
		{
			std::function<void()> task = nullptr;
			std::promise<void>& notification;
			taskParam(std::function<void()>& _task, std::promise<void>& n) :
				task(_task), notification(n) {}
		};
		std::vector<taskParam> m_taskList;
		std::atomic_bool m_begindraw;
		friend class MRenderObj;
	};
}