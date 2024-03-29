#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out)
{
	float RangeMax = 0.8f;
	Out.Normal = In.Normal;
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//光源計算をする
	float4 lightDir;
	lightDir = In.WorldPosition - Light.Position;
	lightDir = normalize(lightDir);

	float light = 0.5f - dot(normal.xyz, lightDir.xyz) * 0.5f;

	//スペキュラの計算
	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する
	//光の反射ベクトルを計算
	float3 refv = reflect(lightDir.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	float specular = -dot(eyev, refv); //鏡面反射の計算
	specular = saturate(specular); //値をサチュレート
	specular = pow(specular, 20); //ここでは３０乗してみる

		//テクスチャのピクセル色を取得
	Out.Diffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	float range = dot(Light.Direction.xyz, lightDir.xyz);

	Out.Diffuse.rgb *=
		In.Diffuse.rgb * light; //明るさと色を乗算
	Out.Diffuse.a *=
		In.Diffuse.a; //α別計算

	//フォグ

	//float len = length((In.WorldPosition.xyz - CameraPosition.xyz));
	float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
	float fog = saturate(dist / 30.f);
	Out.Diffuse.rgb = Out.Diffuse.rgb * (1.f - fog) + (Out.Diffuse.rgb * float3(0.95f, 0.95f, 0.95f)) * fog;
}