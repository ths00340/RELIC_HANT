#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //�s��ϐ����쐬
	wvp = mul(World, View); //wvp = ���[���h�s�񁖃J�����s��
	wvp = mul(wvp, Projection); //wvp = wvp *�v���W�F�N�V�����s��
	Out.Position = mul(In.Position, wvp); //�ϊ����ʂ��o�͂���

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;
	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(In.Position, World);
}