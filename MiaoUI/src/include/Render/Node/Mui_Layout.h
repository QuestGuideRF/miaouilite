
#pragma once
#include <Mui_Base.h>
namespace Mui
{
	namespace Render { class UINodeBase; }
	class UILayouter
	{
	public:
		UILayouter() = default;
		virtual ~UILayouter() = default;
		void Layout(Render::UINodeBase*, size_t = 0, bool = true);
		void SetType(UIAlignment align);
		[[nodiscard]] UIAlignment GetType() const { return m_type; }
	protected:
		UIAlignment m_type = UIAlignment_Block;
		bool m_linearParam_vertical = true;
		bool m_linearParam_filp = false;
		bool m_linearParam_swap = false;
		void Intersect(_m_rectf* dst, const _m_rectf* rect1, const _m_rectf* rect2);
		Render::UINodeBase* GetPreviousNode(Render::UINodeBase*, size_t);
		_m_sizef CalcContentSize(Render::UINodeBase*, bool);
		_m_sizef SizeClamp(Render::UINodeBase*, _m_sizef, _m_scale scale);
		std::pair<_m_rectf, _m_rectf> CalcPadding(Render::UINodeBase*);
		_m_rectf& CalcBaseFrame(Render::UINodeBase*, const _m_rectf&, _m_pointf&, _m_pointf, bool, int = 0);
		void LayoutBlock(Render::UINodeBase*, size_t, bool);
		void LayoutLinear(Render::UINodeBase*, size_t, bool);
		void LayoutAbsolute(Render::UINodeBase*, size_t, bool);
		void LayoutCenter(Render::UINodeBase*, bool);
		void LayoutGrid(Render::UINodeBase*);
		void AnimationLayout(Render::UINodeBase*);
		friend class Render::UINodeBase;
	};
}