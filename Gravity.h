//==============================================================================
// Filename: Gravity.h
// Description: コンポーネントクラスを継承した重力クラスの定義
//==============================================================================
#pragma once
#include "CComponent.h"

class Field;

class Gravity :
	public CComponent
{
private:
	float groundHeight = 0.0f;//着地する地面の高さ
	float gravity = GRAVITY;//毎フレーム加算される下方向への力
	Field* field = nullptr;//細かい当たり判定用フィールド

public:
	Gravity(GameObject* inobject) :CComponent(inobject) {
		groundHeight = 0.0f;
		gravity = GRAVITY;
	};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Set(float ingra) { gravity = ingra; }//重力値設定
};
