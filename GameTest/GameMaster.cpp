#include "stdafx.h"

#include <windows.h>
#include <math.h>

#include "GameMaster.h"
#include "app/AppSettings.h"

CRITICAL_SECTION cs;

GameMaster* GameMaster::instance = NULL;

GameMaster::GameMaster()
{

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
    num_triangles = num_triangles <= 0 || num_triangles > 30 ? 20 : num_triangles;
    float angle = 360.0f / num_triangles;
    angle = angle * PI / 180;
    float radius = 300.0f;
    float x_center = APP_VIRTUAL_WIDTH / 2;
    float y_center = APP_VIRTUAL_HEIGHT / 2;
    float points_x[MAX_NUMBER_OF_TRIANGLES];
    float points_y[MAX_NUMBER_OF_TRIANGLES];
    float a = cosf(angle);
    points_x[0] = radius;
    points_y[0] = 0;
    for (int i = 1; i < num_triangles; i++)
    {
        points_x[i] = points_x[i - 1] * cosf(angle) + points_y[i - 1] * sinf(angle);
        points_y[i] = -1 * points_x[i - 1] * sinf(angle) + points_y[i - 1] * cosf(angle);
    }
    for (int i = 0; i < num_triangles; i++)
    {
        int second_point = i == num_triangles - 1 ? 0 : i + 1;
        m_triangles.push_back(std::make_unique<TriangleBase>(
            x_center, y_center,
            x_center + points_x[i], y_center + points_y[i],
            x_center + points_x[second_point], y_center + points_y[second_point]));
    }
}

void GameMaster::Draw()
{
    for (auto const& i : m_triangles)
    {
        i->Draw();
    }
}
