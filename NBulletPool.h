#pragma once
#include "ObjectPool.h"

class Bullet;

class NBulletPool :
	public ObjectPool<Bullet>
{
public:
	void Set(int num);
};
