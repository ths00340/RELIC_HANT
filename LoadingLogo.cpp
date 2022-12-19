#include "LoadingLogo.h"
#include "Scene.h"

#include <thread>

void LoadingLogo::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/charge_ore.png");
}

void LoadingLogo::Update()
{
}