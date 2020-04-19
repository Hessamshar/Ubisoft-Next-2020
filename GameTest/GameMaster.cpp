#include "stdafx.h"

#include <math.h>
#include <random>

#include "GameMaster.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Bonus.h"
#include "GameSettings.h"
#include "app/AppSettings.h"
#include "app/app.h"

GameMaster* GameMaster::instance = nullptr;

GameMaster::GameMaster()
{

}

GameMaster::~GameMaster()
{
    if (m_player)
    {
        delete m_player;
    }
    for (int i = 0; i < m_triangles.size(); i++)
    {
        delete m_triangles[i];
    }
    for (int i = 0; i < m_bullets.size(); i++)
    {
        delete m_bullets[i];
    }
    for (int i = 0; i < m_enemies.size(); i++)
    {
        delete m_enemies[i];
    }
    for (int i = 0; i < m_bonuses.size(); i++)
    {
        delete m_bonuses[i];
    }
    m_player = nullptr;
    m_triangles.clear();
    m_triangles.shrink_to_fit();
    m_bullets.clear();
    m_bullets.shrink_to_fit();
    m_enemies.clear();
    m_enemies.shrink_to_fit();
    m_bonuses.clear();
    m_bonuses.shrink_to_fit();
}

GameMaster* GameMaster::GetInstance()
{
    if (!instance)
    {
        instance = nullptr;
        instance = new GameMaster();
    }
    return instance;
}

void GameMaster::ResetInstance()
{
    delete instance;
    instance = nullptr;
}

void GameMaster::CreateMap(int num_triangles)
{
    num_triangles = num_triangles <= 0 || num_triangles > MAX_NUMBER_OF_TRIANGLES ? MAX_NUMBER_OF_TRIANGLES : num_triangles;
    float angle = 360.0f / num_triangles;
    angle = angle * PI / 180;
    float center_x = APP_VIRTUAL_CENTER_X;
    float center_y = APP_VIRTUAL_CENTER_Y;
    float points_x[MAX_NUMBER_OF_TRIANGLES];
    float points_y[MAX_NUMBER_OF_TRIANGLES];
    points_x[0] = 0;
    points_y[0] = LEVEL_RADIUS;
    for (int i = 1; i < num_triangles; i++)
    {
        points_x[i] = points_x[i - 1] * cosf(angle) + points_y[i - 1] * sinf(angle);
        points_y[i] = -1 * points_x[i - 1] * sinf(angle) + points_y[i - 1] * cosf(angle);
    }
    for (int i = 0; i < num_triangles; i++)
    {
        int second_point = i == num_triangles - 1 ? 0 : i + 1;
        m_triangles.push_back(new TriangleBase(
            center_x, center_y,
            center_x + points_x[i], center_y + points_y[i],
            center_x + points_x[second_point], center_y + points_y[second_point]));
    }
    m_triangle_size = num_triangles;
    m_current = 0;
}

void GameMaster::MovePlayerRight()
{
    m_current = (m_current + 1) % m_triangle_size;
}

void GameMaster::MovePlayerLeft()
{
    m_current = m_current == 0 ? m_triangle_size - 1 : m_current - 1;
}

void GameMaster::SetPlayer(CSimpleSprite* player_sprite, int frame)
{
    if (m_triangle_size > 0)
    {
        m_player = new Player(m_triangles[m_current]->GetMidX(), m_triangles[m_current]->GetMidY(), APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y);
        m_player->SetSprite(player_sprite, frame);
        m_max_lives = MAX_LIVES_LV1;
        m_max_money = MAX_MONEY_LV1;
        m_max_nukes = MAX_NUKES_LV1;
        m_lives = m_max_lives;
        m_player_hit_timer = PLAYER_HIT_TIME + 1;
    }
}

void GameMaster::AddBullet(CSimpleSprite* bullet_sprite, int frame)
{
    if (m_triangle_size > 0 && m_player)
    {
        m_bullets.push_back(new Bullet(m_player->GetX(), m_player->GetY(), APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y));
        m_bullets.back()->SetSprite(bullet_sprite, frame);
        m_bullets.back()->SetSpeed(BULLET_SPEED);
    }
}

