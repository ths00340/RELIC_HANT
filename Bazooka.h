//==============================================================================
// Filename: Bazooka.h
// Description: Wepon�N���X�p�������o�Y�[�J�N���X�̒�`
//==============================================================================
#pragma once
#include "Wepon.h"
class Bazooka :
	public Wepon
{
	class Model* model;//�C�g

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_VertexLayout;

	float fire_rate = 0.f;//���˃��[�g

	void Predicted(Float3 spos, Float3 vel);//�\���p
	Float3 PredictedPoint;//���e�\�����W
public:
	Bazooka(GameObject* inobject) :Wepon(inobject)
	{
		model = nullptr;
		dmg = 20;
		time = 0.f;
		angle = 0.0f;
		fire_rate = 0.f;
		objS = NULL;
		PredictedPoint = Float3(0.0f, 0.0f, 0.0f);
		m_VertexShader	= nullptr;
		m_PixelShader	= nullptr;
		m_VertexLayout	= nullptr;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetDmg(int in) { dmg = in; }
	static void Load();
};
