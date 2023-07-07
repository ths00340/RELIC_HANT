#include "manager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Enemy_Anni.h"
#include "Stage01.h"
#include "Leg_01.h"
#include "TargetCom.h"

void Enemy_Anni::Init()
{
	BATTLE_DATA::Init();
	sce = Manager::GetScene();
	mtex = sce->AddGameObject<Stage01>((int)OBJ_LAYER::UI);
	EnemyNum = 20;
	for (int i = 0; i < 20 - 1; i++)
	{
		Enemy* en = NULL;
		en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
		en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
		Leg_01* _enLeg = en->LoadComponent<Leg_01>();
		en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(_enLeg->GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
		en->LoadComponent<Status>()->SetMAX(20);
		en->AddComponent<TargetCom>();
	}
	Start = false;
}

void Enemy_Anni::Begin()
{
	//‰‰o‚ÌI—¹”»•Êˆ—
	if (mtex->GetEnd())
	{
		sce->SetAllStop(false);
		Start = true;
	}
	else
	{
		sce->SetAllStop();
		mtex->SetStop(false);
	}
}

void Enemy_Anni::ClearObserver()
{
	TOOL::Display("EnemyNum:%d\n", TargetList->GetListCount());

	if (TargetList->Empty())
	{
		Clear = true;
	}
	if (!sce->GetGameObject<Player>())
	{
		GameOver = true;
	}
}