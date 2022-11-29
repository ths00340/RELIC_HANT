//==============================================================================
// Filename: Enemy_Anni.h
// Description: BATTLE_DATAクラスを継承した敵殲滅クラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class Enemy_Anni :
	public BATTLE_DATA
{
public:
	void Init();
	void Uninit();
	void Update();
};
