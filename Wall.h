#pragma once
#include "GameObject.h"

#define CHiP_X (40)
#define CHiP_Y (10)
#define CHiP_SIZE_X (10)
#define CHiP_SIZE_Y (10)
#define DEFAULT_CHiP_SIZE (2000.0f)

#define CHiP_KAZU (CHiP_X*CHiP_Y)

class Wall :
	public GameObject
{
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;	// インデックスバッファ
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11BlendState* blendState;
	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;
	int numVertex, numIndex;

	struct maisuu
	{
		int a, b, c, d;
	};
	maisuu Mai[CHiP_KAZU];

	D3DXVECTOR4 m_Parameter = { 0.f,0.f,0.f,0.f };
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
