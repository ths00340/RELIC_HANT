//==============================================================================
// Filename: Platform.h
// Description :GameObject�N���X���p��������Q���N���X
//==============================================================================

//���͎g���Ă��Ȃ�
#pragma once
#include "GameObject.h"
class Platform :
	public GameObject
{
	Model* m_model;

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
