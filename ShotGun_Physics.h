#pragma once

#include "Wepon.h"
class ShotGun_Physics :
	public Wepon
{
private:
	static Model* model;//本体
	static Model* barrel;//バレル
	static Model* predictionLine;//予測線

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	//縦のアングルのみ
	int fire_time = 0;

	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);

	Float3 m_barrelpos;
public:
	ShotGun_Physics(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 2;
		time = 0;
		angle = 0.f;
		fire_time = 0;
		objS = NULL;
		shotnum = 0;
		randrot = Float3(0.0f, 0.0f, 0.0f);
		range = 20.f;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
