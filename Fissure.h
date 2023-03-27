//==============================================================================
// Filename: Fissure.h
// Description :GameObjectクラスを継承したひび割れクラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"
class Fissure :
	public GameObject
{
	static ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	int flame = 0;
	int maxtime = 60;
	float alfa = 0.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(float maxtime);
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
