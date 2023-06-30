//==============================================================================
// Filename: Field.h
// Description :GameObjectクラスを継承した地形クラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"

struct VERTEX_3D;

#define CHIP_X (250)
#define CHIP_Y (250)
#define CHIP_SIZE_X (2.5)
#define CHIP_SIZE_Y (2.5)

#define CHIP_KAZU (CHIP_X*CHIP_Y)

class Field :public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;	// インデックスバッファ
	ID3D11ShaderResourceView* m_Texture  = NULL;
	ID3D11ShaderResourceView* m_Texture2 = NULL;
	ID3D11ShaderResourceView* m_Texture3 = NULL;
	ID3D11BlendState* blendState;
	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	VERTEX_3D* pVtx = nullptr;

	int numVertex, numIndex;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetHeight(Float3 inPos);
};
