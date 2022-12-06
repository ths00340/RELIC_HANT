#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "Enemy.h"
#include "BOSS_01.h"
#include "Stage03.h"
#include "TimeStr.h"
#include "Timer2D.h"

void BOSS_01::Init()
{
	Scene* sce = Manager::GetScene();
	Enemy* en = NULL;
	mtex = sce->AddGameObject<Stage03>((int)OBJ_LAYER::UI);
	en = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
	en->SetScl(TOOL::Uniform(1.f));
	en->LoadComponent<Status>()->SetMAX(500);
	en->AddComponent<ShotGun_Physics>();
	Target = en;
	Endurance = new TimeStr();
	*Endurance = 300;
	ptime = 0.0f;

	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance->Min, Endurance->Sec);
	Start = false;
}

void BOSS_01::Update()
{
	Scene* sce = Manager::GetScene();

	//Å‰‚Ì“®‚«
	if (!Start)
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

	//ìí‚Ì‰Â”Û”»’è
	{
		if (!sce->GetLiveObj(Target, (int)OBJ_LAYER::Enemy))
		{
			Clear = true;
		}
		if (!Manager::GetScene()->GetGameObject<Player>() || Endurance <= 0)
		{
			GameOver = true;
		}
	}

	//§ŒÀŽžŠÔ
	{
		if (Start && !Clear && !GameOver)
			ptime += TOOL::SecDiv(1.0f);

		if (ptime > 1.f)
		{
			*Endurance -= 1;
			ptime = ptime - 1.f;
			//TOOL::Display((char*)"§ŒÀŽžŠÔ:%d/Hour %d/Min %d/Sec\n", Endurance->Hour, Endurance->Min, Endurance->Sec);
		}
		timer->SetTime(Endurance->Min, Endurance->Sec);
	}
}