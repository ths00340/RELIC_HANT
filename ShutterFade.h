//==============================================================================
// Filename: ShutterFade.h
// Description: FadeParentクラスを継承したシャッターフェードクラスの定義
//==============================================================================
#pragma once
#include "audio.h"
#include "FadeParent.h"
class ShutterFade :
	public FadeParent
{
	static ID3D11Buffer* m_VertexBuffer;//頂点バッファ
	ID3D11ShaderResourceView* m_Texture;//テクスチャー

	ID3D11VertexShader* m_VertexShader;	//
	ID3D11PixelShader* m_PixelShader;	//描画用プログラムを保存しておく用
	ID3D11InputLayout* m_VertexLayout;	//
	ID3D11BlendState* blendState;
	Audio* shutter;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
