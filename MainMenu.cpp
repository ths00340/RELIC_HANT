#include "MainMenu.h"

///system
#include "manager.h"
#include "audio.h"
#include "Platform.h"
#include "input.h"
#include "Fade.h"

///Scene
#include "EnemyAnni.h"
#include "ENDURANCE.h"
#include "Boss01.h"
#include "Boss01_2.h"
#include "TextureDrawScene.h"
#include "DebugDrawScene.h"

///GameObject
#include "NumberManager.h"
#include "Camera_.h"
#include "MenuTitle.h"
#include "Blur_Tex.h"
#include "BulrS.h"
#include "Player.h"

//Component
#include "Status.h"
#include "cInputOperation.h"
#include "AttitudeControl.h"

NumberManager* a;

void MainMenu::Init()
{
	a = nullptr;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;
	Platform* abc;
	fade = nullptr;

	Manager::AddScene<TextureDrawScene>();

	menuCam = AddGameObject<Camera_>((int)OBJ_LAYER::System);
	abc = AddGameObject<Platform>((int)OBJ_LAYER::GameObject);
	abc->SetPos(TOOL::Uniform());
	abc->SetScl(TOOL::Uniform(0.25f));

	Player* pl = GetGameObject<Player>();
	pl = AddGameObject<Player>((int)OBJ_LAYER::GameObject);
	pl->RemoveComponent<Status>();
	pl->RemoveComponent<cInputOperation>();
	pl->RemoveComponent<AttitudeControl>();
	pl->DeleteDriveS();

	pl->SetScl(TOOL::Uniform(0.3f));
	pl->SetPos(Float3(5.f, 0.5f, 2.f));

	blur = AddGameObject<BulrS>((int)OBJ_LAYER::UI);

	title = AddGameObject<MenuTitle>((int)OBJ_LAYER::UI);
	title->Settype((int)MENU::STAGE);

	MissionNum = MISSION::PRACTICE;
#ifndef MUTE
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\Garage.wav");
	bgm->Play(true, 0.1f);
#endif // !MUTE
}

void MainMenu::Uninit()
{
	if (bgm)
	{
		bgm->StopAll();
		bgm->Destroy();
		bgm = nullptr;
	}

	Game::Uninit();
}

void MainMenu::Update()
{
	Scene::Update();

	int menutype = menuCam->Getmenu();

	if (menutype == (int)MENU::GARAGE)
	{
		blur->SetDraw(false);
		//装備変更

		if (GetGameObject<NumberManager>())
		{
			GetGameObject<NumberManager>()->SetDestroy();
			a = nullptr;
		}
	}
	if (menutype == (int)MENU::SHOP)
	{
		blur->SetDraw(false);
		//ショップ
	}
	if (menutype == (int)MENU::STAGE)
	{
		blur->SetDraw(true);
		if (!GetGameObject<NumberManager>())
		{
			a = AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
			a->SetStatus({ 300,300 }, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, true);
		}

		if (Input::GetKeyTrigger(DIK_W))
		{
			MissionNum = (MISSION)((int)MissionNum + 1);
		}

		if (Input::GetKeyTrigger(DIK_S))
		{
			MissionNum = (MISSION)((int)MissionNum - 1);
		}
		MissionNum = (MISSION)TOOL::Limit((int)MissionNum, (int)MISSION::BOSS_2, (int)MISSION::PRACTICE);
		a->SetScore((int)MissionNum + 1);
	}

	if (Manager::GetCommon() == NULL) {
		if (Input::GetKeyTrigger(DIK_E))
		{
			menuCam->Addmenu(1);
			title->Settype(menuCam->Getmenu());
		}

		if (Input::GetKeyTrigger(DIK_Q))
		{
			menuCam->Addmenu(-1);
			title->Settype(menuCam->Getmenu());
		}
	}

	{//シーン遷移系
		if (Input::GetKeyTrigger(DIK_SPACE) && menutype == (int)MENU::STAGE)
		{
			if (Manager::GetCommon() == NULL)
			{
				fade = Manager::SetCommon<Fade>();
				fade->Set(0.5f);
			}
		}

		MissionNum = (MISSION)TOOL::Limit((int)MissionNum, (int)MISSION::NONE - 1, (int)MISSION::PRACTICE);

		if (fade != NULL)
			if (fade->GetIn())
			{
				switch (MissionNum)
				{
				case MISSION::PRACTICE:
					Manager::SetScene<Game>();
					break;
				case MISSION::ENEMY_ANNI:
					Manager::SetScene<EnemyAnni>();
					break;
				case MISSION::ENDURA:
					Manager::SetScene<ENDURANCE>();
					break;
				case MISSION::BOSS_1:
					Manager::SetScene<Boss01>();
					break;
				case MISSION::BOSS_2:
					Manager::SetScene<Boss01_2>();
					break;
				}
				fade = NULL;
			}
	}
}