//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"//図形や乱数用インクルード
//
//in は入力されてくるデーター
//out は出力するデータ
//

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//1 ピクセル分の色を決定して出力する
	//入力されたピクセルの色をそのまま出力
	float2 UV = In.TexCoord;
	outDiffuse = float4(0.f, 0.f, 0.f, 1.f);

	float scan = fan_shape(UV, 50.f) * grad_rot(UV) + grid(UV);
	outDiffuse.g = scan;

	outDiffuse.r *= lerp(1.0f, 0.12f, 1.f - scan);
	outDiffuse.g *= lerp(1.0f, 0.12f, 1.f - scan);
	outDiffuse.b = lerp(0.0f, 0.58f, 1.f - scan);
	outDiffuse.a = lerp(1.f, 0.5f, 1.f - scan);
	//円形の外側を透明に
	outDiffuse.a *= lerp(0.0f, 1.f, 1.f - disc(UV, 0.5f));
}