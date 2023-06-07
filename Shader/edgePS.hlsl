#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_DepthTexture : register(t2);

SamplerState g_SamplerState : register(s0);

void main(in PS_EdgeInput In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = float4(0.f, 0.f, 0.f, 1.f);
	//周囲のピクセルの法線の値の平均を計算する。
	float3 Normal;
	Normal = g_NormalTexture.Sample(g_SamplerState, In.tex0).xyz * -8.0f;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex1.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex2.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex3.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex4.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex5.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex6.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex7.xy).xyz;
	Normal += g_NormalTexture.Sample(g_SamplerState, In.tex8.xy).xyz;


	//周囲のピクセルの深度値の平均を計算する。
	float depth = g_DepthTexture.Sample(g_SamplerState, In.tex0).x;
	float depth2 = g_DepthTexture.Sample(g_SamplerState, In.tex1).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex2.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex3.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex4.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex5.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex6.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex7.zw).x;
	depth2 += g_DepthTexture.Sample(g_SamplerState, In.tex8.zw).x;
	depth2 /= 8.0f;

	//法線の計算結果、あるいは深度値の計算結果が一定以上ならエッジとみなす。
	if (length(Normal) >= 0.3f || abs(depth - depth2) > 0.0005f) {
		outDiffuse = float4(0.f, 0.f, 0.f, 1.f);//エッジ
	}
	else {
		outDiffuse = g_Texture.Sample(g_SamplerState, In.tex0);
	}
}