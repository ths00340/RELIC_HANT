#include "manager.h"
#include "audio.h"
#include "BOSS_01_InEn.h"
#include "Boss01_2.h"
#include "Tree.h"
#include "SphereShadow.h"

void Boss01_2::Init()
{
	Game::Init();
	SetBattle<BOSS_01_InEn>();
	for (int i = 0; i < 500; i++)
	{
		Tree* t = NULL;
		t = AddGameObject<Tree>((int)OBJ_LAYER::Billboard);
		t->LoadScl()->y = TOOL::RandF() * 10.0f + 8.0f;
		t->Set(Float3((TOOL::RandF() * 400.0f) - 200.0f, t->Getscl().y - 1.0f, (TOOL::RandF() * 400.0f) - 200.0f));
		t->AddComponent<SphereShadow>();
	}
}

void Boss01_2::NextScene()
{
	if (B_Data)
	{
		if (B_Data->GetClear())
			Manager::SetScene<Result_S>();//ゲームクリア
		else
			Manager::SetScene<MainMenu>();//ゲームオーバー
	}
	else
		Manager::SetScene<Result_S>();
	fade = NULL;
}

void Boss01_2::BGMSelect()
{
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\Boss.wav");
	bgm->Play(true, 0.1f);
}