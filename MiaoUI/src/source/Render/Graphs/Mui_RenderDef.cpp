
#include <Render/Graphs/Mui_RenderDef.h>
#include <Render/Mui_RenderMgr.h>
namespace Mui::Render::Def
{
	void MRenderObj::ReleaseProc()
	{
		if (IsBaseThread())
		{
			ReleaseThis();
			return;
		}
		static_cast<MRenderCmd*>(m_base)->Task([this]
		{
			ReleaseThis();
		});
	}
	bool MRenderObj::IsBaseThread()
	{
		return static_cast<MRenderCmd*>(m_base)->IsTaskThread();
	}
	bool MRenderObj::IsDrawing()
	{
		return static_cast<MRenderCmd*>(m_base)->m_begindraw;
	}
	void* MRenderObj::GetBaseRender()
	{
		return static_cast<MRenderCmd*>(m_base)->m_base;
	}
}