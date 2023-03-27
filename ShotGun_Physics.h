//==============================================================================
// Filename: ShotGun_Physics.h
// Description: Wepon�N���X�p�������V���b�g�K���N���X�̒�`
//==============================================================================
#pragma once
#include "Wepon.h"
class ShotGun_Physics :
	public Wepon
{
private:
	Model* model;//�{��
	Model* barrel;//�o����
	Model* predictionLine;//�\����

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int fire_time = 0;

	Float3 randrot = Float3(0.0f, 0.0f, 0.0f);//�����p�̓��ꕨ

	Float3 m_barrelpos;//�C�g�̍��W
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
