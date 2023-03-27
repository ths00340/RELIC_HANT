//==============================================================================
// Filename: Bullet.h
// Description :GameObjectクラスを継承した弾丸クラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"

class Bullet :public GameObject
{
private:
	class Model* m_model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	Float3 startpos;
	int atk;
	float range = 0.f;
	const char* name = "Bullet";
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 pos = Float3(0.0f, 0.0f, 0.0f), Float3 rot = Float3(0.0f, 0.0f, 0.0f), float vel = 0.0f, int dmg = 1, float maxrange = 60.f);
	static void Load();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};