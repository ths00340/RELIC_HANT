#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "Polygon2D.h"
#include "input.h"
#include "Fade.h"
#include "Result_S.h"
#include "ResultBackGrownd.h"
#include "Blinking_Space.h"
#include "TextureDrawScene.h"

void Result_S::Init()
{
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	fade = NULL;

	Manager::AddScene<TextureDrawScene>();

	//Input::SetPause(true);
	//Input::ShowPoint(true);
	AddGameObject<ResultBackGrownd>((int)OBJ_LAYER::UI);
	AddGameObject<Blinking_Space>((int)OBJ_LAYER::UI);
#ifndef MUTE
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\ClearBGM.wav");
	bgm->Play(true, 0.1f);
#endif // !MUTE
}

void Result_S::Uninit()
{
	if (bgm != NULL)
		bgm->StopAll();

	Scene::Uninit();
}

void Result_S::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_SPACE))
		Next = true;

	if (Next)
	{
		Next = false;
		if (Manager::GetCommon() == NULL)
		{
			fade = Manager::SetCommon<Fade>();
			fade->Set(0.5f);
		}
	}

	if (fade != NULL)
		if (fade->GetIn())
		{
			Manager::SetScene<MainMenu>();
			fade = NULL;
		}
}