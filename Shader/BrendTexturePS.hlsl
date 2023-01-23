#include "common.hlsl"
Texture2D g_Texture				: register(t0);//通常テクスチャ
Texture2D g_TextureShadowDepth	: register(t1);//シャドウマップ
Texture2D g_TextureCross		: register(t2);//環境マッピング用テクスチャ
Texture2D g_TextureNormal		: register(t3);//法線マッピング用テクスチャ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//法線マッピング
	{
		float4 normalmap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord+ _SinTime.xy);
		normalmap = normalmap * 2.f - 1.f;
		normal.x = -normalmap.r;
		normal.y = normalmap.b;
		normal.z = normalmap.g;
		normal.w = 0.f;
	}

	//光源計算をする
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;;

	//スペキュラの計算
	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//日向での色＝普通のテクスチャ＊頂点色を作成しておく
	outDiffuse = float4(0.f,0.5f,1.f,1.f);

	//環境マッピング
	//{
	//	float4 envTex = g_TextureCross.SampleBias(g_SamplerState, In.TexCoord, Parameter.z);
	//	outDiffuse.rgb += envTex;
	//}

	outDiffuse *= In.Diffuse*light; //明るさと色を乗算


	//シャドウマッピング
	{
		In.ShadowPosition.xyz /= In.ShadowPosition.w; //投影面でのピクセルの座標つくる
		In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //テクスチャ座標に変換
		In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //
	}



	//シャドウマッピング
	{
		//シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
		float depth = g_TextureShadowDepth.Sample(g_SamplerState,
			In.ShadowPosition.xy);

		//取得値が通常カメラからピクセルへの距離より小さい
		if (depth < In.ShadowPosition.z - 0.0001) //0.01はZファイティング補正値（後述）
		{
			outDiffuse.rgb *= 0.75f; //色を暗くする
		}
	}

	//光の反射ベクトルを計算
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	float specular = -dot(eyev, refv); //鏡面反射の計算
	specular = saturate(specular); //値をサチュレート

	specular = pow(specular, 30); //ここでは３０乗してみる

	outDiffuse.rgb += specular;//スペキュラ値をデフューズとして足しこむ

	//float rim = 1.0 + dot(eyev, normal.xyz); //視線と法線の内積を明るさに変換する
	//rim = pow(rim, 10) * 2.0f; //スペキュラと同じような処理を適当に行う。
	//rim = saturate(rim); //rimをサチュレートする
	//outDiffuse.rgb += rim;

	outDiffuse.a *= In.Diffuse.a;
}