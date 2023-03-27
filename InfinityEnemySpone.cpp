#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "Enemy.h"
#include "InfinityEnemySpone.h"
#include "Stage02.h"
#include "TimeStr.h"
#include "Timer2D.h"
#include "Leg_01.h"

void InfinityEnemySpone::Init()
{
	BATTLE_DATA::Init();
	sce = Manager::GetScene();
	mtex = sce->AddGameObject<Stage02>((int)OBJ_LAYER::UI);

	EnemyNum = 100;
	for (int i = 0; i < EnemyNum; i++)
	{
		Enemy* en = NULL;
		en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
		en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
		Leg_01* _enLeg = en->LoadComponent<Leg_01>();
		en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(_enLeg->GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
	}
	Endurance = DBG_NEW TimeStr();
	*Endurance = 60;
	ptime = 0.f;
	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance->Min, Endurance->Sec);
	Start = false;
}

void InfinityEnemySpone::Begin()
{
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

void InfinityEnemySpone::ClearObserver()
{
	if (*Endurance <= 0)
	{
		Clear = true;
	}
	if (!sce->GetGameObject<Player>())
	{
		GameOver = true;
	}
}

void InfinityEnemySpone::ExtraMove()
{
	int num = sce->GetGameObjects<Enemy>((int)OBJ_LAYER::Enemy).size();
	if (num < EnemyNum)
	{
		for (int i = 0; i < EnemyNum - num; i++)
		{
			Enemy* en = NULL;
			en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
			en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
			Leg_01* _enLeg = en->LoadComponent<Leg_01>();
			en->SetPos(Float3((TOOL::RandF() * 100.f) - 50.f, fabsf(_enLeg->GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
		}
	}
}