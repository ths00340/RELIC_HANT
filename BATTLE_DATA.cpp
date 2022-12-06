#include "BATTLE_DATA.h"
#include "GameObject.h"
#include "TimeStr.h"

void BATTLE_DATA::Uninit()
{
	if (Endurance != NULL)
	{
		delete Endurance;
		Endurance = NULL;
	}
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