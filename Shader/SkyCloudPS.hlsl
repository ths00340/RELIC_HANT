#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out PS_OUT Out)
{
    float2 offset = float2(_SinTime.x, _SinTime.y);
    //ワーピング
    float cloud = fBm(In.WorldPosition.xz * 0.01 + offset, _SinTime.x,10);
    cloud = saturate(cloud);
    Out.Diffuse.rgb = 1.0 * cloud;
    Out.Diffuse.a = cloud;    

    Out.Normal = Out.Diffuse;
}
