#pragma once
#include "ObjectPool.h"

class ParticleManager;

class ParticlePool :
	public ObjectPool
{
private:
	std::list<ParticleManager*> Pool;
public:
	void Init();
	void Uninit();
	void Set(int num);
	ParticleManager* Recycle();
};