void GameMaster::AddEnemy(CSimpleSprite* enemy_sprite, int frame)
{
    if (m_triangle_size > 0 && m_player)
    {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<int> distr(0, m_triangle_size - 1);
        int random_triangle = distr(generator);
        m_enemies.push_back(new Enemy(APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y,
            m_triangles[random_triangle]->GetMidX(), m_triangles[random_triangle]->GetMidY()));
        m_enemies.back()->SetSprite(enemy_sprite, frame);
        m_enemies.back()->SetSpeed(ENEMY_SPEED);
    }
}

void GameMaster::AddBonus(CSimpleSprite* bonus_sprite, int frame)
{
    if (m_triangle_size > 0 && m_player)
    {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<int> distr(0, m_triangle_size - 1);
        int random_triangle = distr(generator);
        m_bonuses.push_back(new Bonus(APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y,
            m_triangles[random_triangle]->GetMidX(), m_triangles[random_triangle]->GetMidY()));
        m_bonuses.back()->SetSprite(bonus_sprite, frame);
        m_bonuses.back()->SetSpeed(BONUS_SPEED);
    }
}

void GameMaster::AddMoney(int money)
{
    money = m_bonus && money > 0 ? money * 2 : money;
    m_money += money;
    m_money = m_money > m_max_money ? m_max_money : m_money;
}

void GameMaster::AddPoints(int points)
{
    points = m_bonus && points > 0 ? points * 2 : points;
    m_points += points;
}

void GameMaster::BuyLives()
{
    if (m_money >= LIFE_PRICE && m_lives < m_max_lives)
    {
        AddMoney(-1 * LIFE_PRICE);
        m_lives += 1;
    }
}

void GameMaster::BuyNukes()
{
    if (m_money >= NUKE_PRICE && m_nukes < m_max_nukes)
    {
        AddMoney(-1 * NUKE_PRICE);
        m_nukes += 1;
    }
}

void GameMaster::UseNukes()
{
    if (m_nukes > 0)
    {
        m_nukes -= 1;
        NukeEnemies();
    }
}

void GameMaster::NukeEnemies()
{
    for (int i = 0; i < m_enemies.size(); i++)
    {
        AddPoints(ENEMY_DESTROY_POINT);
        AddMoney(ENEMY_DESTROY_MONEY);
        delete m_enemies[i];
    }
    m_enemies.clear();
    m_enemies.shrink_to_fit();
}

void GameMaster::PlayerHit(int enemy_index)
{
    if (m_player_hit_timer >= PLAYER_HIT_TIME)
    {
        m_enemies[enemy_index]->Destroy();
        m_player_hit_timer = 0.0f;
        m_player->GetSprite()->SetColor(0.5f, 1.0f, 0.2f);
        m_lives--;
        if (m_lives <= 0)
        {
            m_player->Destroy();
        }
    }
}

void GameMaster::CollisionDetection()
{
    for (int i = 0; i < m_enemies.size(); i++)
    {
        if (AreColliding(m_enemies[i], m_player))
        {
            PlayerHit(i);
            continue;
        }
        for (int j = 0; j < m_bullets.size(); j++)
        {
            if (AreColliding(m_enemies[i], m_bullets[j]))
            {
                EnemyHit(i);
                m_bullets[j]->Destroy();
                break;
            }
        }
    }
    for (int i = 0; i < m_bonuses.size(); i++)
    {
        if (AreColliding(m_bonuses[i], m_player))
        {
            BonusHit(i);
            continue;
        }
    }
}

void GameMaster::EnemyHit(int enemy_index)
{
    AddPoints(ENEMY_DESTROY_POINT);
    AddMoney(ENEMY_DESTROY_MONEY);
    m_enemies[enemy_index]->Destroy();
}

void GameMaster::BonusHit(int bonus_index)
{
    m_bonus = true;
    m_bonuses[bonus_index]->Destroy();
}

