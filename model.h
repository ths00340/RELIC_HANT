//==============================================================================
// Filename: model.h
// Description: モデルクラスの定義
//==============================================================================
#pragma once

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView* Texture;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D* VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	SUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

class Model
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	SUBSET* m_SubsetArray = NULL;
	unsigned int	m_SubsetNum = 0;
	Float3 Size_max={0.f,0.f,0.f};
	Float3 Size_min={0.f,0.f,0.f};

	void LoadObj(const char* FileName, MODEL* Model);
	void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);

public:

	void Draw();

	void Load(const char* FileName);
	void Unload();

	const Float3 Get_max();
	const Float3 Get_min();
	const Float3 Get_total();
};