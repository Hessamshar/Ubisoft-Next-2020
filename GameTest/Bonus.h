#ifndef _BONUS_H_
#define _BONUS_H_

#include "GameObject.h"
#include "app\SimpleSprite.h"

class Bonus : public GameObject
{
public:
	Bonus(float x, float y, float target_x, float target_y);
	void SetSprite(CSimpleSprite* sprite, int frame);
	void Update(float dt);
};

#endif
