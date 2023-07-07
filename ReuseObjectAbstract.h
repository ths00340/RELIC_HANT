#pragma once
#include "GameObject.h"
class ReuseObjectAbstract :
    public GameObject
{
	virtual	void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
};

