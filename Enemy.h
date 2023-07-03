//==============================================================================
// Filename: Enemy.h
// Description :GameObject�N���X���p�������G�l�~�[�N���X�̒�`
//==============================================================================
#pragma once
#include "GameObject.h"
#include "Status.h"
#include "ResourceManager.h"

class Enemy :public GameObject
{
private:
	Model* m_model;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	class Audio* hit;

	GameObject* tag;
	int time = 0;
	Status* status;
	float FireRate = 3.0f;
	float spd = 0.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void PlayHit();
	static void Load();
	void SetFireRate(float intime) { FireRate = intime; }
	void SetShader(SHADER_S Shader) { ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, Shader); }

	ID3D11BlendState* GetBlendState()override { return blendState; }
	Model* GetModel() { return m_model; }

	const char* GetName()
	{
		return name;
	}

	//�_���[�W���o
	void Damage(int dmg);

	//���j���o
	void Finish();
};
