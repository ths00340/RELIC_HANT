//==============================================================================
// Filename: Gatling.h
// Description: Weponクラス継承したガトリングクラスの定義
//==============================================================================
#pragma once

#include "Wepon.h"

class NBulletPool;

class Gatling :
	public Wepon
{
	Model* model;//本体
	Model* barrel;//バレル
	Model* predictionLine;//予測線

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	//縦のアングルのみ
	float rate_max = 0.f;//発射レート //最速
	float rate_min = 0.75f;//初期発射レート//最遅
	float fire_rate = 0.f;//発射レートの比率
	float roll_max = 3.14f;//回転速度最大値
	float barrelRoll = 0.0f;
	int maxspednum = 20;//何発で最速発射になるか
	int shotnum = 0;//発射数カウント用
	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);

	Float3 m_barrelpos;//砲身の座標
	NBulletPool* pool = nullptr;
public:
	Gatling(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 5;
		time = 0;
		angle = 0.f;
		rate_max = 0.35f;
		rate_min = 0.75f;
		fire_rate = 0.f;
		objS = NULL;
		roll_max = 3.14f;
		maxspednum = 50;
		shotnum = 0;
		barrelRoll = 0.0f;
		rate_min = 0.75f;
		randrot = Float3(0.0f, 0.0f, 0.0f);
		range = 60.f;
		pool = nullptr;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
