//==============================================================================
// Filename: Explosion.h
// Description: GameObject�N���X���p�������r���{�[�h�ΉԃG�t�F�N�g�N���X�̒�`
//==============================================================================

//���݃f�t�@�[�h�����_�����O�̊֌W�Ŕ�\��
#pragma once
#include "GameObject.h"
class Explosion :
	public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	int m_Count = 0;
	Float3 tagpos;
	int maxcount = 9;
	int flame = 0;
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
