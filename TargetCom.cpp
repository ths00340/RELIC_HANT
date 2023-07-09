#include "manager.h"
#include "TargetCom.h"

void TargetCom::Init()
{
	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->AttachTarget(object);
}

void TargetCom::OnEnable()
{
	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->AttachTarget(object);
}

void TargetCom::Uninit()
{
	if (!object->GetEnable())
		return;

	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->DetachTarget(object);
}

void TargetCom::OnDisable()
{
	Game* sce = (Game*)Manager::GetScene();
	BATTLE_DATA* bData = sce->GetB_Data();
	bData->GetTargetList()->DetachTarget(object);

	object->RemoveComponent<TargetCom>();
}