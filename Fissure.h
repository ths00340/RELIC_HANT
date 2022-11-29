#pragma once
#include "GameObject.h"
class Fissure :
	public GameObject
{
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
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
