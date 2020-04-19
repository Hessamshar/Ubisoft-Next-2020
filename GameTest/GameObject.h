#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "app\SimpleSprite.h"

class GameObject
{
public:
	GameObject(float x, float y, float target_x, float target_y);
	~GameObject();
	virtual void SetSprite(CSimpleSprite* sprite, int frame);
	void SetPosition(float x, float y) { m_x = x; m_y = y; };
	void SetSpeed(float speed) { m_speed = speed; };
	void SetCollisionWidth(float width) { m_collision_width = width; };
	void SetCollisionHeight(float height) { m_collision_height = height; };
	float GetX() { return m_x; };
	float GetY() { return m_y; };
	CSimpleSprite* GetSprite() { return m_sprite; };
	bool IsDestroyed() const { return m_destroyed; }
	virtual void Update(float dt) = 0;
	virtual void Draw();
protected:
	CSimpleSprite* m_sprite = NULL;
	float m_x = 0.0F;
	float m_y = 0.0F;
	float m_target_x = 0.0F;
	float m_target_y = 0.0F;
	float m_speed = 0.0F;
	float m_angle = 0.0F;
	float m_initial_d = 0.0f;
	float m_collision_width = 50.0f;
	float m_collision_height = 50.0f;
	bool m_destroyed = false;
};

#endif
