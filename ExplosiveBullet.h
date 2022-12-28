#pragma once
#include "GameObject.h"
class ExplosiveBullet :
	public GameObject
{
	static class Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
	Float3 startpos;
	int atk;
	float DmgRange;

	void Finish();
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 pos = Float3(0.0f, 0.0f, 0.0f), Float3 rot = Float3(0.f, 0.f, 0.f), float vel = 0.0f, int dmg = 10, float dmgrange = 5.0f);
	static void Load();
	ID3D11BlendState* GetBlendState()override { return blendState; }
	void AddVel(Float3 add = { 0.f,0.f,0.f });
};
