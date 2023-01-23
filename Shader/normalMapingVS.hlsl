#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);


	float4 worldNormal, normal; //ローカル変数を作成
	normal = float4(In.Normal.xyz, 0.0);//法線ベクトルのwを0としてコピー（平行移動しないため)
	worldNormal = mul(normal, World); //法線をワールド行列で回転する
	worldNormal = normalize(worldNormal); //回転後の法線を正規化する
	Out.Normal = worldNormal; //回転後の法線出力 In.Normalでなく回転後の法線を出力

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light); //明るさを0と1の間で飽和化する

	Out.Diffuse.rgb = light; //明るさを頂点色として出力
	Out.Diffuse *= Light.Diffuse;
	Out.Diffuse.a = In.Diffuse.a; //αは頂点の物をそのまま出力



	Out.TexCoord = In.TexCoord;
	Out.Position = mul(In.Position, wvp);

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);
	
}