#pragma once
#include "GameObject.h"

enum class BLEND_S;

class ParticleTest :public GameObject
{
private:
	Model* m_model;
	ID3D11BlendState* blendState;

	Float3 m_vel;
	Float3 m_ang_vel;
	float Gravity;
	int live_time;
	int time;

	Float3 DefaultSize;

public:
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
