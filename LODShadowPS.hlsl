#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	outDiffuse.rgb *= Parameter.x;

	float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
	float fog = saturate(dist / 50.f);
	outDiffuse.a = outDiffuse.a * (1.f - fog);
	outDiffuse.a += step(0.001f, outDiffuse.a) * 0.05f;

}