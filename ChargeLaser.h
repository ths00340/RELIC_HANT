//==============================================================================
// Filename: ChargeLaser.h
// Description: Weponクラス継承したチャージレーザークラスの定義
//==============================================================================
#pragma once
#include "Wepon.h"
class ChargeEffect;//チャージ用エフェクト

class ChargeLaser :
	public Wepon
{
	static Model* model;
	static Model* predictionLine;//予測線

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	int charge_rate = 0;//チャージ時間
	ChargeEffect* eff = nullptr;//チャージ用エフェクト
public:
	ChargeLaser(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 10;
		time = 0;
		angle = 0.0f;
		charge_rate = 0;
		objS = NULL;
		eff = nullptr;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
