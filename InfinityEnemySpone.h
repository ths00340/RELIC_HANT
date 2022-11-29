//==============================================================================
// Filename: InfinityEnemySpone.h
// Description: BATTLE_DATAクラスを継承した敵無限湧きクラスの定義
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
