#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //行列変数を作成
	wvp = mul(World, View); //wvp = ワールド行列＊カメラ行列
	wvp = mul(wvp, Projection); //wvp = wvp *プロジェクション行列
	Out.Position = mul(In.Position, wvp); //変換結果を出力する

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;
	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);
}