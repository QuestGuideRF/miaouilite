
#pragma once
#include <Render/Node/Mui_RenderNode.h>
#include <Render/Node/Mui_Layout.h>
#include <Window/Mui_BasicWnd.h>
namespace Mui::Render
{
	enum class UICacheType
	{
		Auto,
		Enable,
		Disable
	};
	struct UIBkgndStyle
	{
		_m_color bkgndColor = 0;
		_m_color FrameColor = 0;
		_m_ushort FrameWidth = 0;
		float RoundValue = 0;
		_m_color ShadowColor = 0;
		UIPoint ShadowOffset;
		int ShadowExtend = 0;
		float ShadowRadius = 6.f;
	};
	class UINodeBase : protected MRenderNode
	{
	public:
		UINodeBase();
		~UINodeBase() override;
		enum PosSizeUnitType
		{
			Default,
			Pixel,
			Percentage,
			FillMinus
		};
		struct PosSizeUnit
		{
			PosSizeUnitType x_w = Pixel;
			PosSizeUnitType y_h = Pixel;
		};
		virtual void SetName(const UIString& name);
		[[nodiscard]] virtual UIString GetName() const;
		virtual void SetPos(UIPoint position, bool draw = true);
		virtual void SetPos(int x, int y, bool draw = true);
		[[nodiscard]] virtual UIPoint GetPos() const;
		virtual void SetPosUnit(PosSizeUnit unit, bool draw = true);
		[[nodiscard]] virtual PosSizeUnit GetPosUnit() const;
		virtual void SetSize(UISize size, bool draw = true);
		virtual void SetSize(int width, int height, bool draw = true);
		[[nodiscard]] virtual UISize GetSize() const;
		virtual void SetSizeUnit(PosSizeUnit unit, bool draw = true);
		[[nodiscard]] virtual PosSizeUnit GetSizeUnit() const;
		virtual void SetMinSize(UISize size, bool draw = true);
		[[nodiscard]] virtual UISize GetMinSize() const;
		virtual void SetMaxSize(UISize size, bool draw = true);
		[[nodiscard]] virtual UISize GetMaxSize() const;
		virtual void SetPadding(UIRect padding, bool draw = true);
		[[nodiscard]] virtual UIRect GetPadding() const;
		virtual void SetAlignType(UIAlignment type, bool draw = true);
		[[nodiscard]] virtual UIAlignment GetAlignType() const;
		virtual void SetVisible(bool visible, bool draw = true);
		[[nodiscard]] virtual bool IsVisible() const;
		virtual void SetAlpha(_m_byte alpha, bool draw = true);
		[[nodiscard]] virtual _m_byte GetAlpha() const;
		virtual void SetParent(UINodeBase* UINode);
		[[nodiscard]] virtual UINodeBase* GetParent() const;
		virtual void AddChildren(UINodeBase* UINode);
		virtual void RemoveChildren(UINodeBase* UINode);
		virtual void RemoveChildren(const UIString& name);
		[[nodiscard]] virtual UINodeBase* FindChildren(const UIString& name) const;
		[[nodiscard]] virtual _m_uint GetChildrenCount();
		virtual void GetChildrenList(std::vector<UINodeBase*>& list);
		virtual void UpdateDisplay(bool updateCache = false);
		virtual void UpdateLayout();
		virtual void SetCacheType(UICacheType type);
		[[nodiscard]] virtual UICacheType GetCacheType() const;
		virtual void SetAnimationState(bool begin);
		virtual void SetBackground(UIBkgndStyle style);
		[[nodiscard]] virtual UIBkgndStyle GetBkgndStyle() const;
		virtual void SetScale(_m_rcscale scale, bool draw = true);
		[[nodiscard]] virtual _m_rcscale GetScale() const;
		virtual void EnableDPIScale(bool enable);
		[[nodiscard]] virtual bool IsDPIScaleEnabled() const;
		void Frame(_m_rect frame);
		[[nodiscard]] _m_rect Frame() const;
		void HideNode(bool hide, bool draw = true);
		[[nodiscard]] bool HideNode() const;
		void AutoSize(bool autosize, bool draw = true);
		[[nodiscard]] bool AutoSize() const;
		void InitDeviceResource();
	protected:
		virtual void OnLoadResource(MRenderCmd* render, bool recreate);
		[[nodiscard]] UINodeBase* GetValidParent() const;
		_m_rcscale GetRectScale() const
		{
			_m_rcscale scale = m_data.RectScale;
			if (m_data.EnableDPIScale)
			{
				scale.xs = m_data.DPIScale.cx * scale.xs;
				scale.ys = m_data.DPIScale.cy * scale.ys;
				scale.ws = m_data.DPIScale.cx * scale.ws;
				scale.hs = m_data.DPIScale.cy * scale.hs;
			}
			return scale;
		}
		template<typename T>
		void SizeScale(T& dstX, T& dstY, bool dpi = true)
		{
			_m_scale scale = m_data.RectScale.scale();
			if(dpi && m_data.EnableDPIScale)
			{
				scale.cx *= m_data.DPIScale.cx;
				scale.cy *= m_data.DPIScale.cy;
			}
			dstX = _scale_to(dstX, scale.cx);
			dstY = _scale_to(dstY, scale.cy);
		}
		template<typename T>
		void SizeScale(T& dst, bool x, bool dpi = true)
		{
			_m_scale scale = m_data.RectScale.scale();
			if (dpi && m_data.EnableDPIScale)
			{
				if(x) scale.cx *= m_data.DPIScale.cx;
				else scale.cy *= m_data.DPIScale.cy;
			}
			dst = _scale_to(dst, x ? scale.cx : scale.cy);
		}
		void UpdateScale();
		virtual void OnScale(_m_scale scale);
		virtual _m_sizef GetContentSize();
		virtual _m_pointf GetCalcedPoint();
		virtual void OnLayoutCalced() {}
		void OnRender(MRenderCmd* render, void* data) override;
		void OnRenderChildEnd(MRenderCmd* render, void* data) override;
		struct MPaintParam
		{
			MRenderCmd* render = nullptr;
			MPCRect clipRect = nullptr;
			MPCRect destRect = nullptr;
			bool cacheCanvas = false;
			_m_byte blendedAlpha = 255;
		};
		using MPCPaintParam = const MPaintParam*;
		virtual void OnPaintProc(MPCPaintParam param) { }
		void PaintBackground(MRenderCmd* render, MPCRect dst, bool cache);
	public:
		struct data
		{
			UISize Size;
			PosSizeUnit SizeUnit;
			UIPoint Position;
			PosSizeUnit PosUnit;
			UISize MinSize;
			UISize MaxSize = { -1, -1 };
			Window::UIWindowBasic* ParentWnd = nullptr;
			_m_rectf Frame;
			_m_rectf ClipFrame;
			_m_rect Padding;
			UILayouter Align;
			_m_byte AlphaSrc = 255;
			_m_byte AlphaDst = 255;
			UICacheType CacheType = UICacheType::Auto;
			MCanvasPtr SubAtlas = nullptr;
			bool HideThis = false;
			bool AutoSize = false;
			bool EnableDPIScale = true;
			_m_scale DPIScale = { 1.f,1.f };
			_m_rcscale RectScale = { 1.f,1.f,1.f,1.f };
		};
	protected:
		data m_data;
		struct brush
		{
			MPenPtr FramePen = nullptr;
			MBrushPtr BkgndBrush = nullptr;
		} m_brush;
		UIBkgndStyle m_bgStyle;
		bool m_cacheSupport = false;
		bool m_cacheUpdate = true;
		std::atomic_bool m_initialized;
		friend class Mui::UILayouter;
		friend class Window::UIWindowBasic;
	};
}