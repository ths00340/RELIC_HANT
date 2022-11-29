#pragma once
#include "Game.h"
class Logo_S :
	public Game
{
private:
	int time = 0;
public:
	void Init();
	void Uninit();
	void Update();
};
