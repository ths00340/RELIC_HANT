#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "Game.h"
#include "Polygon2D.h"
#include "Field.h"
#include "Camera.h"
#include "input.h"
#include "model.h"
#include "Player.h"
#include "Enemy.h"
#include "bullet.h"
#include "GameObject.h"
#include "Skybox.h"
#include "ViewCamera.h"
#include "Common.h"

void Game::Init()
{
	Next = false;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	////ÉJÉÅÉâÅ®3DÅ®2DÇÃó¨ÇÍ
	AddGameObject<ViewCamera>((int)OBJ_LAYER::System);
	AddGameObject<Skybox>((int)OBJ_LAYER::GameObject);
	AddGameObject<Field>((int)OBJ_LAYER::GameObject);
	AddGameObject<Player>((int)OBJ_LAYER::GameObject);

	for (int i = 0; i < 20; i++)
	{
		AddGameObject<Tree>((int)OBJ_LAYER::Billboard)->Set(Float3((TOOL::RandF() * 300.0f) - 150.0f, 7.0f, (TOOL::RandF() * 300.0f) - 150.0f));
	}

	class Audio* bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\gameover.wav");
	bgm->Play(true, 0.1f);

	//AddGameObject<Polygon2D>(3);ëïîı
}

void Game::Uninit()
{
	if (bgm != NULL)
		bgm->StopAll();

	if (B_Data)
		B_Data->Uninit();
	Scene::Uninit();
}

void Game::Update()
{
	Scene::Update();
	if (B_Data)
		B_Data->Update();
}

void Game::SetPlayer(Player* player)
{
	if (!pl)
	{
		pl = player;
		g_GameObject[(int)OBJ_LAYER::GameObject].push_back(player);
		GetGameObject<ViewCamera>()->SetView(player->LoadComponent<Camera>());
	}
}