#include "manager.h"
#include "audio.h"
#include "InfinityEnemySpone.h"
#include "ENDURANCE.h"
#include "Tree.h"
#include "SphereShadow.h"

void ENDURANCE::Init()
{
	Game::Init();
	SetBattle<InfinityEnemySpone>();
	for (int i = 0; i < 250; i++)
	{
		Tree* t = nullptr;
		t = AddGameObject<Tree>((int)OBJ_LAYER::Billboard);
		t->LoadScl()->y = TOOL::RandF() * 10.0f + 8.0f;
		Float2 _nPos = TOOL::rand2(i);

		t->Set(Float3((_nPos.x * 400.0f) - 200.0f, t->Getscl().y - 1.0f, (_nPos.y * 400.0f) - 200.0f));
	}
}

void ENDURANCE::NextScene()
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

void ENDURANCE::BGMSelect()
{
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\Enemy.wav");
	bgm->Play(true, 0.1f);
}