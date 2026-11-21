
#include <Control/Mui_Progress.h>
namespace Mui::Ctrl
{
	UIProgressBar::UIProgressBar(UIControl* parent, Attribute attrib) : UIProgressBar(std::move(attrib))
	{
		M_ASSERT(parent)
		parent->AddChildren(this);
	}
	UIProgressBar::UIProgressBar(Attribute attrib) : m_attrib(std::move(attrib))
	{
		m_cacheUpdate = true;
	}
	void UIProgressBar::SetAttribute(std::wstring_view attribName, std::wstring_view attrib, bool draw)
	{
		if (m_attrib.SetAttribute(attribName, attrib))
		{
			m_cacheUpdate = true;
			if (draw)
				UpdateDisplay();
		}
		else
			UIControl::SetAttribute(attribName, attrib, draw);
	}
	std::wstring UIProgressBar::GetAttribute(std::wstring_view attribName)
	{
		if (std::wstring ret; m_attrib.GetAttribute(attribName, ret))
			return ret;
		return UIControl::GetAttribute(attribName);
	}
	void UIProgressBar::Register()
	{
		BindAttribute();
		static auto method = [](UIControl* parent)
		{
			return new UIProgressBar(parent, Attribute());
		};
		MCTRL_REGISTER(method);
	}
	void UIProgressBar::SetMaxValue(int value, bool draw)
	{
		m_attrib.Set().maxValue = value;
		m_cacheUpdate = true;
		if (draw)
			UpdateDisplay();
	}
	int UIProgressBar::GetMaxValue() const
	{
		return m_attrib.Get().maxValue;
	}
	void UIProgressBar::SetCurValue(int value, bool draw)
	{
		m_attrib.Set().value = value;
		m_cacheUpdate = true;
		if (draw)
			UpdateDisplay();
	}
	int UIProgressBar::GetCurValue() const
	{
		return m_attrib.Get().value;
	}
	void UIProgressBar::OnPaintProc(MPCPaintParam param)
	{
		const auto style = m_attrib.Get().style;
		if (!style)
			return;
		CalcDrawRect(param->destRect);
		_m_byte alpha = UINodeBase::m_data.AlphaDst;
		if (param->cacheCanvas) alpha = 255;
		auto scale = GetRectScale().scale();
		style->PaintStyle(param->render, param->destRect, alpha, IsEnabled() ? 0 : 2, 4, scale);
		style->PaintStyle(param->render, &m_drawRect, alpha, IsEnabled() ? 1 : 3, 4, scale);
	}
	int UIProgressBar::Percentage(int value1, int percentValue, int percent)
	{
		float ret = float(value1) * (float)percentValue / (float)percent;
		return (int)ret;
	}
	void UIProgressBar::CalcDrawRect(MPCRect dest)
	{
		m_drawRect = *dest;
		if (auto& attrib = m_attrib.Get(); attrib.vertical)
		{
			int calcValue = Percentage(attrib.value, m_drawRect.GetHeight(), attrib.maxValue);
			if (attrib.bottomShow)
				m_drawRect.top += m_drawRect.GetHeight() - calcValue;
			else
				m_drawRect.bottom -= m_drawRect.GetHeight() - calcValue;
		}
		else
		{
			int calcValue = Percentage(attrib.value, m_drawRect.GetWidth(), attrib.maxValue);
			if (!attrib.leftShow)
				m_drawRect.left += m_drawRect.GetWidth() - calcValue;
			else
				m_drawRect.right -= m_drawRect.GetWidth() - calcValue;
		}
	}
	void UIProgressBar::BindAttribute()
	{
		auto list =
		{
			MakeUIAttrib(m_attrib, &Attribute::style, L"style"),
			MakeUIAttrib(m_attrib, &Attribute::maxValue, L"maxValue"),
			MakeUIAttrib(m_attrib, &Attribute::value, L"value"),
			MakeUIAttrib(m_attrib, &Attribute::vertical, L"vertical"),
			MakeUIAttrib(m_attrib, &Attribute::leftShow, L"leftShow"),
			MakeUIAttrib(m_attrib, &Attribute::bottomShow, L"bottomShow")
		};
		decltype(m_attrib)::RegisterAttrib(list);
	}
}