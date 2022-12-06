#pragma once

#include "Wepon.h"
class Gatling :
	public Wepon
{
	static Model* model;//本体
	static Model* barrel;//バレル
	static Model* predictionLine;//予測線

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	//縦のアングルのみ
	int fire_time = 0;
	float fire_rate = 0.f;
	float rate_min = 0.75f;
	float roll_max = 3.14f;
	float barrelRoll = 0.0f;
	int maxspednum = 20;//何発で最速発射になるか
	int shotnum = 0;
	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);

	Float3 m_barrelpos;
public:
	Gatling(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 5;
		time = 0;
		angle = 0.f;
		fire_time = 0;
		fire_rate = 0.f;
		objS = NULL;
		roll_max = 3.14f;
		maxspednum = 50;
		shotnum = 0;
		barrelRoll = 0.0f;
		rate_min = 0.75f;
		randrot = Float3(0.0f, 0.0f, 0.0f);
		range = 60.f;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
