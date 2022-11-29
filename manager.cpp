#include "main.h"
#include "manager.h"
#include "renderer.h"
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

nametype Manager::Modelname;
std::vector<Model*> Manager::models;

nametype Manager::TextureName;
std::vector<ID3D11ShaderResourceView*> Manager::Texture;

ID3D11BlendState* Manager::B_State[(int)BLEND_S::BLEND_E];
ID3D11RasterizerState* Manager::rs[(int)FRAME_S::FRAME_E];
ID3D11VertexShader* Manager::m_VertexShader[(int)SHADER_S::SHADER_E];
ID3D11PixelShader* Manager::m_PixelShader[(int)SHADER_S::SHADER_E];
ID3D11InputLayout* Manager::m_VertexLayout[(int)SHADER_S::SHADER_E];
MOVE_TYPE	Manager::legtype;
WEPON_TYPE	Manager::wepon;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	StartUp();
	Scene::Loads();
	Audio::InitMaster();
	common = NULL;
	Input::SetPause(true);
	Input::ShowPoint(true);

	srand((unsigned int)timeGetTime());
	SetScene<Logo_S>();
	legtype = MOVE_TYPE::CAR;
	wepon = WEPON_TYPE::GATLING;
}

void Manager::Uninit()
{
	NowScene->Uninit();
	Audio::UninitMaster();
	delete NowScene;
	DeleteModels();
	ReleaseState();
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