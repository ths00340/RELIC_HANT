//==============================================================================
// Filename: Boss01.h
// Description :Gameクラスを継承したボス戦1用シーンクラスの定義
//==============================================================================
#pragma once
#include "Game.h"
class Boss01 :
	public Game
{
public:
	void Init();
	void NextScene();
	void BGMSelect();
};
