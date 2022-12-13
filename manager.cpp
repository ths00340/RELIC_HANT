#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "input.h"
#include <time.h>
#include "Game.h"
#include "Logo_S.h"
#include "Common.h"
#include "audio.h"
#include "Title.h"

Scene* Manager::NowScene;
Common* Manager::common;
std::list<Scene*> Manager::addScene;

MOVE_TYPE	Manager::legtype;
WEPON_TYPE	Manager::wepon;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	ResourceManager::Init();
	Scene::Loads();
	Audio::InitMaster();
	common = NULL;
	Input::SetPause(true);
	Input::ShowPoint(true);

	srand((unsigned int)timeGetTime());
	SetScene<Logo_S>();
	legtype = MOVE_TYPE::FREE;
	wepon = WEPON_TYPE::BAZOOKA;
}

void Manager::Uninit()
{
	NowScene->Uninit();
	Audio::UninitMaster();
	delete NowScene;
	ResourceManager::Uninit();
	Scene::UnLoads();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	NowScene->Update();
	for (Scene* sce : addScene)
	{
		sce->Update();
	}

	if (common != NULL)
	{
		common->Update();
		if (common->GetDestroy())
		{
			common->Uninit();
			delete common;
			common = NULL;
		}
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	Renderer::GetDeviceContext()->RSSetViewports(1, NowScene->GetView());
	NowScene->Draw();
	for (Scene* sce : addScene)
	{
		Renderer::GetDeviceContext()->RSSetViewports(1, sce->GetView());
		sce->Draw();
	}
	if (common != NULL)
	{
		Renderer::GetDeviceContext()->RSSetViewports(1, common->GetView());
		common->Draw();
	}

	Renderer::End();
}