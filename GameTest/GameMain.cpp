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
float enemy_spawner;
float bonus_spawner;
float game_over_text_time;

float max_shooting_timer;
float max_enemy_spawner;

bool max_level;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	gameMaster = nullptr;
	max_shooting_timer = SHOOTING_TIMER_LV1;
	max_enemy_spawner = ENEMY_SPAWNER_LV1;
	move_button_hold_time = MAX_MOVE_BUTTON_HOLD_TIME + 1;
	shooting_timer = max_shooting_timer + 1;
	player_speed = 0.0f;
	enemy_spawner = 0.0f;
	bonus_spawner = 0.0f;
	game_over_text_time = 0.0f;
	max_level = false;
	gameMaster = GameMaster::GetInstance();
	gameMaster->CreateMap(30);
	gameMaster->SetPlayer(App::CreateSprite(".\\MainData\\ships2.bmp", 1, 3), 0);
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
	enemy_spawner += deltaTime;
	bonus_spawner += deltaTime;
	shooting_timer += deltaTime;
	shooting_timer = shooting_timer > max_shooting_timer ? max_shooting_timer + 1 : shooting_timer;
	gameMaster->Update(deltaTime);
	if (!gameMaster->IsGameOver())
	{
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
			if (shooting_timer >= max_shooting_timer)
			{
				shooting_timer = 0.0f;
				gameMaster->AddBullet(App::CreateSprite(".\\MainData\\bullet.bmp", 1, 1), 0);
			}
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

		if (enemy_spawner / max_enemy_spawner >= 1)
		{
			enemy_spawner = 0.0f;
			gameMaster->AddEnemy(App::CreateSprite(".\\MainData\\ships.bmp", 2, 12), 0);
		}
		if (bonus_spawner / BONUS_SPAWNER >= 1)
		{
			bonus_spawner = 0.0f;
			gameMaster->AddBonus(App::CreateSprite(".\\MainData\\ships.bmp", 2, 12), 2);
		}
		if (gameMaster->GetPoints() >= LEVEL_2_POINTS && gameMaster->GetPoints() < LEVEL_3_POINTS)
		{
			gameMaster->FirstUpgradePlayer();
			max_shooting_timer = SHOOTING_TIMER_LV2;
			max_enemy_spawner = ENEMY_SPAWNER_LV2;
		}
		else if (gameMaster->GetPoints() >= LEVEL_3_POINTS && !max_level)
		{
			max_level = true;
			gameMaster->SecondUpgradePlayer();
			max_shooting_timer = SHOOTING_TIMER_LV3;
			max_enemy_spawner = ENEMY_SPAWNER_LV3;
		}
	}
	else
	{
		game_over_text_time += deltaTime;
		if (game_over_text_time > GAME_OVER_TEXT_TIME)
		{
			GameReset();
		}
	}
}

void PrintString(float x, float y, std::string st, float r, float g, float b)
{
	char const* st_char = st.c_str();
	App::Print(x, y, st_char, r, g, b);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	PrintString(20, APP_VIRTUAL_HEIGHT - 50, "LIVES: " + std::to_string(gameMaster->GetLives()) + "/" + std::to_string(gameMaster->GetMaxLives()), 1.0f, 0.4f, 0.4f);
	PrintString(20, APP_VIRTUAL_HEIGHT - 80, "MONEY: " + std::to_string(gameMaster->GetMoney()) + "/" + std::to_string(gameMaster->GetMaxMoney()), 0.22f, 0.85f, 0.04f);
	PrintString(20, APP_VIRTUAL_HEIGHT - 110, "NUKES: " + std::to_string(gameMaster->GetNukes()) + "/" + std::to_string(gameMaster->GetMaxNukes()), 0.95f, 0.65f, 0.35f);
	PrintString(20, 110, "POINTS: " + std::to_string(gameMaster->GetPoints()), 0.09f, 0.89f, 0.95f);

	PrintString(20, APP_VIRTUAL_HEIGHT - 200, std::to_string(LIFE_PRICE) + " LIFE PRICE", 0.79f, 0.51f, 0.95f);
	PrintString(20, APP_VIRTUAL_HEIGHT - 230, std::to_string(NUKE_PRICE) + " NUKE PRICE", 0.79f, 0.51f, 0.95f);

	if (game_over_text_time > 0.0f)
	{
		App::Print(20, APP_VIRTUAL_HEIGHT / 2, "GAME OVER :(", 1.0f, 1.0f, 1.0f);
	}

	if (gameMaster->IsBonus())
	{
		App::Print(APP_VIRTUAL_WIDTH - 100, APP_VIRTUAL_HEIGHT / 2, "2XP", 1.0f, 1.0f, 1.0f);
	}

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
