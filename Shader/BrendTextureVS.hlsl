#include "common.hlsl"
void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //いつもの処理だが
	wvp = mul(World, View); //1パス目はライトカメラの行列
	wvp = mul(wvp, Projection); //2パス目は通常カメラの行列が設定される
	Out.Position = mul(In.Position, wvp); //頂点座標をワールド変換して出力

	float4 worldNormal, normal; //法線のワールド変換
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal; //法線を出力

	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord; //テクスチャ座標を出力
	
	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);//ワールド行列＊ライトビュー行列
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);//さらに＊ライトプロジェクション行列
	Out.ShadowPosition = mul(In.Position, lightwvp);//ライトカメラからみた頂点座標出力

		//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);
}