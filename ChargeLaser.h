//==============================================================================
// Filename: ChargeLaser.h
// Description: Wepon�N���X�p�������`���[�W���[�U�[�N���X�̒�`
//==============================================================================
#pragma once
#include "Wepon.h"
class ChargeEffect;//�`���[�W�p�G�t�F�N�g

class ChargeLaser :
	public Wepon
{
	Model* model;
	Model* predictionLine;//�\����

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	float charge_rate = 0.f;//�`���[�W����
	ChargeEffect* eff = nullptr;//�`���[�W�p�G�t�F�N�g
public:
	ChargeLaser(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 10;
		time = 0.f;
		angle = 0.0f;
		charge_rate = 0.f;
		objS = NULL;
		eff = nullptr;
		m_VertexShader = nullptr;
		m_PixelShader = nullptr;
		m_VertexLayout = nullptr;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
};
