#include "DebugDrawScene.h"
#include "manager.h"
void DebugDrawScene::Draw()
{
	Game* sce = (Game*)Manager::GetScene();
	sce->DebugDraw();
}