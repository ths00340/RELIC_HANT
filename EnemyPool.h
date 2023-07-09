#pragma once
#include "ObjectPool.h"

class Enemy;

//\‘¢‚ª‰˜‚¢‚Ì‚ÅC³

class EnemyPool :
	public ObjectPool
{
private:
	std::list<Enemy*> Pool;
public:
	void Init();
	void Uninit();
	void Set(int num);
	Enemy* Recycle();
};
