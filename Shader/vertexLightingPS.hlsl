#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out)
{
	Out.Diffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Diffuse *= In.Diffuse;
	Out.Normal = 0.f;
}