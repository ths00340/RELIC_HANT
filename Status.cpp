#include "main.h"
#include "input.h"
#include "Tools.h"
#include "GameObject.h"
#include "Status.h"

void Status::Init()
{
}

void Status::Uninit()
{
}

void Status::Update()
{
	HP = max(HP, 0);
	St = max(St, 0);

	if (AutoBreak)
	{
		if (HP <= 0)
		{
			TOOL::Display((char*)"HP‘S‘¹F%s\n", object->GetName());
			object->Finish();
			return;
		}
	}

	if (AutoHeal)
	{
		heal += TOOL::SecDiv(HP_MAX) / HealTime;
		if (heal >= 1.f)
		{
			HP += 1;
			heal -= 1.f;
		}
	}

	if (AutoStHeal)
		St += TOOL::SecDiv(St_MAX) / StHealTime;

	HP = min(HP, HP_MAX);
	St = min(St, St_MAX);

	wheel = min(wheel, 0);
}

void Status::Draw()
{
}