#ifndef _BULLET_H_
#define _BULLET_H_

#include "GameObject.h"
#include "app\SimpleSprite.h"

class Bullet : public GameObject
{
public:
	Bullet(float x, float y, float target_x, float target_y);
	void SetSprite(CSimpleSprite* sprite, int frame);
	void Update(float dt);
};

#endif
