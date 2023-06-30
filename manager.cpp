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
#include "TextureDrawScene.h"

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
	Audio::InitMaster();
	common = nullptr;
	Input::ShowPoint(true);

	srand((unsigned int)timeGetTime());
	SetScene<Logo_S>();
	legtype = MOVE_TYPE::CAR;
	wepon = WEPON_TYPE::SHOTGUN_1;
}

void Manager::Uninit()
{
	NowScene->Uninit();
	DeleteAddScenes();
	Audio::UninitMaster();
	delete NowScene;
	ResourceManager::Uninit();
	Scene::UnLoads();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Renderer::Update();
	Input::Update();

	NowScene->Update();
	for (Scene* sce : addScene)
	{
		sce->Update();
	}

	if (common != nullptr)
	{
		common->Update();
		if (common->GetDestroy())
		{
			common->Uninit();
			delete common;
			common = nullptr;
		}
	}
}

void Manager::Draw()
{
	Renderer::GetDeviceContext()->RSSetViewports(1, NowScene->GetView());

	Float3 SunPos = { -10.f,10.f,-10.f };

	LIGHT light;
	light.Enable = true;
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.f,1.f,1.f, 1.0f);
	light.Position = { SunPos.x,SunPos.y,SunPos.z,0.f };
	Float3 dir = Float3(0.f, 0.f, 0.f) - Float3(light.Position.x, light.Position.y, light.Position.z);
	light.Direction = D3DXVECTOR4(dir.x, dir.y, dir.z, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);

	D3DXVECTOR3 eye, up, at;
	eye = SunPos;
	at = { 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };

	D3DXMatrixLookAtLH(&light.ViewMatrix, &eye, &at, &up);
	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 1000.0f);

	//ライト情報をセット
	Renderer::SetLight(light);
	//*******１パス目 シャドウバッファの作成*******
	Renderer::BeginDepth();

	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	NowScene->ObjectDraws(OBJ_LAYER::GameObject, OBJ_LAYER::Billboard);

	Renderer::SetLight(light);
	Renderer::BeginTexture();
	//レンダーテクスチャシーン描画//マルチレンダーになるかも…
	NowScene->ObjectDraws(OBJ_LAYER::System, OBJ_LAYER::Billboard);

	Renderer::SetLight(light);
	Renderer::Begin();
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