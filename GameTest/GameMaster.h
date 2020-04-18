#ifndef _GAME_MASTER_H_
#define _GAME_MASTER_H_

#include <vector>
#include "TriangleBase.h"
#include "Bullet.h"
#include "app\SimpleSprite.h"


class GameMaster
{
public:
	~GameMaster();
	GameMaster(const GameMaster&) = delete;
	GameMaster(GameMaster&&) = delete;
	GameMaster& operator=(const GameMaster&) = delete;
	GameMaster& operator=(GameMaster&&) = delete;
	static GameMaster* getInstance();
	void SetPlayerSprite(CSimpleSprite* player, int frame);
	void CreateMap(int num_triangles);
	float GetPlayerX();
	float GetPlayerY();
	void MovePlayerRight();
	void MovePlayerLeft();
	void AddBullet(CSimpleSprite* bullet_sprite, int frame);
	void Update(float dt);
	void Draw();
private:
	GameMaster();
	static GameMaster *instance;
	CSimpleSprite *m_player_sprite = NULL;
	std::vector<TriangleBase*> m_triangles;
	int m_triangle_size = 0;
	int m_current = -1;
	std::vector<Bullet*> m_bullets;
};

#endif
