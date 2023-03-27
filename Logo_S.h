//==============================================================================
// Filename: Logo_S.h
// Description :Gameクラスを継承した最初のロゴ表示用クラスの定義
//==============================================================================
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
	void Draw();
};
