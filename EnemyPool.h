#pragma once
#include "ObjectPool.h"

class Enemy;

//構造が汚いので修正

class EnemyPool :
	public ObjectPool<Enemy>
{
public:
	void Set(int num);
};
