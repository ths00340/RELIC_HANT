#pragma once
#include "Game.h"
#include "audio.h"
#include "ViewCamera.h"
class Title :
	public Game
{
private:
	GameObject* enemy;
	GameObject* dodai;
	Audio* BGM;
	Audio* OK;
public:
	void Init();
	void Uninit();
	void Update();
};
