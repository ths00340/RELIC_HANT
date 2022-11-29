#pragma once
#include "GameObject.h"
class DebugCamMove :
	public GameObject
{
private:
	GameObject* parent = NULL;
	const char* name = "DebugCamera";
public:
	void Init();
	void Uninit();
	void Update();
	void FixedUpdate();
	void Draw();

	void Set(GameObject* inpa = NULL)
	{
		parent = inpa;
		if (parent == NULL)
			return;
		m_pos = parent->Getpos();
		m_rot = parent->Getrot();
	};

	GameObject* Get()
	{
		return parent;
	}
};
