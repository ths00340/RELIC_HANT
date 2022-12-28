#include "BATTLE_DATA.h"
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
}

void BATTLE_DATA::Update()
{
	if (!Start)
	{
		Begin();
	}

	ExtraMove();

	ClearObserver();

	if (timer)
	{
		IsTimeLimit();
	}
}

void BATTLE_DATA::IsTimeLimit()
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

GameObject* BATTLE_DATA::GetTarget()
{
	if (Target)
		return Target;
	else
		return nullptr;
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