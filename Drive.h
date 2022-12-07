#pragma once
//==============================================================================
// Filename: Drive.h
// Description: コンポーネントクラスを継承した（アニメーション用）足回りクラスの定義
//==============================================================================
#include "main.h"
#include "CComponent.h"
class Drive :
	public CComponent
{
protected:
	float angle = 0.0f;
	float spd = 0.0f;
	bool move = true;
	Float3 oldPos;
public:
	Drive(GameObject* inobject) :CComponent(inobject) {}
	void AddMove(float in) { angle += in; }
};
