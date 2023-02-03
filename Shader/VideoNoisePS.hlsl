//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"
//
//in は入力されてくるデーター
//out は出力するデータ
//
//DirectXのテクスチャの設定を受け継ぐ
Texture2D g_Texture : register(t0);//テクスチャー０番
//DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0);//テクスチャサンプ

static const float division = 768;
static const float blackinterval = 6;
static const int blackheight = 1;
static const float noisewidth = 0.01;

void main(in PS_IN In, out half4 outDiffuse : SV_Target)
{
	//画面Y座標を分割
	int divisionindex = In.TexCoord.y * division;

	//一定間隔で横に区切ったブロックを作る
	int noiseindex = divisionindex / blackinterval;

	//ブロックごとに横にずらす座標を決める前処理
	//時間による乱数のシード値（timeに互いに素っぽい数をかけたのを複数用意するといい感じになる）
	float3 timenoise = float3(0, int(_SinTime.w * 61), int(_SinTime.w * 83));
	//ときどき大きくずらす（時間のシード値が変更されるたびに5%の確率でノイズが10倍になる）
	float noiserate = rand(timenoise) < 0.05 ? 10 : 1;

	//横にずらす大きさを乱数で決める（0~1）（時間的にも位置的にもランダムになるように位置によるシード値と時間によるシード値を別次元で与える）
	float xnoise = rand(float3(noiseindex, 0, 0) + timenoise);
	xnoise = xnoise * xnoise - 0.5;             //ずれを2乗して0.5引く（2乗しないと乱れすぎる気がした）
	xnoise = xnoise * noisewidth * noiserate;   //ずれにスケールをかける
	xnoise = xnoise * (Parameter.x / 2 + 1.1);   //時間的にずれに波があるようにする（いい感じになる気がする）
	xnoise = xnoise + (abs((int(_SinTime.w * 2000) % int(division / blackinterval)) - noiseindex) < 5 ? 0.005 : 0);    //ラスタースキャンっぽいノイズ

	float2 uv = In.TexCoord + float2(xnoise, 0);

	//ぼやけさせる
	half4 col1 = g_Texture.Sample(g_SamplerState, uv);;
	half4 col2 = g_Texture.Sample(g_SamplerState, uv + float2(0.005, 0));
	half4 col3 = g_Texture.Sample(g_SamplerState, uv + float2(-0.005, 0));
	half4 col4 = g_Texture.Sample(g_SamplerState, uv + float2(0, 0.005));
	half4 col5 = g_Texture.Sample(g_SamplerState, uv + float2(0, -0.005));
	half4 col = (col1 * 4 + col2 + col3 + col4 + col5) / 8;

	col.rgb = divisionindex % blackinterval < blackheight ? float4(0, 0, 0, 1) : col.rgb;
	outDiffuse = col;
}