#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal); //ピクセルの法線を正規化
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5; //光源計算をする

	//テクスチャのピクセル色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb; //明るさを乗算
	outDiffuse.rgb *= light;
	outDiffuse.a *= In.Diffuse.a; //αに明るさは関係ないので別計算

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//光の反射ベクトルを計算
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	//ハーフベクトルの作成
	float3 halfv = eyev + Light.Direction.xyz; //視線とライトベクトルを加算
	halfv = normalize(halfv); //正規化する

	float specular = -dot(halfv, normal.xyz); //ハーフベクトルと法線の内積を計算
	specular = saturate(specular); //サチュレートする

	//specular = pow(specular, 30); //ここでは３０乗してみる

	//outDiffuse.rgb += specular;//スペキュラ値をデフューズとして足しこむ

	float rim = 1.0 + dot(eyev, normal.xyz); //視線と法線の内積を明るさに変換する
	rim = pow(rim, 1) * 1.0f; //スペキュラと同じような処理を適当に行う。
	rim = saturate(rim); //rimをサチュレートする
	float4 col = { 1.f,0.f,0.f,1.f };
	outDiffuse.rgb += rim * col; //通常の色へ加算する。
	outDiffuse.a = In.Diffuse.a;
}