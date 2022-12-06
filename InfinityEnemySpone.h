//==============================================================================
// Filename: InfinityEnemySpone.h
// Description: BATTLE_DATAクラスを継承した敵無限湧きクラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class InfinityEnemySpone :
	public BATTLE_DATA
{
public:
	void Init();
	void Update();
};
