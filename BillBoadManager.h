#pragma once
#include "GameObject.h"
class BillBoadManager :
	public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11BlendState* m_BlendState = nullptr;
	ID3D11Buffer* m_pWorldBuffer = nullptr;
	ID3D11ShaderResourceView* m_pWorldSRV = nullptr;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	std::vector<D3DXMATRIX> m_Matrixs;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	bool Set(Float3 inPos, Float3 inScl);

	void SetTexture(ID3D11ShaderResourceView* Resource)
	{
		m_Texture = Resource;
	}
};