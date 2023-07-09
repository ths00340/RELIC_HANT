//==============================================================================
// Filename: Gatling.h
// Description: Wepon�N���X�p�������K�g�����O�N���X�̒�`
//==============================================================================
#pragma once

#include "Wepon.h"

class NBulletPool;

class Gatling :
	public Wepon
{
	Model* model;//�{��
	Model* barrel;//�o����
	Model* predictionLine;//�\����

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	//�c�̃A���O���̂�
	float rate_max = 0.f;//���˃��[�g //�ő�
	float rate_min = 0.75f;//�������˃��[�g//�Œx
	float fire_rate = 0.f;//���˃��[�g�̔䗦
	float roll_max = 3.14f;//��]���x�ő�l
	float barrelRoll = 0.0f;
	int maxspednum = 20;//�����ōő����˂ɂȂ邩
	int shotnum = 0;//���ː��J�E���g�p
	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);

	Float3 m_barrelpos;//�C�g�̍��W
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
