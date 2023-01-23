#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal); //ピクセルの法線を正規化

	float4 normalmap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
	normalmap = normalmap * 2.f - 1.f;
	normal.x = -normalmap.r;
	normal.y = normalmap.b;
	normal.z = normalmap.g;
	normal.w = 0.f;

	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;//光源計算をする

	//テクスチャのピクセル色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb; //明るさを乗算
	outDiffuse.rgb *= light;
	outDiffuse.a *= In.Diffuse.a; //αに明るさは関係ないので別計算

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//光の反射ベクトルを計算
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	float specular = -dot(eyev, refv); //鏡面反射の計算
	specular = saturate(specular); //値をサチュレート

	specular = pow(specular, 30); //ここでは３０乗してみる

	outDiffuse.rgb += specular;//スペキュラ値をデフューズとして足しこむ

	outDiffuse.r = max(outDiffuse.r, 0.05f);
	outDiffuse.g = max(outDiffuse.g, 0.05f);
	outDiffuse.b = max(outDiffuse.b, 0.05f);
}