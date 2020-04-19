#include "stdafx.h"

#include "Player.h"
#include "app\app.h"
#include "GameSettings.h"


Player::Player(float x, float y, float target_x, float target_y) : GameObject(x, y, target_x, target_y)
{

}

void Player::SetSprite(CSimpleSprite* sprite, int frame)
{
	GameObject::SetSprite(sprite, frame);
	m_sprite->SetScale(1.0f);
};

void Player::Update(float dt)
{
	if (m_sprite)
	{
		m_angle = PI - atan2(m_target_y - (double)m_y, (double)m_x - m_target_x);
		m_sprite->SetAngle(3 * PI / 2 + m_angle);
		m_sprite->SetPosition(m_x, m_y);
		m_sprite->Update(dt);
	}
}
