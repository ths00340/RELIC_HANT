#pragma once
#include "GameObject.h"

class _2DTemp :public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;//���_�o�b�t�@
	static ID3D11ShaderResourceView* m_Texture;//�e�N�X�`���[

	static ID3D11VertexShader* m_VertexShader;	//
	static ID3D11PixelShader* m_PixelShader;	//�`��p�v���O������ۑ����Ă����p
	static ID3D11InputLayout* m_VertexLayout;	//
	static ID3D11BlendState* blendState;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
