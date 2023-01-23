#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);


	float4 worldNormal, normal; //���[�J���ϐ����쐬
	normal = float4(In.Normal.xyz, 0.0);//�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
	Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light); //���邳��0��1�̊ԂŖO�a������

	Out.Diffuse.rgb = light; //���邳�𒸓_�F�Ƃ��ďo��
	Out.Diffuse *= Light.Diffuse;
	Out.Diffuse.a = In.Diffuse.a; //���͒��_�̕������̂܂܏o��



	Out.TexCoord = In.TexCoord;
	Out.Position = mul(In.Position, wvp);

	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(In.Position, World);
	
}