#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out : SV_Target)
{
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//光源計算をする
	float light = -dot(normal.xyz, Light.Direction.xyz);

	Out.Normal.rgb = normal;
	Out.Normal.a = 1.f;
	//スペキュラの計算
	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	////光の反射ベクトルを計算
	//float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	//refv = normalize(refv); //正規化する

	//ハーフベクトル作成
	float3	halfv = eyev + Light.Direction.xyz;
	halfv = normalize(halfv); //正規化する

	//鏡面反射の計算
	float specular = -dot(halfv, normal.xyz);
	specular = saturate(specular); //値をサチュレート
	specular = pow(specular, 30); //ここでは３０乗してみる

	//リムライティング
	float rim = 1.0 + dot(eyev, normal.xyz); //視線と法線の内積を明るさに変換する
	rim = pow(rim, 3) * 2.0f; //スペキュラと同じような処理を適当に行う。
	rim = saturate(rim); //rimをサチュレートする

	//テクスチャのピクセル色を取得
	Out.Diffuse =
		g_Texture.Sample(g_SamplerState, In.TexCoord);

	Out.Diffuse.rgb *=
		In.Diffuse.rgb * light; //明るさと色を乗算
	Out.Diffuse.a *=
		In.Diffuse.a; //α別計算

	//スペキュラ値をデフューズとして足しこむ
	Out.Diffuse.rgb += specular;
	//リム値をデフューズとして足しこむ
	Out.Diffuse.r += rim;

	Out.Diffuse.r = max(Out.Diffuse.r, 0.05f);
	Out.Diffuse.g = max(Out.Diffuse.g, 0.05f);
	Out.Diffuse.b = max(Out.Diffuse.b, 0.05f);
}