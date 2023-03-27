//==============================================================================
// Filename: ENDURANCE.h
// Description :Gameクラスを継承した耐久戦用クラスの定義
//==============================================================================
#pragma once
#include "Game.h"
class ENDURANCE :
	public Game
{
public:
	void Init();
	void NextScene();
	void BGMSelect();
};
