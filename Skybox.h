//==============================================================================
// Filename: Skybox.h
// Description :GameObject�N���X���p��������N���X�̒�`
//==============================================================================
#pragma once
#include "GameObject.h"
class Skybox :
	public GameObject
{
private:
	Model* m_model;
	GameObject* m_pPl;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
