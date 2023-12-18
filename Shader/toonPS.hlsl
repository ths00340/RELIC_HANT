#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out)
{
	Out.Normal = In.Normal;
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);

	//光源計算をする
	float light = -dot(normal.xyz, Light.Direction.xyz);

	float toonlight = 0.f;

	toonlight = 0.6f;
	toonlight += step(0.7f, light) * 0.2f;
	toonlight += step(0.9f, light) * 0.1f;
	light = toonlight;
	//テクスチャのピクセル色を取得
	Out.Diffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	Out.Diffuse.rgb *=
		In.Diffuse.rgb * light; //明るさと色を乗算
	Out.Diffuse.a *=
		In.Diffuse.a; //α別計算

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	float d = -dot(eyev, normal.xyz);

	//if(d<0.2)の中身
	float egg = 1.f - ((1.f - step(0.1f, d)) * 0.8f);

	Out.Diffuse.rgb *= egg;
}