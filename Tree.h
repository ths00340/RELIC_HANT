//==============================================================================
// Filename: Tree.h
// Description :GameObject�N���X���p�������r���{�[�h�p�̖؃N���X�̒�`
//==============================================================================
//�f�t�@�[�h�����_�����O�̊֌W�Ŕ�\��
#pragma once
#include "GameObject.h"
class Tree :
	public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;

	class ViewCamera* m_Vcam;
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
