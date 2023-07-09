#include "BATTLE_DATA.h"
#include "renderer.h"
#include "GameObject.h"
#include "TimeStr.h"
#include "Timer2D.h"
#include "Manager.h"

void BATTLE_DATA::Uninit()
{
	if (Endurance != NULL)
	{
		delete Endurance;
		Endurance = NULL;
	}
	if (TargetList != nullptr)
	{
		delete TargetList;
		TargetList = nullptr;
	}
}

void BATTLE_DATA::Update()
{
	if (!Start)
	{
		Begin();
		return;
	}

	ExtraMove();//“ÆŽ©‚Ì‹““®

	ClearObserver();//ƒNƒŠƒA”»’è

	//§ŒÀŽžŠÔ‚ª‚ ‚é‚È‚ç
	if (timer)
	{
		//ŽžŠÔ‚ÌI—¹”»’è
		IsTimeLimit();
	}
}

void BATTLE_DATA::IsTimeLimit()
{
	if (!Clear && !GameOver)
		ptime += Renderer::GetDeltaTime();

	if (ptime > 1.f)
	{
		*Endurance -= 1;
		ptime = ptime - 1.f;
	}
	timer->SetTime(Endurance->Min, Endurance->Sec);
}

TimeStr* BATTLE_DATA::GetTime()
{
	return Endurance;
}

void BATTLE_DATA::AddTime(int sec)
{
	*Endurance + sec;
}

void BATTLE_DATA::SetTime(int sec)
{
	*Endurance = sec;
}