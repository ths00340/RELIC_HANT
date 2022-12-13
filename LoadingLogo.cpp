#include "LoadingLogo.h"
#include "Scene.h"

#include <thread>

void LoadingLogo::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/charge_ore.png");

	std::thread th(Scene::Loads);//別スレッドへ登録
	th.detach();//登録した変数を起動
}

void LoadingLogo::Uninit()
{
}

void LoadingLogo::Update()
{
}