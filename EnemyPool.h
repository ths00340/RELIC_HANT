#pragma once
#include "ObjectPool.h"

class Enemy;

//\‘¢‚ª‰˜‚¢‚Ì‚ÅC³

class EnemyPool :
	public ObjectPool<Enemy>
{
public:
	void Set(int num);
};
