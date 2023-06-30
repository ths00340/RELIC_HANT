//==============================================================================
// Filename: Tree.h
// Description :GameObjectクラスを継承したビルボード用の木クラスの定義
//==============================================================================
//デファードレンダリングの関係で非表示
#pragma once
#include "GameObject.h"
class Tree :
	public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;

	class ViewCamera* m_Vcam;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 inpos);
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
