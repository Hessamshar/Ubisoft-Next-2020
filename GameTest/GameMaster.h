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
	void FirstUpgradePlayer();
	void SecondUpgradePlayer();
	void AddBullet(CSimpleSprite* bullet_sprite, int frame);
	void AddEnemy(CSimpleSprite* enemy_sprite, int frame);
	void AddBonus(CSimpleSprite* bonus_sprite, int frame);
	int GetLives() const { return m_lives; };
	int GetPoints() const { return m_points; };
	int GetMoney() const { return m_money; };
	int GetNukes() const { return m_nukes; };
	int GetMaxLives() const { return m_max_lives; };
	int GetMaxMoney() const { return m_max_money; };
	int GetMaxNukes() const { return m_max_nukes; };
	void BuyNukes();
	void BuyLives();
	void UseNukes();
	bool IsBonus() const { return m_bonus; };
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
	std::vector<GameObject*> m_spawned_objects;
	int m_lives = 0;
	int m_points = 0;
	int m_money = 0;
	int m_nukes = 0;
	int m_max_lives = 0;
	int m_max_money = 0;
	int m_max_nukes = 0;
	float m_player_hit_timer = 0.0f;
	float m_bonus_time = 0.0f;
	bool m_bonus = false;

	void CollisionDetection();
	bool AreColliding(GameObject* obj1, GameObject* obj2);
	void AddPoints(int points);
	void AddMoney(int money);
	void Nuke();
	void PlayerHit(int enemy_index);
	void EnemyHit(int enemy_index);
	void BonusHit(int bonus_index);
};

#endif
