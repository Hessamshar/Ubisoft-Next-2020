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
	void CreateMap(int num_triangles);
	void MovePlayerRight();
	void MovePlayerLeft();
	void SetPlayer(CSimpleSprite* player_sprite, int frame);
	void AddBullet(CSimpleSprite* bullet_sprite, int frame);
	void AddEnemy(CSimpleSprite* enemy_sprite, int frame);
	void Update(float dt);
	void Draw();
private:
	GameMaster();
	static GameMaster *instance;
	GameObject *m_player = NULL;
	std::vector<TriangleBase*> m_triangles;
	int m_triangle_size = 0;
	int m_current = -1;
	std::vector<GameObject*> m_bullets;
	std::vector<GameObject*> m_enemies;

	void CollisionDetection();
	bool AreColliding(GameObject* obj1, GameObject* obj2);
};

#endif
