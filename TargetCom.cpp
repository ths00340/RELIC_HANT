#include "manager.h"
#include "TargetCom.h"

void TargetCom::Init()
{
	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->AttachTarget(object);
}

void TargetCom::Uninit()
{
	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->DetachTarget(object);
}