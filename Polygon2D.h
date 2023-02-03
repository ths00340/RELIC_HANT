//==============================================================================
// Filename: Polygon2D.h
// Description: ゲームオブジェクトを継承した2D用オブジェクトの定義
//==============================================================================
#pragma once
#include "GameObject.h"
class Polygon2D :public GameObject
{
protected:
	ID3D11Buffer* m_VertexBuffer = nullptr;//頂点バッファ
	ID3D11ShaderResourceView* m_Texture = nullptr;//テクスチャー
	ID3D11ShaderResourceView* m_AddTexture[7];//テクスチャー

	ID3D11VertexShader* m_VertexShader = nullptr;	//
	ID3D11PixelShader* m_PixelShader = nullptr;	//描画用プログラムを保存しておく用
	ID3D11InputLayout* m_VertexLayout = nullptr;	//
	ID3D11BlendState* blendState = nullptr;
	Float2 pos = { 0.f,0.f };
	Float2 size = { 0.f,0.f };
	Float2 uv = { 0.f,0.f };
	Float2 wh = { 0.f,0.f };
	COLOR  color = { 0.f,0.f,0.f,0.f };
	Float4 Paramarter = { 0.f,0.f,0.f,0.f };
	float  rot = 0.f;
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update() = 0;
	virtual void Draw();

	ID3D11BlendState* GetBlendState()override { return blendState; }

	void SetStatus(float X = 0.0f, float Y = 0.0f, float Width = 10.0f, float Height = 10.0f,
		float U = 0.0f, float V = 0.0f, float UW = 1.0f, float VH = 1.0f,
		COLOR Color = COLOR(1.0f, 1.0f, 1.0f, 1.0f), float Rot = 0.0f)
	{
		pos = Float2(X, Y);
		size = Float2(Width, Height);
		uv = Float2(U, V);
		wh = Float2(UW, VH);
		color = Color;
		rot = Rot;
	}

	void SetPos2D(float X, float Y) {
		pos = Float2(X, Y);
	}
	void SetSize2D(float Width, float Height) {
		size = Float2(Width, Height);
	}
	void SetUV(float U, float V) {
		uv = Float2(U, V);
	}
	void SetWH(float W, float H) {
		wh = Float2(W, H);
	}
	void SetColor2D(COLOR incolor) {
		color = incolor;
	}
	void SetRot2D(float Rot) {
		rot = Rot;
	}

	Float2* LoadPos2D() { return &pos; }
	Float2* LoadSize2D() { return &size; }
	Float2* LoadUV() { return &uv; }
	Float2* LoadWH() { return &wh; }
	COLOR* LoadColor2D() { return &color; }
	float* LoadRot2D() { return &rot; }

	void LoadTex(const char* filename);
	void SetTexture(ID3D11ShaderResourceView* Resource)
	{
		m_Texture = Resource;
	}
	void SetShader(enum class SHADER_S type);

	void SetAddTexture(ID3D11ShaderResourceView* AddResource, int num)
	{
		if (num < 7 && num >= 0)
			m_AddTexture[num] = AddResource;
	}
};
