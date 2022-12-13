#include "LoadingLogo.h"
#include "Scene.h"

#include <thread>

void LoadingLogo::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/charge_ore.png");

	std::thread th(Scene::Loads);//�ʃX���b�h�֓o�^
	th.detach();//�o�^�����ϐ����N��
}

void LoadingLogo::Uninit()
{
}

void LoadingLogo::Update()
{
}