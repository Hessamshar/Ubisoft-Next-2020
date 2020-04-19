#include "stdafx.h"

#include <windows.h>
#include <math.h>

#include "GameMaster.h"
#include "Bullet.h"
#include "Enemy.h"
#include "GameSettings.h"
#include "app/AppSettings.h"
#include "app/app.h"

CRITICAL_SECTION cs;

GameMaster* GameMaster::instance = NULL;

GameMaster::GameMaster()
{

}

GameMaster::~GameMaster()
{
    if (m_player_sprite)
    {
        delete m_player_sprite;
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
    m_triangles.clear();
    m_bullets.clear();
    m_enemies.clear();
}

GameMaster* GameMaster::getInstance()
{
    if (!instance)
    {
        //EnterCriticalSection(&cs);
        if (!instance)
        {
            instance = new GameMaster();
        }
        //LeaveCriticalSection(&cs);
    }
    return instance;
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

void GameMaster::SetPlayerSprite(CSimpleSprite* player, int frame)
{
    m_player_sprite = player;
    m_player_sprite->SetPosition(0.0f, 0.0f);
    m_player_sprite->SetFrame(frame);
    m_player_sprite->SetScale(1.0f);
};

void GameMaster::MovePlayerRight()
{
    m_current = m_current == m_triangle_size - 1 ? 0 : m_current + 1;
}

void GameMaster::MovePlayerLeft()
{
    m_current = m_current == 0 ? m_triangle_size - 1 : m_current - 1;
}

float GameMaster::GetPlayerX()
{
    if (m_triangle_size > 0 && m_player_sprite)
    {
        return m_triangles[m_current]->GetMidX();
    }
    return 0.0f;
}

float GameMaster::GetPlayerY()
{
    if (m_triangle_size > 0 && m_player_sprite)
    {
        return m_triangles[m_current]->GetMidY();
    }
    return 0.0f;
}

void GameMaster::AddBullet(CSimpleSprite* bullet_sprite, int frame)
{
    m_bullets.push_back(new Bullet(GetPlayerX(), GetPlayerY(), APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y));
    m_bullets.back()->SetSprite(bullet_sprite, frame);
    m_bullets.back()->SetSpeed(BULLET_SPEED);
}

void GameMaster::AddEnemy(CSimpleSprite* enemy_sprite, int frame)
{
    m_enemies.push_back(new Enemy(APP_VIRTUAL_CENTER_X, APP_VIRTUAL_CENTER_Y, GetPlayerX(), GetPlayerY()));
    m_enemies.back()->SetSprite(enemy_sprite, frame);
    m_enemies.back()->SetSpeed(ENEMY_SPEED);
}

void GameMaster::CollisionDetection()
{
    for (int i = 0; i < m_bullets.size(); i++)
    {
        for (int j = 0; j < m_enemies.size(); j++)
        {

        }
    }
}

void GameMaster::Update(float dt)
{
    if (m_triangle_size > 0 && m_player_sprite)
    {
        CollisionDetection();
        m_player_sprite->SetPosition(m_triangles[m_current]->GetMidX(), m_triangles[m_current]->GetMidY());
        float angle = atan2(APP_VIRTUAL_CENTER_Y - (double)m_triangles[m_current]->GetMidY(),
            (double)m_triangles[m_current]->GetMidX() - APP_VIRTUAL_CENTER_X);
        m_player_sprite->SetAngle(5 * PI / 2 - angle);
        m_player_sprite->Update(dt);
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
    if (m_triangle_size > 0 && m_player_sprite)
    {
        for (int i = 0; i < m_bullets.size(); i++)
        {
            m_bullets[i]->Draw();
        }
        for (int i = 0; i < m_enemies.size(); i++)
        {
            m_enemies[i]->Draw();
        }
        m_player_sprite->Draw();
        if (DEBUG)
        {
            float player_x = m_triangles[m_current]->GetMidX();
            float player_y = m_triangles[m_current]->GetMidY();
            float width = m_collision_width * m_player_sprite->GetScale() / 2;
            float height = m_collision_height * m_player_sprite->GetScale() / 2;
            App::DrawLine(player_x - width, player_y - height, player_x + width, player_y - height, 0.5, 1, 0.5);
            App::DrawLine(player_x + width, player_y - height, player_x + width, player_y + height, 0.5, 1, 0.5);
            App::DrawLine(player_x + width, player_y + height, player_x - width, player_y + height, 0.5, 1, 0.5);
            App::DrawLine(player_x - width, player_y + height, player_x - width, player_y - height, 0.5, 1, 0.5);
        }
    }
}
