#pragma once
#include "GameObject.h"
class BillboadTemp :
	public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
	int m_Count = 0;
	Float3 tagpos;
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
