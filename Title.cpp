#include "MainMenu.h"
#include "Camera.h"
#include "Skybox.h"
#include "Polygon2D.h"
#include "Field.h"
#include "audio.h"
#include "ViewCamera.h"
#include "DebugCamMove.h"
#include "input.h"
#include "Common.h"
#include "Fade.h"
#include "Title.h"
#include "Gravity.h"
#include "Leg_01.h"
#include "Title2D.h"
#include "Dodai.h"
#include "Blinking_Space.h"
#include "Enemy.h"
#include "TextureDrawScene.h"

void Title::Init()
{
	Manager::AddScene<TextureDrawScene>();
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;
	VCam = AddGameObject<ViewCamera>((int)OBJ_LAYER::System);
	AddGameObject<Skybox>((int)OBJ_LAYER::GameObject);
	//AddGameObject<Field>((int)OBJ_LAYER::GameObject)->SetPos({0.0f,-0.3f,0.0f});
	dodai = AddGameObject<Dodai>((int)OBJ_LAYER::GameObject);
	dodai->SetPos({ 0.f,0.08f,3.f });

	enemy = AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
	enemy->SetPos({ 0.f,0.3f,3.f });
	enemy->RemoveComponent<Gravity>();

	AddGameObject<Title2D>((int)OBJ_LAYER::UI);
	Manager::GetScene()->AddGameObject<Blinking_Space>((int)OBJ_LAYER::UI);
	VCam->GetView()->SetTag({ 0.0f,2.0f,3.0f });
#ifndef MUTE
	BGM = AddGameObject<Audio>((int)OBJ_LAYER::System);
	BGM->Load("asset\\BGM\\Title.wav");
	BGM->Play(true, 0.1f);
#endif // !MUTE
}

void Title::Uninit()
{
	Game::Uninit();
}

void Title::Update()
{
	enemy->LoadRot()->y += TOOL::AToR(0.5f);

	if (enemy->LoadComponent<Leg_01>())
		enemy->LoadComponent<Leg_01>()->AddMove(TOOL::AToR(10));

	dodai->LoadRot()->y += TOOL::AToR(0.5f);

	Game::Update();

	{//ÉVÅ[ÉìëJà⁄ån
		if (Input::GetKeyTrigger(DIK_SPACE))
		{
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
}