//==============================================================================
// Filename: Logo_S.h
// Description :Game�N���X���p�������ŏ��̃��S�\���p�N���X�̒�`
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
