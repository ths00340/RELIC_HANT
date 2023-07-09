#include "ParticlePool.h"
#include "manager.h"
#include "Scene.h"
#include "ParticleManager.h"

void ParticlePool::Init()
{
	Once = false;
	Pool.clear();
}

void ParticlePool::Uninit()
{
	Pool.clear();
}

void ParticlePool::Set(int num)
{
	if (Once)
		return;

	Once = true;

	Scene* sce = Manager::GetScene();

	for (int i = 0; i < num; i++)
	{
		ParticleManager* pm = nullptr;
		pm = sce->AddGameObject<ParticleManager>((int)OBJ_LAYER::GameObject);
		pm->SetEnable(false);
		Pool.push_back(pm);
	}
}

ParticleManager* ParticlePool::Recycle()
{
	if (Once)
		for (ParticleManager* obj : Pool)
		{
			if (!obj->GetEnable())
			{
				obj->SetEnable(true);
				return obj;
			}
		}

	return nullptr;
}