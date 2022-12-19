#include "Scene.h"

bool Scene::isLoad;

void Scene::Update()
{
	for (int i = 0; i < LAYER_NUM; i++)
	{
		for (GameObject* object : g_GameObject[i])//”ÍˆÍforƒ‹[ƒv
		{
			if (object->GetStop())
				continue;

			object->Update();

			for (CComponent* com : object->GetComponent())
				com->Update();

			object->FixedUpdate();
		}
		g_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
	}
}
