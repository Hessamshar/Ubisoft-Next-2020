#include "stdafx.h"

#include "TriangleBase.h"
#include "app\app.h"

TriangleBase::TriangleBase(float x1, float y1, float x2, float y2, float x3, float y3)
{
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	m_x3 = x3;
	m_y3 = y3;
	m_mid_x = x2 + (x3 - x2) / 2;
	m_mid_y = y2 + (y3 - y2) / 2;
}

void TriangleBase::Update(float dt)
{
	m_mid_x = m_x2 + (m_x3 - m_x2) / 2;
	m_mid_y = m_y2 + (m_y3 - m_y2) / 2;
}

void TriangleBase::Draw() const
{
	App::DrawLine(m_x1, m_y1, m_x2, m_y2, m_red, m_green, m_blue);
	App::DrawLine(m_x1, m_y1, m_x3, m_y3, m_red, m_green, m_blue);
	App::DrawLine(m_x2, m_y2, m_x3, m_y3, m_red, m_green, m_blue);
}
