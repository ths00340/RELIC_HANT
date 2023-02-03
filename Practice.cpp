#include "Tools.h"
#include "manager.h"
#include "input.h"
#include "Scene.h"
#include "Practice.h"
#include "SousaH.h"
#include "Blinking_Space.h"
#include "MissionTex.h"
#include "Status.h"
#include "Player.h"

void Practice::Init()
{
	sce = Manager::GetScene();
	Once = true;
	Clear = false;
	Manager::GetScene()->AddGameObject<SousaH>((int)OBJ_LAYER::UI);
	Start = false;
}

void Practice::Begin()
{
	if (Once)
	{
		Player* obj;
		obj = Manager::GetScene()->GetGameObject<Player>();
		obj->LoadComponent<Status>()->SetMAX(100);
		obj->LoadComponent<Status>()->SetAutoHeal_HP(true, 5.0f);
		obj->LoadComponent<Status>()->SetBreak();
		Once = false;
		Manager::GetScene()->AddGameObject<Blinking_Space>((int)OBJ_LAYER::UI)->SetPos2D(SCREEN_HEIGHT - 300.f, 100.f);

		mtex = Manager::GetScene()->AddGameObject<MissionTex>((int)OBJ_LAYER::UI);
	}

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

void Practice::ClearObserver()
{
	if (Input::GetKeyPress(VK_SPACE))
	{
		ClTime += TOOL::SecDiv(1.0f);
	}
	else
	{
		ClTime = 0.f;
	}

	if (ClTime > 3.0f)
	{
		Clear = true;
	}
}