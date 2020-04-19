#include "stdafx.h"

#include "GameObject.h"
#include "app\app.h"
#include "GameSettings.h"

bool GAME_SETTING_DEBUG;

GameObject::GameObject(float x, float y, float target_x, float target_y)
{
	m_x = x;
	m_y = y;
	m_target_x = target_x;
	m_target_y = target_y;
	m_initial_d = sqrtf(pow((double)m_target_x - m_x, 2) + pow((double)m_target_y - m_y, 2));
}

GameObject::~GameObject()
{
	delete m_sprite;
}

void GameObject::SetSprite(CSimpleSprite* sprite, int frame)
{
	m_sprite = sprite;
	m_sprite->SetPosition(m_x, m_y);
	m_sprite->SetFrame(frame);
	m_sprite->SetScale(0.0f);
	m_angle = PI - atan2(m_target_y - (double)m_y, (double)m_x - m_target_x);
	m_sprite->SetAngle(3 * PI / 2 + m_angle);
};

void GameObject::Draw()
{
	if (m_sprite)
	{
		m_sprite->Draw();
		if (DEBUG)
		{
			float width = m_collision_width * m_sprite->GetScale() / 2;
			float height = m_collision_height * m_sprite->GetScale() / 2;
			App::DrawLine(m_x - width, m_y - height, m_x + width, m_y - height, 0.5, 1, 0.5);
			App::DrawLine(m_x + width, m_y - height, m_x + width, m_y + height, 0.5, 1, 0.5);
			App::DrawLine(m_x + width, m_y + height, m_x - width, m_y + height, 0.5, 1, 0.5);
			App::DrawLine(m_x - width, m_y + height, m_x - width, m_y - height, 0.5, 1, 0.5);
		}
	}
}
