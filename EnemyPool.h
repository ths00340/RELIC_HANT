#pragma once
#include "ObjectPool.h"

class Enemy;

//�\���������̂ŏC��

class EnemyPool :
	public ObjectPool<Enemy>
{
public:
	void Set(int num);
};
