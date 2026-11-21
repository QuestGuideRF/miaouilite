
#pragma once
#include <set>
#include <Render/Graphs/Mui_RenderDef.h>
#include <FileSystem/DreamMoonRes.h>
#define MUIRESFILE std::pair<Mui::DMResources*, std::wstring>
namespace Mui
{
	enum class UIStyleType
	{
		Null,
		Image,
		Geometry
	};
	class UIResourceMgr;
	namespace Window { class UIWindowBasic; }
	class UIBitmap : public RAII::MBasicObj
	{
	public:
		UIBitmap(UIBitmap&&) = delete;
		UIBitmap(const UIBitmap&) = delete;
		UIBitmap& operator=(const UIBitmap&) = delete;
		UIBitmap& operator=(UIBitmap&&) = delete;
		Render::Def::MBitmapPtr GetBitmap() const { return m_bitmap; }
	protected:
		UIBitmap() = default;
		Render::Def::MBitmapPtr m_bitmap = nullptr;
		friend class UIResourceMgr;
	};
	using UIBitmapPtr = RAII::Mui_Ptr<UIBitmap>;
	class UIStyle : public RAII::MBasicObj
	{
	public:
		UIStyle(UIStyle&&) = delete;
		UIStyle(const UIStyle&) = delete;
		UIStyle& operator=(const UIStyle&) = delete;
		UIStyle& operator=(UIStyle&&) = delete;
		[[nodiscard]] UIStyleType GetType() const { return m_type; }
		[[nodiscard]] _m_ushort GetPartCount() const { return m_partCount; }
		[[nodiscard]] _m_rect_t<int> GetNineGridPath() const { return m_gridPath; }
		[[nodiscard]] bool IsNineGrid() const { return m_nineGrid; }
		virtual void PaintStyle(Render::MRenderCmd* render, MPCRect dest,
			_m_byte alpha, int state, _m_ushort count, _m_scale scale = { 1.f, 1.f }) = 0;
	protected:
		UIStyle() = default;
		UIStyleType m_type = UIStyleType::Null;
		_m_ushort m_partCount = 0;
		bool m_nineGrid = false;
		_m_rect_t<int> m_gridPath = { 0 };
		friend class UIResourceMgr;
	};
	using UIStylePtr = RAII::Mui_Ptr<UIStyle>;
	class UIResourceMgr final
	{
	public:
		explicit UIResourceMgr(Render::MRenderCmd* render) : m_render(render) {}
		UIResourceMgr(UIResourceMgr&&) = delete;
		UIResourceMgr(const UIResourceMgr&) = delete;
		UIResourceMgr& operator=(const UIResourceMgr&) = delete;
		UIResourceMgr& operator=(UIResourceMgr&&) = delete;
		~UIResourceMgr();
		bool AddResourcePath(std::wstring_view path, std::wstring_view key);
		bool AddResourceMem(UIResource memfile, std::wstring_view key);
		bool RemoveResource(std::wstring_view path);
		UIStyle* FindStyle(std::wstring_view name);
		void LoadStyleList();
		bool LoadStyle(std::wstring_view resname, UIStyle** dst = nullptr);
		bool AddImageStyle(std::wstring_view name, UIResource memimg, _m_ushort count,
			UIStyle** dst = nullptr, bool nineGrid = false, _m_rect_t<int> gridpath = { 0 });
		bool AddGeometryStyle(std::wstring_view name, std::wstring_view xml, UIStyle** dst = nullptr);
		[[nodiscard]] UIStyle* RemoveStyle(std::wstring_view resname);
		UIResource ReadResource(std::wstring_view name);
		UIBitmapPtr CreateUniqueUIBitamp(std::wstring_view name);
		UIBitmapPtr CreateUniqueUIBitamp(UIResource res);
		UIBitmapPtr CreateSharedUIBitmap(std::wstring_view name);
	private:
		Render::MRenderCmd* m_render = nullptr;
		struct resfile
		{
			MUIRESFILE file;
			std::wstring path;
		};
		std::vector<resfile> m_resList;
		struct StyleData
		{
			UIStylePtr style = nullptr;
			bool memres = false;
			std::wstring srcname;
		};
		std::unordered_map<std::wstring, StyleData> m_styleList;
		std::unordered_map<std::wstring, UIBitmap*> m_sharedBmpList;
		bool LoadStyleFromDMRes(MUIRESFILE& file, std::wstring_view name, std::wstring& dst,
			bool res = false, UIResource* dstres = nullptr);
		bool LoadStyleInternal(MUIRESFILE& file, std::wstring_view name, UIStyle** dststyle);
		bool AddImageStyleInternal(UIResource memimg, _m_ushort count,
			UIStyle** dst = nullptr, bool nineGrid = false, _m_rect_t<int> gridpath = { 0 });
		bool AddGeometryStyleInternal(std::wstring_view xml, UIStyle** dst = nullptr);
		void FreeResFile(MUIRESFILE& file);
		friend class UIStyle;
		friend class UIMgrResource;
		friend class UIBitmapShared;
		friend class UIBitmapSVG;
		friend class Window::UIWindowBasic;
	};
	class UIStyleImage final : public UIStyle
	{
	protected:
		void PaintStyle(Render::MRenderCmd* render, MPCRect dest,
			_m_byte alpha, int state, _m_ushort count, _m_scale scale) override;
		UIBitmapPtr m_bitmap = nullptr;
		friend class UIResourceMgr;
	};
	class UIStyleGeometry final : public UIStyle
	{
	protected:
		void PaintStyle(Render::MRenderCmd* render, MPCRect dest,
			_m_byte alpha, int state, _m_ushort count, _m_scale scale) override;
		void InitResource(Render::MRenderCmd* render);
		struct CMD
		{
			int type = 0;
			_m_rect_t<int> m_dst = { 0 };
			_m_byte m_rctype[4] = { 0 };
			_m_color color = 0;
			float param = 0.f;
			int width = 0;
		};
		Render::Def::MPenPtr m_pen = nullptr;
		Render::Def::MBrushPtr m_brush = nullptr;
		std::vector<std::vector<CMD>> m_cmdlist;
		friend class UIResourceMgr;
	};
}