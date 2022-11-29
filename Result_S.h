#pragma once
#include "Game.h"
class Result_S :
	public Game
{
private:
	Scene* mainscene;
	bool In;
	bool Out;
	float test;
public:
	void Init();
	void Uninit();
	void Update();
};
