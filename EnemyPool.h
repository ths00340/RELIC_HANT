#pragma once
#include "ObjectPool.h"

class Enemy;

//�\���������̂ŏC��

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
