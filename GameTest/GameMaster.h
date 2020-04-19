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
	static GameMaster* GetInstance();
	static void ResetInstance();
	void CreateMap(int num_triangles);
	void MovePlayerRight();
	void MovePlayerLeft();
	void SetPlayer(CSimpleSprite* player_sprite, int frame);
	void AddBullet(CSimpleSprite* bullet_sprite, int frame);
	void AddEnemy(CSimpleSprite* enemy_sprite, int frame);
	int GetLives() const { return m_lives; };
	int GetPoints() const { return m_points; };
	int GetMoney() const { return m_money; };
	int GetNukes() const { return m_nukes; };
	void BuyNukes();
	void BuyLives();
	void UseNukes();
	bool IsGameOver() const;
	void Update(float dt);
	void Draw();
private:
	GameMaster();
	static GameMaster *instance;
	GameObject *m_player = nullptr;
	std::vector<TriangleBase*> m_triangles;
	int m_triangle_size = 0;
	int m_current = -1;
	std::vector<GameObject*> m_bullets;
	std::vector<GameObject*> m_enemies;
	int m_lives = 0;
	int m_points = 0;
	int m_money = 0;
	int m_nukes = 0;

	void CollisionDetection();
	bool AreColliding(GameObject* obj1, GameObject* obj2);
	void AddPoints(int points) { m_points += points; }
	void AddMoney(int money);
	void PlayerHit();
	void EnemyHit();
	void DestroyAllEnemies();
};

#endif
