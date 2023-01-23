#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal); //�s�N�Z���̖@���𐳋K��

	float4 normalmap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
	normalmap = normalmap * 2.f - 1.f;
	normal.x = -normalmap.r;
	normal.y = normalmap.b;
	normal.z = normalmap.g;
	normal.w = 0.f;

	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;//�����v�Z������

	//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb; //���邳����Z
	outDiffuse.rgb *= light;
	outDiffuse.a *= In.Diffuse.a; //���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
	specular = saturate(specular); //�l���T�`�����[�g

	specular = pow(specular, 30); //�����ł͂R�O�悵�Ă݂�

	outDiffuse.rgb += specular;//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������

	outDiffuse.r = max(outDiffuse.r, 0.05f);
	outDiffuse.g = max(outDiffuse.g, 0.05f);
	outDiffuse.b = max(outDiffuse.b, 0.05f);
}