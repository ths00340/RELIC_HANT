#pragma once
#include "ObjectPool.h"

class Bullet;

class NBulletPool :
	public ObjectPool
{
private:
	std::list<Bullet*> Pool;
public:
	void Init();
	void Uninit();
	void Set(int num);
	Bullet* Recycle();
};
