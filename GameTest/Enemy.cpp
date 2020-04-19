#include "stdafx.h"

#include "Enemy.h"
#include "app\app.h"
#include "GameSettings.h"


Enemy::Enemy(float x, float y, float target_x, float target_y) : GameObject(x, y, target_x, target_y)
{
	SetRotDirRand();
}

void Enemy::SetSprite(CSimpleSprite* sprite, int frame)
{
	GameObject::SetSprite(sprite, frame);
	m_sprite->SetScale(0.0f);
};

void Enemy::Update(float dt)
{
	if (m_sprite)
	{
		m_angle = PI - atan2(m_target_y - (double)m_y, (double)m_x - m_target_x);
		m_sprite->SetAngle(3 * PI / 2 + m_angle);
		m_sprite->SetPosition(m_x, m_y);
		m_sprite->Update(dt);
	}
	if (m_move_mode == 0)
	{
		float d = sqrtf(pow((double)m_target_x - m_x, 2) + pow((double)m_target_y - m_y, 2));
		float d_p = d / m_initial_d;
		m_sprite->SetScale(1.0f - d_p);
		if (d_p > 0.02f)
		{
			float dx = cosf(m_angle) * m_speed * dt / 1000;
			float dy = sinf(m_angle) * m_speed * dt / 1000;
			m_x += dx;
			m_y += dy;
		}
		else
		{
			m_move_mode = 1;
			m_x = m_target_x;
			m_y = m_target_y;
			m_target_x = APP_VIRTUAL_CENTER_X;
			m_target_y = APP_VIRTUAL_CENTER_Y;
			m_speed = ENEMY_ROTATION_SPEED;
			m_collision_width /= 2;
			m_collision_height /= 2;
		}
	}
	else
	{
		float angle = m_rotaiton_dir * m_speed * dt / 1000;
		m_x = (m_x - m_target_x) * cosf(angle) - (m_y - m_target_y) * sinf(angle) + m_target_x;
		m_y = (m_x - m_target_x) * sinf(angle) + (m_y - m_target_y) * cosf(angle) + m_target_y;
	}
}

void Enemy::SetRotDirRand()
{
	m_rotaiton_dir = (rand() > RAND_MAX / 2) ? 1 : -1;
}
