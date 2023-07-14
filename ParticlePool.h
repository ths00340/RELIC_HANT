#pragma once
#include "ObjectPool.h"

class ParticleManager;

class ParticlePool :
	public ObjectPool<ParticleManager>
{
public:
	void Set(int num);
};
