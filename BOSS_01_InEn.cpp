#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "EnemyPool.h"
#include "Enemy.h"
#include "BOSS_01_InEn.h"
#include "TimeStr.h"
#include "Timer2D.h"
#include "ShotGun_Physics.h"
#include "Leg_01.h"
#include "TargetCom.h"
#include "MissionTex.h"

void BOSS_01_InEn::Init()
{
	BATTLE_DATA::Init();
	EnemyNum = 200;
	sce = Manager::GetScene();
	m_pPool = sce->GetGameObject<EnemyPool>(OBJ_LAYER::System);
	m_pPool->Set(EnemyNum + 1);

	Enemy* en = m_pPool->Recycle();
	mtex = sce->AddGameObject<MissionTex>((int)OBJ_LAYER::UI);
	mtex->LoadTex("asset/texture/MissTex04.png");

	if (en)
	{
		en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
		en->SetScl(TOOL::Uniform(1.f));
		en->LoadComponent<Status>()->SetMAX(300);
		en->SetShader(SHADER_S::LIGHT_LIM);
		en->AddComponent<TargetCom>();
		//en->AddComponent<Bazooka>()->SetDmg(5);
	}

	Endurance = DBG_NEW TimeStr();
	*Endurance = 300;
	ptime = 0.0f;
	Once = true;
	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance->Min, Endurance->Sec);
	Start = false;
}

void BOSS_01_InEn::Begin()
{
	//Å‰‚Ì“®‚«
	if (mtex->GetEnd())
	{
		sce->SetAllStop(false);
		Start = true;
		timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
		EnemyPool* enpool = sce->GetGameObject<EnemyPool>(OBJ_LAYER::System);

		for (int i = 0; i < EnemyNum; i++)
		{
			Enemy* en = m_pPool->Recycle();

			if (en)
			{
				en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
				Leg_01* leg = en->LoadComponent<Leg_01>();
				Float2 _nPos = TOOL::rand2(i);
				en->SetPos(Float3((_nPos.x * 200.f) - 100.f, fabsf(leg->GetModel()->Get_min().y * en->Getscl().y), (_nPos.y * 200.0f) - 100.f));
				en->LoadComponent<Status>()->SetMAX(5);
				if (i > EnemyNum - 20)
				{
					en->SetScl(TOOL::Uniform(0.25f));
					en->AddComponent<ShotGun_Physics>()->SetPredict();
				}
			}
		}
	}
	else
	{
		sce->SetAllStop();
		mtex->SetStop(false);
	}
}

void BOSS_01_InEn::ClearObserver()
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

void BOSS_01_InEn::ExtraMove()
{
	//“ÁŽêƒ‚[ƒVƒ‡ƒ“
	if (Once)
	{
		Once = false;
	}
	else
	{
		int num = sce->GetGameObjects<Enemy>((int)OBJ_LAYER::Enemy).size();
		if (num < EnemyNum)
		{
			for (int i = 0; i < EnemyNum - num; i++)
			{
				Enemy* en = m_pPool->Recycle();
				if (en)
				{
					en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
					Leg_01* leg = en->LoadComponent<Leg_01>();
					Float2 _nPos = TOOL::rand2(i);
					en->SetPos(Float3((_nPos.x * 200.f) - 100.f, fabsf(leg->GetModel()->Get_min().y * en->Getscl().y), (_nPos.y * 200.0f) - 100.f));
					en->LoadComponent<Status>()->SetMAX(5);
				}
			}
		}
	}
}