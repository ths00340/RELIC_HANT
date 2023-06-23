#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out)
{
	Out.Normal = float4(0.f,1.f,0.f,0.f);
	float4 normal = normalize(In.Normal); //�s�N�Z���̖@���𐳋K��
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5; //�����v�Z������

	//�e�N�X�`���̃s�N�Z���F���擾
	Out.Diffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Diffuse.rgb *= In.Diffuse.rgb; //���邳����Z
	Out.Diffuse.rgb *= light;
	Out.Diffuse.a *= In.Diffuse.a; //���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	//�n�[�t�x�N�g���̍쐬
	float3 halfv = eyev + Light.Direction.xyz; //�����ƃ��C�g�x�N�g�������Z
	halfv = normalize(halfv); //���K������

	float specular = -dot(halfv, normal.xyz); //�n�[�t�x�N�g���Ɩ@���̓��ς��v�Z
	specular = saturate(specular); //�T�`�����[�g����

	//Out.Normal = In.Normal;
}