#pragma once
#include "GameObject.h"
class Plane :
    public GameObject
{
	ID3D11Buffer* m_VertexBuffer;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	ID3D11BlendState* GetBlendState()override { return blendState; }
	void SetShader(SHADER_S inType);
};

