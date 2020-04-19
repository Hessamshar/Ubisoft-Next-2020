//------------------------------------------------------------------------
// GameTest.cpp
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
float button_hold_time = MAX_HOLD_TIME + 1;
float player_speed = 0.0f;
float enemy_respawner = 0.0f;

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
	gameMaster = GameMaster::getInstance();
	gameMaster->CreateMap(30);
	gameMaster->SetPlayerSprite(App::CreateSprite(".\\TestData\\ships2.bmp", 1, 3), 0);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	enemy_respawner += deltaTime;
	gameMaster->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		//testSprite->SetAnimation(ANIM_RIGHT);
		//float x, y;
		//testSprite->GetPosition(x, y);
		//x += 1.0f;
		//testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		//testSprite->SetAnimation(ANIM_LEFT);
		//float x, y;
		//testSprite->GetPosition(x, y);
		//x -= 1.0f;
		//testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		//testSprite->SetAnimation(ANIM_FORWARDS);
		//float x, y;
		//testSprite->GetPosition(x, y);
		//y += 1.0f;
		//testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		//testSprite->SetAnimation(ANIM_BACKWARDS);
		//float x, y;
		//testSprite->GetPosition(x, y);
		//y -= 1.0f;
		//testSprite->SetPosition(x, y);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		//testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		//testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		player_speed += PLAYER_ACCELERATION * deltaTime / 100;
		player_speed = player_speed >= PLAYER_MAX_SPEED ? PLAYER_MAX_SPEED : player_speed;
		button_hold_time += player_speed;
		if (button_hold_time / MAX_HOLD_TIME > 1.0f)
		{
			button_hold_time = 0.0f;
			gameMaster->MovePlayerRight();
		}
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		player_speed += PLAYER_ACCELERATION * deltaTime / 100;
		player_speed = player_speed >= PLAYER_MAX_SPEED ? PLAYER_MAX_SPEED : player_speed;
		button_hold_time += player_speed;
		if (button_hold_time / MAX_HOLD_TIME > 1.0f)
		{
			button_hold_time = 0.0f;
			gameMaster->MovePlayerLeft();
		}
		//testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (!App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false) && !App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		button_hold_time = MAX_HOLD_TIME + 1;
		player_speed = 0.0f;
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false) && App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		button_hold_time = 0.0f;
		player_speed = 0.0f;
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		//testSprite->SetAnimation(-1);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		//testSprite->SetVertex(0, testSprite->GetVertex(0) + 5.0f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
	if (App::GetController().CheckButton('a', true))
	{
		gameMaster->AddBullet(App::CreateSprite(".\\TestData\\bullet.bmp", 1, 1), 0);
	}
	if (enemy_respawner / 3000 >= 1)
	{
		enemy_respawner = 0.0f;
		gameMaster->AddEnemy(App::CreateSprite(".\\TestData\\ships.bmp", 2, 12), 0);
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

	//const std::string s = std::to_string(bullet->getX());
	//char const* pchar = s.c_str();
	//App::Print(100, 100, pchar);
	gameMaster->Draw();

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	delete gameMaster;
}