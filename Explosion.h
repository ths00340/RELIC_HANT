//==============================================================================
// Filename: Explosion.h
// Description: GameObjectクラスを継承したビルボード火花エフェクトクラスの定義
//==============================================================================

//現在デファードレンダリングの関係で非表示
#pragma once
#include "GameObject.h"
class Explosion :
	public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	int m_Count = 0;
	Float3 tagpos;
	int maxcount = 9;
	int flame = 0;
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
