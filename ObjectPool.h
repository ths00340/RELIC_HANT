#pragma once
#include "GameObject.h"

class ObjectPool :
	public GameObject
{
protected:
	bool Once = false;
public:
	void Init() { Once = false; };
	void Uninit() {};
	void Update() {};
	void Draw() {};
	virtual void Set(int num) = 0;
};
