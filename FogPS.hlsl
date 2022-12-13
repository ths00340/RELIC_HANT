//thsozai ==>�O��܂ł̏�ԁ@ZIP�@�@�p�X���[�h�@nabe

//BlinnPhongLightingPS.hlsl
//BlinnPhongLightingVS.hlsl

#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float RangeMax = 0.8f;

	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�����v�Z������
	float4 lightDir;
	lightDir = In.WorldPosition - Light.Position;
	lightDir = normalize(lightDir);

	float light = 0.5f - dot(normal.xyz, lightDir.xyz) * 0.5f;

	//�X�y�L�����̌v�Z
	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������
	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(lightDir.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
	specular = saturate(specular); //�l���T�`�����[�g
	specular = pow(specular, 20); //�����ł͂R�O�悵�Ă݂�

		//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	float range = dot(Light.Direction.xyz, lightDir.xyz);

	//light	 = step(RangeMax, range) * light;
	//specular = step(RangeMax, range) * specular;

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //���邳�ƐF����Z
	outDiffuse.a *=
		In.Diffuse.a; //���ʌv�Z

	//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������
	//outDiffuse.rgb += specular;

	//�t�H�O

	//float len = length((In.WorldPosition.xyz - CameraPosition.xyz));
	float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
	float fog = saturate(dist / 300.f);
	outDiffuse.rgb = outDiffuse.rgb * (1.f-fog) + (float3(0.95f, 0.95f, 0.95f)) * fog;
}