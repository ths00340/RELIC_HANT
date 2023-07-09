#include "NBulletPool.h"
#include "manager.h"
#include "Scene.h"
#include "bullet.h"

void NBulletPool::Init()
{
	Once = false;
	Pool.clear();
}

void NBulletPool::Uninit()
{
	Pool.clear();
}

void NBulletPool::Set(int num)
{
	if (Once)
		return;

	Once = true;

	Scene* sce = Manager::GetScene();

	for (int i = 0; i < num; i++)
	{
		Bullet* ble = nullptr;
		ble = sce->AddGameObject<Bullet>((int)OBJ_LAYER::GameObject);
		ble->SetEnable(false);
		Pool.push_back(ble);
	}
}

Bullet* NBulletPool::Recycle()
{
	if (Once)
		for (Bullet* obj : Pool)
		{
			if (!obj->GetEnable())
			{
				obj->SetEnable(true);
				return obj;
			}
		}

	return nullptr;
}