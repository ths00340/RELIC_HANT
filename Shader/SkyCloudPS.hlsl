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
    Out.Diffuse = 0.f;
    Out.Diffuse.a = 1.f;

    float3 lightDir = Light.Direction.xyz;
    lightDir = normalize(lightDir);

    float3 eyeVector = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    float dle = -dot(lightDir, eyeVector);
    float3 sunLight = float3(1.f, 1.f, 1.f);
    float3 wavelength = float3(0.650f, 0.570f, 0.475f);
    float3 wavelength4inv = 1.f / pow(wavelength, 4);

    float atomDensityEye = .05f + pow(1.f - eyeVector.y, 20) * .95f;//視線方向の大気の密度の算出
    float atomDensityLight = .05f + pow(1.f + lightDir.y, 20) * .95f;//光源からの大気の値
    float3 scatteringLight = sunLight * exp(-atomDensityLight * atomDensityEye * wavelength4inv * 1.f);

    float rayleighPhase = 0.5f + dle * dle;

    Out.Diffuse.rgb += scatteringLight * atomDensityEye * wavelength4inv * rayleighPhase * 10.f;


    float g = 0.990f;
    float g2 = g * g;
    float miePhase = 1.5f * ((1.f - g2) / (2.f + g2)) * (1.f + dle * dle) / pow(1.f + g2 - 2.f * g * dle, 1.5f);

    Out.Diffuse.rgb += scatteringLight * miePhase;

    float2 cloudPosition = In.WorldPosition.xz / In.WorldPosition.y;
    float noise = fBm(cloudPosition + offset, _SinTime.x, 5);
    Out.Diffuse.rgb = lerp(Out.Diffuse.rgb, float3(1.f, 1.f, 1.f), noise);

    Out.Diffuse.rgb *= smoothstep(-0.f, 1.f, In.WorldPosition.y);

    //トーンマッピング
    Out.Diffuse.rgb = Out.Diffuse.rgb / (Out.Diffuse.rgb + 1.f);

    Out.Normal = 0.f;
}