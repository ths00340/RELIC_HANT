#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "Enemy.h"
#include "InfinityEnemySpone.h"
#include "Stage02.h"

void InfinityEnemySpone::Init()
{
	Scene* sce = Manager::GetScene();
	mtex = sce->AddGameObject<Stage02>((int)OBJ_LAYER::UI);
	EnemyNum = 100;
	for (int i = 0; i < EnemyNum; i++)
	{
		Enemy* en = NULL;
		en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
		en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
		en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(Leg_01::GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
	}
	Endurance = 60;
	ptime = 0.f;
	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance.Min, Endurance.Sec);
	Start = false;
}

void InfinityEnemySpone::Uninit()
{
}

void InfinityEnemySpone::Update()
{
	Scene* sce = Manager::GetScene();

	if (!Start)
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

	int num = sce->GetGameObjects<Enemy>((int)OBJ_LAYER::Enemy).size();
	if (num < EnemyNum)
	{
		for (int i = 0; i < EnemyNum - num; i++)
		{
			Enemy* en = NULL;
			en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
			en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
			en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(Leg_01::GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
		}
	}

	if (Start && !Clear && !GameOver)
		ptime += TOOL::SecDiv(1.0f);

	if (ptime > 1.f)
	{
		Endurance = Endurance - 1;
		ptime = ptime - 1.f;
		TOOL::Display((char*)"êßå¿éûä‘:%d/Hour %d/Min %d/Sec\n", Endurance.Hour, Endurance.Min, Endurance.Sec);
	}

	if (Endurance <= 0)
	{
		Clear = true;
	}
	if (!sce->GetGameObject<Player>())
	{
		GameOver = true;
	}
	timer->SetTime(Endurance.Min, Endurance.Sec);
}