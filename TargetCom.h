//==============================================================================
// Filename: TargetCom.h
// Description: コンポーネントクラスを継承した討伐対象の登録用コンポーネントの定義
//==============================================================================
#pragma once
#include "CComponent.h"
class TargetCom :
	public CComponent
{
public:
	TargetCom(GameObject* inobject) :CComponent(inobject) {};
	void Init();
	void Update();
	void Uninit();
	void Draw() {};
};
