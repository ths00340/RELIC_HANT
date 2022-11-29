#pragma once
#include "GameObject.h"
class Skybox :
	public GameObject
{
private:
	static Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
