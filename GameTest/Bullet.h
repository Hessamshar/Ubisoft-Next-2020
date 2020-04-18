#ifndef _BULLET_H_
#define _BULLET_H_

#include "app\SimpleSprite.h"

class Bullet
{
public:
	Bullet(float x, float y, float target_x, float target_y);
	~Bullet();
	void SetSprite(CSimpleSprite* sprite, int frame);
	void SetSpeed(float speed) { m_speed = speed; };
	bool IsDestroyed() const { return m_destroyed; }
	void Update(float dt);
	void Draw();
private:
	CSimpleSprite* m_sprite = NULL;
	float m_x = 0.0F;
	float m_y = 0.0F;
	float m_target_x = 0.0F;
	float m_target_y = 0.0F;
	float m_speed = 0.0F;
	float m_angle = 0.0F;
	float m_initial_d = 0.0f;
	bool m_destroyed = false;
};

#endif
