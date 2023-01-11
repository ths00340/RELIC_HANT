//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//
//DirectX�̃e�N�X�`���̐ݒ���󂯌p��
Texture2D g_Texture : register(t0);//�e�N�X�`���[�O��
//DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);//�e�N�X�`���T���v

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float2 uv=In.TexCoord;
	uv = floor(uv * 30.f) / 30.f+0.001f;
	//���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, uv);

	//���͂��ꂽ�s�N�Z���̐F�����̂܂܏o��
	outDiffuse *= In.Diffuse;

	outDiffuse.a = 1.0f;
}