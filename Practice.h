//==============================================================================
// Filename: Practice.h
// Description: BATTLE_DATAクラスを継承した練習用クラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class Practice :
	public BATTLE_DATA
{
public:
	void Init();
	void Begin();
	void ClearObserver();
};
