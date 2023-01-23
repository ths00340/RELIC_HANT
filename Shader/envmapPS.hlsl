#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv:register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�����v�Z������
	float light = -dot(normal.xyz, Light.Direction.xyz);

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(eyev.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.5 + 0.5f;
	envTexCoord.y = -refv.y * 0.5 + 0.5f;

		//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 envTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, Parameter.z);

	outDiffuse.rgb += envTex;

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //���邳�ƐF����Z
	outDiffuse.a *=
		In.Diffuse.a; //���ʌv�Z

	outDiffuse.r = max(outDiffuse.r, 0.05f);
	outDiffuse.g = max(outDiffuse.g, 0.05f);
	outDiffuse.b = max(outDiffuse.b, 0.05f);
}


