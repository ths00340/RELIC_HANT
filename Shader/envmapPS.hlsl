#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv:register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//光源計算をする
	float light = -dot(normal.xyz, Light.Direction.xyz);

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//光の反射ベクトルを計算
	float3 refv = reflect(eyev.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.5 + 0.5f;
	envTexCoord.y = -refv.y * 0.5 + 0.5f;

		//テクスチャのピクセル色を取得
	outDiffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 envTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, Parameter.z);

	outDiffuse.rgb += envTex;

	outDiffuse.rgb *=
		In.Diffuse.rgb * light; //明るさと色を乗算
	outDiffuse.a *=
		In.Diffuse.a; //α別計算

	outDiffuse.r = max(outDiffuse.r, 0.05f);
	outDiffuse.g = max(outDiffuse.g, 0.05f);
	outDiffuse.b = max(outDiffuse.b, 0.05f);
}


