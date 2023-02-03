#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;

	float4 worldNormal, normal; //ローカル変数を作成
	normal = float4(In.Normal.xyz, 0.0);//法線ベクトルのwを0としてコピー（平行移動しないため)
	worldNormal = mul(normal, World); //法線をワールド行列で回転する
	worldNormal = normalize(worldNormal); //回転後の法線を正規化する
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //回転後の法線出力 In.Normalでなく回転後の法線を出力
}