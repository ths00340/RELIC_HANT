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
	bool isGround = false;//当たったかどうか
	Float3 m_Normal = Float3(0.f, 0.f, 0.f);
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
	const bool GetisGround() { return isGround; }
	const Float3 GetGroundNormal() { return m_Normal; }
};
