#ifndef _GAME_MASTER_H_
#define _GAME_MASTER_H_

#include <vector>
#include "TriangleBase.h"
#include "GameObject.h"
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
	void SetCollisionWidth(float width) { m_collision_width = width; };
	void SetCollisionHeight(float height) { m_collision_height = height; };
	float GetPlayerX();
	float GetPlayerY();
	void MovePlayerRight();
	void MovePlayerLeft();
	void AddBullet(CSimpleSprite* bullet_sprite, int frame);
	void AddEnemy(CSimpleSprite* enemy_sprite, int frame);
	void Update(float dt);
	void Draw();
private:
	GameMaster();
	static GameMaster *instance;
	CSimpleSprite *m_player_sprite = NULL;
	std::vector<TriangleBase*> m_triangles;
	int m_triangle_size = 0;
	int m_current = -1;
	float m_collision_width = 50.0f;
	float m_collision_height = 50.0f;
	std::vector<GameObject*> m_bullets;
	std::vector<GameObject*> m_enemies;

	void CollisionDetection();
};

#endif
