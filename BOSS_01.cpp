#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "EnemyPool.h"
#include "Enemy.h"
#include "BOSS_01.h"
#include "Stage03.h"
#include "TimeStr.h"
#include "Timer2D.h"
#include "ShotGun_Physics.h"
#include "TargetCom.h"

void BOSS_01::Init()
{
	BATTLE_DATA::Init();

	//現在のシーンの登録
	sce = Manager::GetScene();

	//指令書の登録
	mtex = sce->AddGameObject<Stage03>((int)OBJ_LAYER::UI);

	m_pPool = sce->GetGameObject<EnemyPool>(OBJ_LAYER::System);
	m_pPool->Set(1);

	//ボスエネミーの設定
	{
		Enemy* en = NULL;
		en = m_pPool->Recycle();
		if (en)
		{
			en->SetScl(TOOL::Uniform(1.f));
			en->LoadComponent<Status>()->SetMAX(500);
			en->AddComponent<ShotGun_Physics>();
			en->SetShader(SHADER_S::LIGHT_LIM);
			en->AddComponent<TargetCom>();
		}
	}

	//時間管理オブジェクトの生成
	Endurance = DBG_NEW TimeStr();
	*Endurance = 300;//制限時間を300秒に設定
	ptime = 0.0f;

	//時間表示用オブジェクトを生成
	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance->Min, Endurance->Sec);

	//スタート用関数をfalseに
	Start = false;
}

void BOSS_01::Begin()
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

void BOSS_01::ClearObserver()
{
	if (TargetList->Empty())
	{
		Clear = true;
	}

	if (!Manager::GetScene()->GetGameObject<Player>() || Endurance <= 0)
	{
		GameOver = true;
	}
}