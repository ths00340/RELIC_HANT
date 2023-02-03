//頂点シェーダー
#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"
//
//in は入力されてくるデーター
//out は出力するデータ
//

void main(in VS_IN In, out PS_IN Out)
{
	float Time = _SinTime.w * 1.f;
	float Hight = 2.5f;
	float4 Pos = In.Position;
	float4 fbmPos = Pos / 10.f;
	fbmPos = (fbmPos + 1.f) * 0.5f;
	float2 fbmPosXZ = fbmPos.xz;

	float offsetY = fBm(fbmPosXZ, Time);
	offsetY = (2.f * offsetY) - 1.f;
	offsetY *= Hight;
	Pos.y += offsetY;

	float texX = (2.f * fBm((fbmPosXZ + float2(0.1f, 0.f)), Time) - 1.f) * Hight;
	float texx = (2.f * fBm((fbmPosXZ + float2(-0.1f, 0.f)), Time) - 1.f) * Hight;
	float texZ = (2.f * fBm((fbmPosXZ + float2(0.f, 0.1f)), Time) - 1.f) * Hight;
	float texz = (2.f * fBm((fbmPosXZ + float2(0.f, -0.1f)), Time) - 1.f) * Hight;

	float3 du = { 1,2.f * (texX - texx), 0 };
	float3 dv = { 0,2.f * (texZ - texz), 1 };

	float4 Normal = float4(-normalize(Vec3Cross(du, dv)), 0.f);

	//１頂点分のデータを受け取り、処理して、出力する
	//頂点変換処理 この処理は必ず必要
	float4 worldNormal, normal; //ローカル変数を作成
	normal = float4(Normal.xyz, 0.0);//法線ベクトルのwを0としてコピー（平行移動しないため)
	worldNormal = mul(normal, World); //法線をワールド行列で回転する
	worldNormal = normalize(worldNormal); //回転後の法線を正規化する
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //回転後の法線出力 In.Normalでなく回転後の法線を出力

	matrix wvp; //行列変数を作成
	wvp = mul(World, View); //wvp = ワールド行列＊カメラ行列
	wvp = mul(wvp, Projection); //wvp = wvp *プロジェクション行列
	Out.Position = mul(Pos, wvp); //変換結果を出力する

								  //受け取ったこの頂点のデフューズをそのまま出力
	Out.Diffuse = In.Diffuse;
	//テクスチャ座標を出力
	Out.TexCoord = In.TexCoord;

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(Pos, World);

	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);//ワールド行列＊ライトビュー行列
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);//さらに＊ライトプロジェクション行列
	Out.ShadowPosition = mul(Pos, lightwvp);//ライトカメラからみた頂点座標出力
}