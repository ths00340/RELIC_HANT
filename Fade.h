#pragma once
#include "Common.h"
#include <iostream>

class Fade :
	public Common
{
private:
	static ID3D11Buffer* m_VertexBuffer;//頂点バッファ
	static ID3D11ShaderResourceView* m_Texture;//テクスチャー

	static ID3D11VertexShader* m_VertexShader;	//
	static ID3D11PixelShader* m_PixelShader;	//描画用プログラムを保存しておく用
	static ID3D11InputLayout* m_VertexLayout;	//
public:
	static void Load();
	static void UnLoad();
	void Init()		override;
	void Uninit()	override;
	void Update()	override;
	void Draw()		override;
};
