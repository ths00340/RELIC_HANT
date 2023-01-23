#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�����v�Z������
	float light = -dot(normal.xyz, Light.Direction.xyz);

	float toonlight = 0.f;

	toonlight = 0.4f;
	toonlight += step(0.5f, light) * 0.4f;
	toonlight += step(0.7f, light) * 0.2f;
	light = toonlight;
	//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //���邳�ƐF����Z
	outDiffuse.a *=
		In.Diffuse.a; //���ʌv�Z

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	float d = -dot(eyev, normal.xyz);

	//if(d<0.2)�̒��g
	float egg = 1.f - ((1.f - step(0.1f, d)) * 0.8f);

	outDiffuse.rgb *= egg;
}