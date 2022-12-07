//==============================================================================
// Filename: Wepon.h
// Description: コンポーネントクラス継承した武器用クラスの定義
//==============================================================================
#pragma once
#include "main.h"
#include "CComponent.h"

class Status;
class Audio;

class Wepon :
	public CComponent
{
protected:
	Float3 m_pos;//座標(親オブジェクトに相対的な位置)
	Float3 m_rot;//角度(親オブジェクトに相対的な角度)
	Float3 m_scl;//サイズ(親オブジェクトに相対的なサイズ)
	float angle = 0.0f;
	int dmg = 1;
	int time = 0;//時間計測用
	Float3 ShotAngle;//発射角度
	Float3 shotpos;//発射座標

	Status* objS = NULL;
	Audio* shot = NULL;
	float range;//射程距離
	float ReCast;
public:
	Wepon(GameObject* inobject) :CComponent(inobject) {
		m_pos = { 0.0f,0.0f,0.0f };
		m_rot = { 0.0f,0.0f,0.0f };
		m_scl = { 1.0f,1.0f,1.0f };
		angle = 0.f;
		dmg = 1;
		time = 0;
		ShotAngle = { 0.0f,0.0f,0.0f };
		shotpos = { 0.0f,0.0f,0.0f };
		range = 60.f;
		ReCast = 3.f;

		objS = NULL;
		shot = NULL;
	};
};