bool GameMaster::AreColliding(GameObject* obj1, GameObject* obj2)
{
    if ((obj1->GetX() - obj1->GetCollisionWidth() / 2 * obj1->GetSprite()->GetScale() < obj2->GetX() + obj2->GetCollisionWidth() / 2 * obj2->GetSprite()->GetScale()) &&
        (obj1->GetX() + obj1->GetCollisionWidth() / 2 * obj1->GetSprite()->GetScale() > obj2->GetX() - obj2->GetCollisionWidth() / 2 * obj2->GetSprite()->GetScale()) &&
        (obj1->GetY() - obj1->GetCollisionHeight() / 2 * obj1->GetSprite()->GetScale() < obj2->GetY() + obj2->GetCollisionHeight() / 2 * obj2->GetSprite()->GetScale()) &&
        (obj1->GetY() + obj1->GetCollisionHeight() / 2 * obj1->GetSprite()->GetScale() > obj2->GetY() - obj2->GetCollisionHeight() / 2 * obj2->GetSprite()->GetScale()))
    {
        return true;
    }
    return false;
}

void GameMaster::FirstUpgradePlayer()
{
    m_max_lives = MAX_LIVES_LV2;
    m_max_money = MAX_MONEY_LV2;
    m_max_nukes = MAX_NUKES_LV2;
    m_player->GetSprite()->SetFrame(1);
}

void GameMaster::SecondUpgradePlayer()
{
    m_max_lives = MAX_LIVES_LV3;
    m_max_money = MAX_MONEY_LV3;
    m_max_nukes = MAX_NUKES_LV3;
    m_player->GetSprite()->SetFrame(2);
}

bool GameMaster::IsGameOver() const
{
    if (m_triangle_size > 0 && m_player)
    {
        if (m_player->IsDestroyed())
        {
            m_player->GetSprite()->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
        }
        return m_player->IsDestroyed();
    }
    return false;
}

void GameMaster::Update(float dt)
{
    if (m_triangle_size > 0 && m_player)
    {
        m_player_hit_timer += dt;
        m_player_hit_timer = m_player_hit_timer > PLAYER_HIT_TIME ? PLAYER_HIT_TIME + 1 : m_player_hit_timer;
        if (m_player_hit_timer > PLAYER_HIT_TIME)
        {
            m_player->GetSprite()->SetColor(1.0f, 1.0f, 1.0f);
        }
        if (m_bonus)
        {
            m_bonus_time += dt;
            if (m_bonus_time > BONUS_TIME)
            {
                m_bonus_time = 0.0f;
                m_bonus = false;
            }
        }
        CollisionDetection();
        m_player->SetPosition(m_triangles[m_current]->GetMidX(), m_triangles[m_current]->GetMidY());
        float angle = atan2(APP_VIRTUAL_CENTER_Y - (double)m_triangles[m_current]->GetMidY(),
            (double)m_triangles[m_current]->GetMidX() - APP_VIRTUAL_CENTER_X);
        m_player->Update(dt);
        for (int i = 0; i < m_bullets.size(); i++)
        {
            if (m_bullets[i]->IsDestroyed())
            {
                delete m_bullets[i];
                m_bullets.erase(m_bullets.begin() + i);
            }
            else
            {
                m_bullets[i]->Update(dt);
            }
        }
        for (int i = 0; i < m_enemies.size(); i++)
        {
            if (m_enemies[i]->IsDestroyed())
            {
                delete m_enemies[i];
                m_enemies.erase(m_enemies.begin() + i);
            }
            else
            {
                m_enemies[i]->Update(dt);
            }
        }
        for (int i = 0; i < m_bonuses.size(); i++)
        {
            if (m_bonuses[i]->IsDestroyed())
            {
                delete m_bonuses[i];
                m_bonuses.erase(m_bonuses.begin() + i);
            }
            else
            {
                m_bonuses[i]->Update(dt);
            }
        }
    }
}

void GameMaster::Draw()
{
    for (int i = 0; i < m_triangle_size; i++)
    {
        if (i != m_current)
        {
            m_triangles[i]->SetColor(1, 1, 1);
            m_triangles[i]->Draw();
        }
    }
    if (m_current != -1)
    {
        m_triangles[m_current]->SetColor(1.00, 0.87, 0.05);
        m_triangles[m_current]->Draw();
    }
    if (m_triangle_size > 0 && m_player)
    {
        for (int i = 0; i < m_bullets.size(); i++)
        {
            m_bullets[i]->Draw();
        }
        for (int i = 0; i < m_enemies.size(); i++)
        {
            m_enemies[i]->Draw();
        }
        for (int i = 0; i < m_bonuses.size(); i++)
        {
            m_bonuses[i]->Draw();
        }
        m_player->Draw();
    }
}
