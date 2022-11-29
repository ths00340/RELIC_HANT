#pragma once

#include "CComponent.h"
#include "Wepon.h"
#include "ChargeEffect.h"
class ChargeLaser :
	public Wepon
{
	static Model* model;
	static Model* predictionLine;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	//ècÇÃÉAÉìÉOÉãÇÃÇ›

	int charge_rate = 0;
	ChargeEffect* eff = nullptr;
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
