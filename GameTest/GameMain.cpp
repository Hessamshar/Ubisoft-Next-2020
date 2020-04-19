//------------------------------------------------------------------------
// GameMain.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <string>
//------------------------------------------------------------------------
#include "app\app.h"
#include "GameSettings.h"
#include "GameMaster.h"
#include "Bullet.h"
//------------------------------------------------------------------------
GameMaster *gameMaster;
float move_button_hold_time;
float shooting_timer;
float player_speed;
float enemy_respawner;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	gameMaster = nullptr;
	move_button_hold_time = MAX_MOVE_BUTTON_HOLD_TIME + 1;
	shooting_timer = SHOOTING_TIMER + 1;
	player_speed = 0.0f;
	enemy_respawner = 0.0f;
	gameMaster = GameMaster::GetInstance();
	gameMaster->CreateMap(30);
	gameMaster->SetPlayer(App::CreateSprite(".\\TestData\\ships2.bmp", 1, 3), 0);
}

void GameReset()
{
	GameMaster::ResetInstance();
	Init();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	enemy_respawner += deltaTime;
	shooting_timer += deltaTime;
	shooting_timer = shooting_timer > SHOOTING_TIMER ? SHOOTING_TIMER + 1 : shooting_timer;
	gameMaster->Update(deltaTime);
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		player_speed += PLAYER_ACCELERATION * deltaTime / 100;
		player_speed = player_speed >= PLAYER_MAX_SPEED ? PLAYER_MAX_SPEED : player_speed;
		move_button_hold_time += player_speed;
		if (move_button_hold_time / MAX_MOVE_BUTTON_HOLD_TIME > 1.0f)
		{
			move_button_hold_time = 0.0f;
			gameMaster->MovePlayerRight();
		}
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		player_speed += PLAYER_ACCELERATION * deltaTime / 100;
		player_speed = player_speed >= PLAYER_MAX_SPEED ? PLAYER_MAX_SPEED : player_speed;
		move_button_hold_time += player_speed;
		if (move_button_hold_time / MAX_MOVE_BUTTON_HOLD_TIME > 1.0f)
		{
			move_button_hold_time = 0.0f;
			gameMaster->MovePlayerLeft();
		}
	}
	if (!App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false) && !App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		move_button_hold_time = MAX_MOVE_BUTTON_HOLD_TIME + 1;
		player_speed = 0.0f;
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false) && App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		move_button_hold_time = 0.0f;
		player_speed = 0.0f;
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, false))
	{
		if (shooting_timer >= SHOOTING_TIMER)
		{
			shooting_timer = 0.0f;
			gameMaster->AddBullet(App::CreateSprite(".\\TestData\\bullet.bmp", 1, 1), 0);
		}
	}
	if (enemy_respawner / 2000 >= 1)
	{
		enemy_respawner = 0.0f;
		gameMaster->AddEnemy(App::CreateSprite(".\\TestData\\ships.bmp", 2, 12), 0);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		gameMaster->BuyLives();
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_X, true))
	{
		gameMaster->BuyNukes();
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_Y, true))
	{
		gameMaster->UseNukes();
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_BACK, true))
	{
		GameReset();
	}
	if (gameMaster->IsGameOver())
	{
		GameReset();
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	//static float a = 0.0f;
	//float r = 1.0f;
	//float g = 1.0f;
	//float b = 1.0f;
	//a += 0.1f;
	//for (int i = 0; i < 20; i++)
	//{

	//	float sx = 200 + sinf(a + i * 0.1f)*60.0f;
	//	float sy = 200 + cosf(a + i * 0.1f)*60.0f;
	//	float ex = 700 - sinf(a + i * 0.1f)*60.0f;
	//	float ey = 700 - cosf(a + i * 0.1f)*60.0f;
	//	g = (float)i / 20.0f;
	//	b = (float)i / 20.0f;
	//	App::DrawLine(sx, sy, ex, ey,r,g,b);
	//}

	std::string lives_str = "LIVES: " + std::to_string(gameMaster->GetLives()) + "/" + std::to_string(MAX_LIVES);
	char const* lives_char = lives_str.c_str();
	App::Print(20, APP_VIRTUAL_HEIGHT - 50, lives_char, 1.0f, 0.4f, 0.4f);
	std::string money_str = "MONEY: " + std::to_string(gameMaster->GetMoney()) + "/" + std::to_string(MAX_MONEY);
	char const* money_char = money_str.c_str();
	App::Print(20, APP_VIRTUAL_HEIGHT - 80, money_char, 0.22f, 0.85f, 0.04f);
	std::string nukes_str = "NUKES: " + std::to_string(gameMaster->GetNukes()) + "/" + std::to_string(MAX_NUKES);
	char const* nukes_char = nukes_str.c_str();
	App::Print(20, APP_VIRTUAL_HEIGHT - 110, nukes_char, 0.95f, 0.65f, 0.35f);
	std::string points_str = "POINTS: " + std::to_string(gameMaster->GetPoints());
	char const* points_char = points_str.c_str();
	App::Print(20, 110, points_char, 0.09f, 0.89f, 0.95f);

	gameMaster->Draw();

}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	GameMaster::ResetInstance();
}
