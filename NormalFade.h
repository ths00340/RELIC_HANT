#pragma once
#include "FadeParent.h"
class NormalFade :
	public FadeParent
{
	static ID3D11Buffer* m_VertexBuffer;//���_�o�b�t�@
	static ID3D11ShaderResourceView* m_Texture;//�e�N�X�`���[

	ID3D11VertexShader* m_VertexShader;	//
	ID3D11PixelShader* m_PixelShader;	//�`��p�v���O������ۑ����Ă����p
	ID3D11InputLayout* m_VertexLayout;	//
	ID3D11BlendState* blendState;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
