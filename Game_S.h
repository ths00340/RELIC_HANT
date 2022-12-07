#pragma once
#include "Game.h"
#include "Blur_Tex.h"
class Game_S :
	public Game
{
protected:
	bool debug_com = false;
	bool WireFrame = false;
	bool Start = false;
	bool Once = false;
	bool Stop = false;
	Blur_Tex* tex = nullptr;
	int cleartime = 0;
	Audio* ClearBGM = nullptr;;
	Audio* OverBGM = nullptr;;
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void NextScene();
	virtual void BGMSelect();
};
