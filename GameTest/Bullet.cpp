#include "stdafx.h"

#include "Bullet.h"
#include "app\app.h"
#include "GameSettings.h"


Bullet::Bullet(float x, float y, float target_x, float target_y)
{
	m_x = x;
	m_y = y;
	m_target_x = target_x;
	m_target_y = target_y;
	m_initial_d = sqrtf(pow((double)m_target_x - m_x, 2) + pow((double)m_target_y - m_y, 2));
}

Bullet::~Bullet()
{
	delete m_sprite;
}

void Bullet::SetSprite(CSimpleSprite* sprite, int frame)
{
	m_sprite = sprite;
	m_sprite->SetPosition(m_x, m_y);
	m_sprite->SetFrame(frame);
	m_sprite->SetScale(1.0f);
	m_angle = PI - atan2(m_target_y - (double)m_y, (double)m_x - m_target_x);
	m_sprite->SetAngle(3 * PI / 2 + m_angle);
};

void Bullet::Update(float dt)
{
	if (m_sprite)
	{
		m_angle = PI - atan2(m_target_y - (double)m_y, (double)m_x - m_target_x);
		m_sprite->SetAngle(3 * PI / 2 + m_angle);
		m_sprite->SetPosition(m_x, m_y);
		m_sprite->Update(dt);
	}
	float d = sqrtf(pow((double)m_target_x - m_x, 2) + pow((double)m_target_y - m_y, 2));
	float d_p = d / m_initial_d;
	m_sprite->SetScale(d_p);
	if (d_p > 0.02f)
	{
		float dx = cosf(m_angle) * m_speed * dt / 1000;
		float dy = sinf(m_angle) * m_speed * dt / 1000;
		m_x += dx;
		m_y += dy;
	}
	else
	{
		m_destroyed = true;
	}
}

void Bullet::Draw()
{
	if (m_sprite)
	{
		m_sprite->Draw();
		if (DEBUG)
		{
			float width = m_sprite->GetWidth() * m_sprite->GetScale() / 2;
			float height = m_sprite->GetHeight() * m_sprite->GetScale() / 2;
			App::DrawLine(m_x - width, m_y - height, m_x + width, m_y - height, 0.5, 1, 0.5);
			App::DrawLine(m_x + width, m_y - height, m_x + width, m_y + height, 0.5, 1, 0.5);
			App::DrawLine(m_x + width, m_y + height, m_x - width, m_y + height, 0.5, 1, 0.5);
			App::DrawLine(m_x - width, m_y + height, m_x - width, m_y - height, 0.5, 1, 0.5);
		}
	}
}
