#include "common.hlsl"
void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View); //ライトカメラの行列
	wvp = mul(wvp, Projection); //通常カメラの行列
	float4 worldNormal, normal; //法線のワールド変換
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	//光源処理
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
	Out.Position = mul(In.Position, wvp); //頂点座標をワールド変換して出力
	Out.Normal = worldNormal; //法線を出力
	Out.TexCoord = In.TexCoord; //テクスチャ座標を出力
	Out.WorldPosition = mul(In.Position, World);

	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);//ワールド行列＊ライトビュー行列
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);//さらに＊ライトプロジェクション行列
	Out.ShadowPosition = mul(In.Position, lightwvp);//ライトカメラからみた頂点座標出力
}