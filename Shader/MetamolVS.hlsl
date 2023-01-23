//���_�V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//

void main(in VS_IN In, out PS_IN Out)
{
	float Time = _SinTime.w*1.2f;
	float Hight = 2.5f;
	float4 Pos = In.Position;
	float4 fbmPos = Pos / 10.f;
	fbmPos = (fbmPos + 1.f) * 0.5f;
	float2 fbmPosXZ = fbmPos.xz;

	float offsetY = fBm(fbmPosXZ, Time) * Hight;
	offsetY = (2.f * offsetY) - 1.f;
	Pos.y += offsetY;

	float3 texX = 2.f * (fBm((fbmPosXZ + float2( 0.05f, 0.f)), Time) * Hight) - 1.f;
	float3 texx = 2.f * (fBm((fbmPosXZ + float2(-0.05f, 0.f)), Time) * Hight) - 1.f;
	float3 texZ = 2.f * (fBm((fbmPosXZ + float2(0.f,  0.05f)), Time) * Hight) - 1.f;
	float3 texz = 2.f * (fBm((fbmPosXZ + float2(0.f, -0.05f)), Time) * Hight) - 1.f;

	float3 du = { 1, 0, offsetY * (texX.x - texx.x) };
	float3 dv = { 0, 1, offsetY * (texZ.x - texz.x) };


	float4 Normal = float4(normalize(cross(du, dv)) * 0.5 + 0.5, 0.f);

	//�P���_���̃f�[�^���󂯎��A�������āA�o�͂���
	//���_�ϊ����� ���̏����͕K���K�v
	float4 worldNormal, normal; //���[�J���ϐ����쐬
	normal = float4(Normal.xyz, 0.0);//�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
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