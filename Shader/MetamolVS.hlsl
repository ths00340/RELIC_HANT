//���_�V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//

void main(in VS_IN In, out PS_IN Out)
{
	float Time = _SinTime.w * 1.f;
	float Hight = 2.5f;
	float4 Pos = In.Position;
	float4 fbmPos = Pos / 10.f;
	fbmPos = (fbmPos + 1.f) * 0.5f;
	float2 fbmPosXZ = fbmPos.xz;

	float offsetY = fBm(fbmPosXZ, Time);
	offsetY = (2.f * offsetY) - 1.f;
	offsetY *= Hight;
	Pos.y += offsetY;

	float texX = (2.f * fBm((fbmPosXZ + float2(0.1f, 0.f)), Time) - 1.f) * Hight;
	float texx = (2.f * fBm((fbmPosXZ + float2(-0.1f, 0.f)), Time) - 1.f) * Hight;
	float texZ = (2.f * fBm((fbmPosXZ + float2(0.f, 0.1f)), Time) - 1.f) * Hight;
	float texz = (2.f * fBm((fbmPosXZ + float2(0.f, -0.1f)), Time) - 1.f) * Hight;

	float3 du = { 1,2.f * (texX - texx), 0 };
	float3 dv = { 0,2.f * (texZ - texz), 1 };

	float4 Normal = float4(-normalize(Vec3Cross(du, dv)), 0.f);

	//�P���_���̃f�[�^���󂯎��A�������āA�o�͂���
	//���_�ϊ����� ���̏����͕K���K�v
	float4 worldNormal, normal; //���[�J���ϐ����쐬
	normal = float4(Normal.xyz, 0.0);//�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��

	matrix wvp; //�s��ϐ����쐬
	wvp = mul(World, View); //wvp = ���[���h�s�񁖃J�����s��
	wvp = mul(wvp, Projection); //wvp = wvp *�v���W�F�N�V�����s��
	Out.Position = mul(Pos, wvp); //�ϊ����ʂ��o�͂���

								  //�󂯎�������̒��_�̃f�t���[�Y�����̂܂܏o��
	Out.Diffuse = In.Diffuse;
	//�e�N�X�`�����W���o��
	Out.TexCoord = In.TexCoord;

	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(Pos, World);

	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);//���[���h�s�񁖃��C�g�r���[�s��
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);//����Ɂ����C�g�v���W�F�N�V�����s��
	Out.ShadowPosition = mul(Pos, lightwvp);//���C�g�J��������݂����_���W�o��
}