//==============================================================================
// Filename: EnemyAnni.h
// Description :Gameクラスを継承した殲滅クラスの定義
//==============================================================================
#pragma once
#include "Game.h"
class EnemyAnni :
	public Game
{
public:
	void Init();
	void NextScene();
	void BGMSelect();
};
