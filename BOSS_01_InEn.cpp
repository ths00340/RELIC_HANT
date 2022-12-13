#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "Enemy.h"
#include "BOSS_01_InEn.h"
#include "Stage03.h"
#include "TimeStr.h"
#include "Timer2D.h"

void BOSS_01_InEn::Init()
{
	sce = Manager::GetScene();
	Enemy* en = NULL;
	mtex = sce->AddGameObject<Stage03>((int)OBJ_LAYER::UI);
	en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
	en->SetScl(TOOL::Uniform(1.f));
	en->LoadComponent<Status>()->SetMAX(300);
	//en->AddComponent<Bazooka>()->SetDmg(5);
	Target = en;
	Endurance = new TimeStr();
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
	}
	else
	{
		sce->SetAllStop();
		mtex->SetStop(false);
	}
}

void BOSS_01_InEn::ClearObserver()
{
	if (!Manager::GetScene()->GetGameObject<Player>() || Endurance <= 0)
	{
		GameOver = true;
	}
	if (!sce->GetLiveObj(Target, (int)OBJ_LAYER::Enemy))
	{
		Clear = true;
	}
}

void BOSS_01_InEn::ExtraMove()
{
	//“ÁŽêƒ‚[ƒVƒ‡ƒ“
	if (Once)
	{
		if (Target->Getvec().y == 0.f)
		{
			Once = false;

			EnemyNum = 200;
			for (int i = 0; i < EnemyNum; i++)
			{
				Enemy* en = NULL;
				en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
				en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
				en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(Leg_01::GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
				en->LoadComponent<Status>()->SetMAX(5);
				if (i > EnemyNum - 20)
				{
					en->SetScl(TOOL::Uniform(0.25f));
					en->AddComponent<ShotGun_Physics>();
				}
			}
		}
	}
	else
	{
		int num = sce->GetList((int)OBJ_LAYER::Enemy).size();
		if (num < EnemyNum)
		{
			for (int i = 0; i < EnemyNum - num; i++)
			{
				Enemy* en = NULL;
				en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
				en->SetScl(TOOL::Uniform(TOOL::RandF() * 0.5 + 0.25f));
				en->SetPos(Float3((TOOL::RandF() * 200.f) - 100.f, fabsf(Leg_01::GetModel()->Get_min().y * en->Getscl().y), (TOOL::RandF() * 200.0f) - 100.f));
				en->LoadComponent<Status>()->SetMAX(5);
			}
		}
	}
}