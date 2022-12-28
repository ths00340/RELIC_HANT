#pragma once
#include "GameObject.h"
class LaserBullet :
	public GameObject
{
private:
	static class Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
	int atk;
	int time;
	int livetime;
	float startsize;
	float Range = 0.f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 pos = Float3(0.f, 0.f, 0.f), Float3 rot = Float3(0.f, 0.f, 0.f), int dmg = 1, float range = 60.f);
	void Finish();
	void SetScl(Float3 inscl) {
		m_scl = inscl;
		startsize = m_scl.x;
	};
	static void Load();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
