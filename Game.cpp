#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Game.h"
#include "audio.h"
#include "GameObject.h"
#include "Common.h"
#include "Camera.h"
#include "ViewCamera.h"
#include "Field.h"

void Game::Init()
{
	Next = false;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	////カメラ→3D→2Dの流れ
	VCam = AddGameObject<ViewCamera>((int)OBJ_LAYER::System);
	AddGameObject<Field>((int)OBJ_LAYER::NoCaring);
	AddGameObject<Skybox>((int)OBJ_LAYER::NoCaring);
	AddGameObject<Player>((int)OBJ_LAYER::GameObject);

	for (int i = 0; i < 20; i++)
	{
		AddGameObject<Tree>((int)OBJ_LAYER::Billboard)->Set(Float3((TOOL::RandF() * 300.0f) - 150.0f, 7.0f, (TOOL::RandF() * 300.0f) - 150.0f));
	}

#ifndef MUTE
	class Audio* bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\gameover.wav");
	bgm->Play(true, 0.1f);
#endif // MUTE

	//AddGameObject<Polygon2D>(3);装備
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

void Game::Draw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };


	for (int i = 0; i < 2; i++)
		for (GameObject* object : g_GameObject[i])
		{
			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}

	for (int i = 2; i < LAYER_NUM-1; i++)
	{
		for (GameObject* object : g_GameObject[i])//範囲forループ
		{
			if (VCam != nullptr)
				if (!VCam->CheckView(object->Getpos()))
					continue;

			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}

	for (GameObject* object : g_GameObject[LAYER_NUM-1])//範囲forループ
	{
		if (object->GetBlendState() != NULL)
			Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

		object->Draw();

		for (CComponent* com : object->GetComponent())
			com->Draw();
	}

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

void Game::DebugDraw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < LAYER_NUM; i++)
	{
		for (GameObject* object : g_GameObject[i])//範囲forループ
		{
			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}
}