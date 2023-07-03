#include "common.hlsl"
#include "PainterTool.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_DepthTexture : register(t2);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float2 st = In.TexCoord * 10.f;
	outDiffuse.rgb = g_DepthTexture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.a = 1.f;
}