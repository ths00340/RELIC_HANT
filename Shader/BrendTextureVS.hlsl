#include "common.hlsl"
void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //�����̏�������
	wvp = mul(World, View); //1�p�X�ڂ̓��C�g�J�����̍s��
	wvp = mul(wvp, Projection); //2�p�X�ڂ͒ʏ�J�����̍s�񂪐ݒ肳���
	Out.Position = mul(In.Position, wvp); //���_���W�����[���h�ϊ����ďo��

	//���_�@�������[���h�s��ŉ�]������(���_�Ɠ�����]��������)
	float4 worldNormal, normal; //���[�J���ϐ����쐬
	normal = float4(In.Normal.xyz, 0.0);//�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��

	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord; //�e�N�X�`�����W���o��

	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);//���[���h�s�񁖃��C�g�r���[�s��
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);//����Ɂ����C�g�v���W�F�N�V�����s��
	Out.ShadowPosition = mul(In.Position, lightwvp);//���C�g�J��������݂����_���W�o��

		//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(In.Position, World);
}