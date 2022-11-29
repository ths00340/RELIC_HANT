#pragma once
#include "GameObject.h"

struct VERTEX_3D;

#define CHIP_X (100)
#define CHIP_Y (100)
#define CHIP_SIZE_X (10)
#define CHIP_SIZE_Y (10)

#define CHIP_KAZU (CHIP_X*CHIP_Y)

class Field :public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;	// インデックスバッファ
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11BlendState* blendState;
	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	VERTEX_3D* pVtx = nullptr;

	int numVertex, numIndex;

	struct maisuu
	{
		int a, b, c, d;
	};
	maisuu Mai[CHIP_KAZU];
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetHeight(Float3 inPos);
};
