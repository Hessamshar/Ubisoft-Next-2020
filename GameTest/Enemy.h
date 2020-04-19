#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "GameObject.h"
#include "app\SimpleSprite.h"

class Enemy : public GameObject
{
public:
	Enemy(float x, float y, float target_x, float target_y);
	void SetSprite(CSimpleSprite* sprite, int frame);
	void Update(float dt);
private:
	int m_move_mode = 0;
	int m_rotaiton_dir = 1;

	void SetRotDirRand();
};

#endif
