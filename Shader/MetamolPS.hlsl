//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"
//
//in は入力されてくるデーター
//out は出力するデータ
//

Texture2D g_Texture : register(t0);//通常テクスチャ
Texture2D g_TextureShadowDepth : register(t1);//シャドウマップ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);
	//法線マッピング
	{
		float4 normalmap = In.Normal * 2.f - 1.f;
		normal.x = -normalmap.r;
		normal.y = normalmap.b;
		normal.z = normalmap.g;
		normal.w = 0.f;
	}

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//光源計算をする
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;
	light = clamp(light, 0.5f, 1.f);

	//海面の高さ
	float sea_height = In.WorldPosition.y - 1.f;

	float r = 0.02;
	float facing = saturate(1.f - dot(normal, eyev));
	float fresnel = r + (1.0 - r) * pow(facing, 5.0);

	float3 sea_base_color = float3(0.105f, 0.223f, 0.301f) * light * 1.5f;
	float3 sea_color = sea_base_color + (float3(0.294f, 0.349f, 0.137f) * (sea_height * 0.5 + 0.2) * 0.75f);

	//ベースのピクセル色を取得
	outDiffuse = float4(sea_color, 1.f);

	//シャドウマップ
	{
		In.ShadowPosition.xyz /= In.ShadowPosition.w; //投影面でのピクセルの座標つくる
		In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //テクスチャ座標に変換
		In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

			//シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
		float depth = g_TextureShadowDepth.Sample(g_SamplerState,In.ShadowPosition.xy);
		outDiffuse.rgb = lerp(outDiffuse.rgb, outDiffuse.rgb * 0.5f,step(depth, In.ShadowPosition.z - 0.001));
	}

	//光源計算
	{
		//光の反射ベクトルを計算
		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
		refv = normalize(refv); //正規化する

		//鏡面反射の計算
		float specular = -dot(eyev, refv);

		//値をサチュレート
		specular = saturate(specular);

		//ここでは３０乗してみる
		specular = pow(specular, 100);

		//スペキュラ値をデフューズとして足しこむ
		outDiffuse.rgb += specular;
	}
}