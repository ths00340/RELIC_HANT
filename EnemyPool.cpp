#include "EnemyPool.h"
#include "manager.h"
#include "Scene.h"
#include "Enemy.h"

void EnemyPool::Init()
{
	Pool.clear();
}

void EnemyPool::Uninit()
{
	Pool.clear();
}

void EnemyPool::Set(int num)
{
	if (Once)
		return;

	Once = true;

	Scene* sce = Manager::GetScene();

	for (int i = 0; i < num; i++)
	{
		Enemy* ene = nullptr;
		ene = sce->AddGameObject<Enemy>((int)OBJ_LAYER::Enemy);
		ene->SetEnable(false);
		Pool.push_back(ene);
	}
}

Enemy* EnemyPool::Recycle()
{
	for (Enemy* obj : Pool)
	{
		if (!obj->GetEnable())
		{
			obj->SetEnable(true);
			return obj;
		}
	}
	return nullptr;
}