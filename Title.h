//==============================================================================
// Filename: Title.h
// Description :Game�N���X���p�������^�C�g���V�[���N���X�̒�`
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
