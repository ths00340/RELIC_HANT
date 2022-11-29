#pragma once
#include "Game.h"
#include "audio.h"
#include "ViewCamera.h"
class Title :
	public Game
{
private:
	ViewCamera* VCam;
	GameObject* enemy;
	GameObject* dodai;
	Audio* BGM;
	Audio* OK;
public:
	void Init();
	void Uninit();
	void Update();
};
