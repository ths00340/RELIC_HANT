#pragma once
#include "Common.h"
#include <iostream>

class Fade :
	public Common
{
private:
	static ID3D11Buffer* m_VertexBuffer;//���_�o�b�t�@
	static ID3D11ShaderResourceView* m_Texture;//�e�N�X�`���[

	static ID3D11VertexShader* m_VertexShader;	//
	static ID3D11PixelShader* m_PixelShader;	//�`��p�v���O������ۑ����Ă����p
	static ID3D11InputLayout* m_VertexLayout;	//
public:
	static void Load();
	static void UnLoad();
	void Init()		override;
	void Uninit()	override;
	void Update()	override;
	void Draw()		override;
};
