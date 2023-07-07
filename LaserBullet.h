//==============================================================================
// Filename: LaserBullet.h
// Description :GameObjectクラスを継承したレーザークラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"
class LaserBullet :
	public GameObject
{
private:
	class Model* m_model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	int atk;
	float time;
	float livetime;
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
