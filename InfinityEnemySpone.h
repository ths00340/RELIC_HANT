//==============================================================================
// Filename: InfinityEnemySpone.h
// Description: BATTLE_DATA�N���X���p�������G�����N���N���X�̒�`
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class InfinityEnemySpone :
	public BATTLE_DATA
{
private:
	float ptime = 0.0f;
public:
	void Init();
	void Uninit();
	void Update();
};
