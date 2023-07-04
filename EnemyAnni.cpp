#include "manager.h"
#include "audio.h"
#include "Enemy_Anni.h"
#include "EnemyAnni.h"
#include "Tree.h"
#include "SphereShadow.h"
#include "BillBoadManager.h"

void EnemyAnni::Init()
{
	Game::Init();
	SetBattle<Enemy_Anni>();

	BillBoadManager* _billM = AddGameObject<BillBoadManager>((int)OBJ_LAYER::NoCaring);
	_billM->SetTexture(ResourceManager::AddTex("asset/texture/Tree01.png"));
	for (int i = 0; i < 250; i++)
	{
		float sizeY = TOOL::RandF() * 10.0f + 8.0f;
		_billM->Set(
			Float3((TOOL::RandF() * 400.0f) - 200.0f
				, sizeY - 1.0f
				, (TOOL::RandF() * 400.0f) - 200.0f)
			, Float3(8.f, sizeY, 8.f));
	}
}

void EnemyAnni::NextScene()
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

void EnemyAnni::BGMSelect()
{
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\Enemy.wav");
	bgm->Play(true, 0.1f);
}