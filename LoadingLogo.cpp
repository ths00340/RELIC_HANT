#include "LoadingLogo.h"
#include "Scene.h"

#include <thread>

void LoadingLogo::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/charge_ore.png");

	std::thread th(Scene::Loads);//•ÊƒXƒŒƒbƒh‚Ö“o˜^
	th.detach();//“o˜^‚µ‚½•Ï”‚ğ‹N“®
}

void LoadingLogo::Uninit()
{
}

void LoadingLogo::Update()
{
}