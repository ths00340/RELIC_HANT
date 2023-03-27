//==============================================================================
// Filename: Boss01_2.h
// Description :Gameクラスを継承した大量の敵＋ボス戦1用シーンクラスの定義
//==============================================================================
#pragma once
#include "Game.h"
class Boss01_2 :
	public Game
{
public:
	void Init();
	void NextScene();
	void BGMSelect();
};
