//==============================================================================
// Filename: ShotGun_Physics.h
// Description: Weponクラス継承したショットガンクラスの定義
//==============================================================================
#pragma once
#include "Wepon.h"
class ShotGun_Physics :
	public Wepon
{
private:
	Model* model;//本体
	Model* barrel;//バレル
	Model* predictionLine;//予測線

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int fire_time = 0;

	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);//乱数用の入れ物

	Float3 m_barrelpos;//砲身の座標
public:
	ShotGun_Physics(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 2;
		time = 0;
		angle = 0.f;
		fire_time = 0;
		objS = NULL;
		randrot = Float3(0.0f, 0.0f, 0.0f);
		range = 20.f;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
