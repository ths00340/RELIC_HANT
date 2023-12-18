#include "DebugDrawScene.h"
#include "manager.h"
void DebugDrawScene::Draw()
{
#ifdef DEBUG
	Game* sce = (Game*)Manager::GetScene();
	sce->DebugDraw();
#endif // DEBUG
}