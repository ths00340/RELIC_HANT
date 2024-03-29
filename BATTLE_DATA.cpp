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

	ExtraMove();//独自の挙動

	ClearObserver();//クリア判定

	//制限時間があるなら
	if (timer)
	{
		//時間の終了判定
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