//==============================================================================
// Filename: ParticleTest.h
// Description: GameObjectクラスを継承したなんちゃってパーティクルのクラス定義
//==============================================================================

//インスタンシングの組み込みに伴い削除予定
#pragma once
#include "GameObject.h"

enum class BLEND_S;

class ParticleTest :public GameObject
{
private:
	D3DXMATRIX m_World;
	Float3 m_vel = { 0.f,0.f,0.f };
	Float3 m_ang_vel = { 0.f,0.f,0.f };
	float Gravity = 0.f;
	int live_time = 0;
	int time = 0;

	Float3 DefaultSize;

public:
	ParticleTest()
	{
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
	void InstanceDraw();
	void Set(D3DXVECTOR3 pos = Float3(0.f, 0.f, 0.f),
		float inGravity = GRAVITY,
		int inLiveTime = 60,
		Float3 vel = Float3(1.f, 1.f, 1.f),
		Float3 scl = Float3(0.15f, 0.15f, 0.15f),
		Float3 Rot = Float3(1.0f, 1.0f, 1.0f));

	D3DXMATRIX GetWorld() { return m_World; };
};
