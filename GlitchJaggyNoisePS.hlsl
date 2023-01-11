//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
//
//in は入力されてくるデーター
//out は出力するデータ
//
//DirectXのテクスチャの設定を受け継ぐ
Texture2D g_Texture : register(t0);//テクスチャー０番
//DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0);//テクスチャサンプ

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord;
    //ポスタライズ 
    float posterize1 = floor(frac(perlinNoise(_SinTime.xy) * 10.f) / (1.f / Parameter.y)) * (1.f / Parameter.y);
    float posterize2 = floor(frac(perlinNoise(_SinTime.xy) * 5.f) /  (1.f / Parameter.y)) * (1.f / Parameter.y);
    //uv.x方向のノイズ計算 -0.1 < noiseX < 0.1
    float noiseX = (2.f * rand(posterize1) - 0.5f) * 0.1f;
    //step(t,x) はxがtより大きい場合1を返す
    float frequency = step(rand(posterize2), Parameter.x);
    noiseX *= frequency;
    //uv.y方向のノイズ計算 -1 < noiseY < 1
    float noiseY = 2.f * rand(posterize1) - 0.5f;
    //グリッチの高さの補間値計算 どの高さに出現するかは時間変化でランダム
    float glitchLine1 = step(uv.y - noiseY, rand(uv));
    float glitchLine2 = step(uv.y + noiseY, noiseY);
    float glitch = saturate(glitchLine1 - glitchLine2);
    //速度調整
    uv.x = lerp(uv.x, uv.x + noiseX, glitch);
    //テクスチャサンプリング
    outDiffuse = g_Texture.Sample(g_SamplerState,uv);
}