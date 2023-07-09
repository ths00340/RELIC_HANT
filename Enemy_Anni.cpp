#include "manager.h"
#include "Scene.h"
#include "EnemyPool.h"
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
	m_pPool = sce->GetGameObject<EnemyPool>(OBJ_LAYER::System);
	m_pPool->Set(EnemyNum);

	for (int i = 0; i < 20; i++)
	{
		Enemy* en = m_pPool->Recycle();
		if (en)
		{
			en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
			Leg_01* _enLeg = en->LoadComponent<Leg_01>();
			Float2 _nPos = TOOL::rand2(i);

			en->SetPos(Float3((_nPos.x * 200.f) - 100.f, fabsf(_enLeg->GetModel()->Get_min().y * en->Getscl().y), (_nPos.y * 200.0f) - 100.f));
			en->LoadComponent<Status>()->SetMAX(20);
			en->AddComponent<TargetCom>();
		}
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