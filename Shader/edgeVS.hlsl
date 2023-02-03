//���_�V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//
Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_DepthTexture : register(t2);

void main(in VS_IN In, out PS_EdgeInput Out)
{
	matrix wvp; //�s��ϐ����쐬
	wvp = mul(World, View); //wvp = ���[���h�s�񁖃J�����s��
	wvp = mul(wvp, Projection); //wvp = wvp *�v���W�F�N�V�����s��
	In.Position = mul(In.Position, wvp); //�ϊ����ʂ��o�͂���

	float2 texSize;
	float level;
	//�e�N�X�`���[�̃T�C�Y���擾����
	g_Texture.GetDimensions(0, texSize.x, texSize.y, level);
	Out.Position = In.Position;
	float2 tex = In.TexCoord;

	float offset = 1.0f;
	//�@��
	{
		//�^�񒆂̃s�N�Z��
		Out.tex0 = tex;

		//�E��̃s�N�Z��
		Out.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
		Out.tex2.xy = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
		Out.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//�E�̃s�N�Z��
		Out.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
		Out.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
		Out.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
		Out.tex7.xy = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
		Out.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);
	}

	//�[�x�l
	{
		//�[�x�l�����o���Ƃ��Ɏg��UV���W
		offset = 1.0f;
		//�E��̃s�N�Z��
		Out.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
		Out.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
		Out.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//�E�̃s�N�Z��
		Out.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
		Out.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
		Out.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
		Out.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
		Out.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);
	}
}