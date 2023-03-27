//==============================================================================
// Filename: Title.h
// Description :Gameクラスを継承したタイトルシーンクラスの定義
//==============================================================================
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
