#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;

	float4 worldNormal, normal; //���[�J���ϐ����쐬
	normal = float4(In.Normal.xyz, 0.0);//�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��
}