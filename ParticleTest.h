#pragma once
#include "GameObject.h"

enum class BLEND_S;

class ParticleTest :public GameObject
{
private:
	Model* m_model = nullptr;
	ID3D11BlendState* blendState = nullptr;

	Float3 m_vel = { 0.f,0.f,0.f };
	Float3 m_ang_vel = { 0.f,0.f,0.f };
	float Gravity = 0.f;
	int live_time = 0;
	int time = 0;

	Float3 DefaultSize;

public:
	ParticleTest()
	{
		m_model = nullptr;
		blendState = nullptr;

		m_vel = { 0.f,0.f,0.f };
		m_ang_vel = { 0.f,0.0f,0.f };
		Gravity = 0.f;;
		live_time = 0;
		time = 0;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(D3DXVECTOR3 pos = Float3(0.f, 0.f, 0.f),
		float inGravity = GRAVITY,
		int inLiveTime = 60,
		Float3 vel = Float3(1.f, 1.f, 1.f),
		Float3 scl = Float3(0.15f, 0.15f, 0.15f),
		Float3 Rot = Float3(1.0f, 1.0f, 1.0f));

	void SetModel(Model* inmodel, BLEND_S blend);
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
