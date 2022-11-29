#pragma once
#include "GameObject.h"
#include "Status.h"

class Enemy :public GameObject
{
private:
	static Model* m_model;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
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
	ID3D11BlendState* GetBlendState()override { return blendState; }
	static Model* GetModel() { return m_model; }

	const char* GetName()
	{
		return name;
	}

	void Damage(int dmg);
	void Finish();
};
