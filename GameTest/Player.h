#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include "app\SimpleSprite.h"

class Player : public GameObject
{
public:
	Player(float x, float y, float target_x, float target_y);
	void SetSprite(CSimpleSprite* sprite, int frame);
	void Update(float dt);
};

#endif
