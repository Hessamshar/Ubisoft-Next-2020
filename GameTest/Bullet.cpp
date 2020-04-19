#include "stdafx.h"

#include "Bullet.h"
#include "app\app.h"
#include "GameSettings.h"


Bullet::Bullet(float x, float y, float target_x, float target_y) : GameObject(x, y, target_x, target_y)
{

}

void Bullet::SetSprite(CSimpleSprite* sprite, int frame)
{
	GameObject::SetSprite(sprite, frame);
	m_sprite->SetScale(1.0f);
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
