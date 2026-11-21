
#pragma once
#include <Mui_Base.h>
namespace Mui::Render
{
	class MRenderCmd;
	namespace Def
	{
		enum class MImgFormat
		{
			PNG,
			JPG,
			BMP
		};
		class MRenderObj : public RAII::MBasicObj
		{
		protected:
			void ReleaseProc() final;
			virtual void ReleaseThis() = 0;
			bool IsBaseThread();
			bool IsDrawing();
			void* GetBaseRender();
			template<typename T>
			T* GetBaseRenderCast()
			{
				return static_cast<T*>(GetBaseRender());
			}
			void* m_base = nullptr;
			friend class Mui::Render::MRenderCmd;
		};
		class MBitmap : public MRenderObj
		{
		public:
			virtual UISize GetSize() = 0;
			virtual int GetWidth() = 0;
			virtual int GetHeight() = 0;
		};
		using MBitmapPtr = RAII::Mui_Ptr<MBitmap>;
		class MCanvas : public MRenderObj
		{
		public:
			virtual UISize GetSize() = 0;
			virtual int GetWidth() = 0;
			virtual int GetHeight() = 0;
			virtual _m_param GetFlag() = 0;
			virtual _m_rect GetSubRect() = 0;
		};
		using MCanvasPtr = RAII::Mui_Ptr<MCanvas>;
		class MPen : public MRenderObj
		{
		public:
			virtual void SetColor(_m_color color) = 0;
			virtual void SetWidth(_m_uint width) = 0;
			virtual void SetOpacity(_m_byte alpha) = 0;
			virtual void SetWidthAndColor(_m_uint width, _m_color color) = 0;
			virtual _m_color GetColor() = 0;
			virtual _m_uint GetWidth() = 0;
			virtual _m_byte GetOpacity() = 0;
		};
		using MPenPtr = RAII::Mui_Ptr<MPen>;
		class MBrush : public MRenderObj
		{
		public:
			virtual void SetColor(_m_color color) = 0;
			virtual void SetOpacity(_m_byte alpha) = 0;
			virtual _m_color GetColor() = 0;
			virtual _m_byte GetOpacity() = 0;
		};
		using MBrushPtr = RAII::Mui_Ptr<MBrush>;
		class MGradientBrush : public MRenderObj
		{
		public:
			virtual _m_uint GetColorPosCount() = 0;
			virtual _m_color GetPosColor(_m_uint index) = 0;
			virtual void SetOpacity(_m_byte alpha) = 0;
			virtual _m_byte GetOpacity() = 0;
			virtual UIPoint GetStartPoint() = 0;
			virtual void SetStartPoint(UIPoint start) = 0;
			virtual UIPoint GetEndPoint() = 0;
			virtual void SetEndPoint(UIPoint end) = 0;
		};
		using MGradientBrushPtr = RAII::Mui_Ptr<MGradientBrush>;
		class MFont : public MRenderObj
		{
		public:
			virtual void SetFontName(std::wstring_view name) = 0;
			virtual void SetFontSize(_m_uint size, std::pair<_m_uint, _m_uint> range) = 0;
			virtual void SetFontStyle(UIFontStyle style, std::pair<_m_uint, _m_uint> range) = 0;
			virtual void SetFontColor(MBrush* brush, std::pair<_m_uint, _m_uint> range) = 0;
			virtual void SetText(std::wstring_view text) = 0;
			virtual UIRect GetMetrics() = 0;
			virtual const UIString& GetFontName() = 0;
			virtual _m_uint GetFontSize() = 0;
			virtual UIFontStyle GetFontStyle() = 0;
			virtual _m_color GetFontColor() = 0;
			virtual const UIString& GetText() = 0;
		};
		using MFontPtr = RAII::Mui_Ptr<MFont>;
		class MEffects : public MRenderObj
		{
		public:
			enum Types
			{
				GaussianBlur,
				BlackAndWhite,
				Sepia
			};
			virtual std::wstring GetEffectName() = 0;
			virtual Types GetEffectType() = 0;
			virtual void SetEffectValue(float param) = 0;
			virtual float GetEffectValue() = 0;
		};
		using MEffectPtr = RAII::Mui_Ptr<MEffects>;
		class MGeometry : public MRenderObj
		{
		public:
			enum MGeometryTypes
			{
				RoundRect,
				Ellipse
			};
			virtual MGeometryTypes GetGeometryType() = 0;
		};
		using MGeometryPtr = RAII::Mui_Ptr<MGeometry>;
		class MBatchBitmap : public MRenderObj
		{
		public:
			virtual void AddSub(_m_rect dst, _m_rect src, _m_byte alpha = 255) = 0;
			virtual bool DelSub(_m_uint index) = 0;
			virtual _m_uint GetCount() = 0;
			virtual void Clear() = 0;
		};
		using MBatchBitmapPtr = RAII::Mui_Ptr<MBatchBitmap>;
		class MRender : public MRenderObj
		{
		public:
			virtual _m_lpcwstr GetRenderName() = 0;
			virtual bool InitRender(_m_uint width, _m_uint height) = 0;
			virtual bool Resize(_m_uint width, _m_uint height) = 0;
			virtual MCanvas* CreateCanvas(_m_uint width, _m_uint height, _m_param param = 0U) = 0;
			virtual MBitmap* CreateBitmap(UIResource resource, _m_param param = 0U) = 0;
			virtual MBitmap* CreateBitmap(std::wstring_view path, _m_param param = 0U) = 0;
			virtual MBitmap* CreateBitmap(_m_uint width, _m_uint height, void* bit, _m_uint len, _m_uint stride) = 0;
			virtual MBitmap* CreateSVGBitmap(std::wstring_view path, _m_uint width, _m_uint height, bool repColor = false, _m_color color = 0) = 0;
			virtual MBitmap* CreateSVGBitmapFromXML(std::wstring_view xml, _m_uint width, _m_uint height, bool repColor = false, _m_color color = 0) = 0;
			virtual MPen* CreatePen(_m_uint width, _m_color color) = 0;
			virtual MBrush* CreateBrush(_m_color color) = 0;
			virtual MGradientBrush* CreateGradientBrush(const std::pair<_m_color, float>* vertex, _m_ushort count, UIPoint start, UIPoint end) = 0;
			virtual MFont* CreateFonts(std::wstring_view text, std::wstring_view fontName, _m_uint fontSize, _m_ptrv fontCollection = 0) = 0;
			virtual MEffects* CreateEffects(MEffects::Types effect, float value) = 0;
			virtual MGeometry* CreateRoundGeometry(_m_rect dest, float round) = 0;
			virtual MGeometry* CreateEllipseGeometry(_m_rect dest) = 0;
			virtual MCanvas* CreateSubAtlasCanvas(_m_uint width, _m_uint height) = 0;
			virtual MBatchBitmap* CreateBatchBitmap() = 0;
			virtual bool CopyBitmapContent(MBitmap* dst, MBitmap* src, UIPoint dstPt, _m_rect srcRect) = 0;
			virtual bool CopyBitmapContent(MCanvas* dst, MCanvas* src, UIPoint dstPt, _m_rect srcRect) = 0;
			virtual void BeginDraw() = 0;
			virtual void SetCanvas(MCanvas* canvas) = 0;
			virtual void ResetCanvas() = 0;
			virtual void DrawBitmap(MBitmap* img, _m_byte alpha = 255, _m_rect dest = { 0 }, _m_rect src = { 0 }, bool highQuality = true) = 0;
			virtual void DrawBitmap(MCanvas* canvas, _m_byte alpha = 255, _m_rect dest = { 0 }, _m_rect src = { 0 }, bool highQuality = true) = 0;
			virtual void DrawBatchBitmap(MBatchBitmap* bmp, MBitmap* input, bool highQuality = true) = 0;
			virtual void DrawBatchBitmap(MBatchBitmap* bmp, MCanvas* input, bool highQuality = true) = 0;
			virtual void DrawNinePalacesImg(MBitmap* img, _m_byte alpha, _m_rect dest, _m_rect src, _m_rect margin, bool highQuality = true) = 0;
			virtual void DrawNinePalacesImg(MCanvas* canvas, _m_byte alpha, _m_rect dest, _m_rect src, _m_rect margin, bool highQuality = true) = 0;
			virtual void DrawRectangle(_m_rect dest, MPen* pen) = 0;
			virtual void DrawRoundedRect(_m_rect dest, float round, MPen* pen) = 0;
			virtual void FillRectangle(_m_rect dest, MBrush* brush) = 0;
			virtual void FillRectangle(_m_rect dest, MGradientBrush* brush) = 0;
			virtual void FillRoundedRect(_m_rect dest, float round, MBrush* brush) = 0;
			virtual void DrawTextLayout(MFont* font, _m_rect dest, MBrush* brush, TextAlign alignment) = 0;
			virtual void DrawBitmapEffects(MBitmap* img, MEffects* effect, _m_byte alpha = 255, _m_rect dest = { 0 }, _m_rect src = { 0 }) = 0;
			virtual void DrawBitmapEffects(MCanvas* canvas, MEffects* effect, _m_byte alpha = 255, _m_rect dest = { 0 }, _m_rect src = { 0 }) = 0;
			virtual void DrawLine(UIPoint x, UIPoint y, MPen* pen) = 0;
			virtual void DrawEllipse(_m_rect dest, MPen* pen) = 0;
			virtual void FillEllipse(_m_rect dest, MBrush* brush) = 0;
			virtual void PushClipRect(_m_rect rect) = 0;
			virtual void PopClipRect() = 0;
			virtual void PushClipGeometry(MGeometry* geometry) = 0;
			virtual void PopClipGeometry() = 0;
			virtual void Clear(_m_color color = 0) = 0;
			virtual _m_result EndDraw() = 0;
			virtual MCanvas* GetCanvas() = 0;
			virtual MCanvas* GetRenderCanvas() = 0;
			virtual void* Get() = 0;
			virtual void Flush() = 0;
			virtual MCanvas* GetSharedCanvas() = 0;
			virtual bool SaveMBitmap(MBitmap* bitmap, std::wstring_view path, MImgFormat format) = 0;
			virtual bool SaveMCanvas(MCanvas* canvas, std::wstring_view path, MImgFormat format) = 0;
			virtual UIResource SaveMBitmap(MBitmap* bitmap, MImgFormat format) = 0;
			virtual UIResource SaveMCanvas(MCanvas* canvas, MImgFormat format) = 0;
			virtual bool CheckSubAtlasSource(MCanvas* canvas, MCanvas* canvas1) = 0;
		};
	}
}