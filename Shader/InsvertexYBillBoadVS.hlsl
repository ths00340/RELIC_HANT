#include "common.hlsl"

void main(in VS_IN_INSTANCE In, out PS_IN Out)
{
	float4x4 InstanceWorld = GetInstanceTransform(In.InstanceId);
	matrix wvp, ww;
	ww = mul(InstanceWorld, World);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, ww);
	worldNormal = normalize(worldNormal);
	worldNormal.w = 1.f;
	Out.Normal = worldNormal; //回転後の法線出力 In.Normalでなく回転後の法線を出力

	Out.Position = mul(In.Position, InstanceWorld);
	Out.Position = mul(Out.Position, View);
	Out.Position = mul(Out.Position, Projection);

	//DirectX未対応

	//float3 WorldPos = mul((float3x3)mul(In.Position, InstanceWorld), Out.Position);

	//float4 localPosition = float4(0.0, 0.0, 0.0, 1.0);
	//matrix modelViewMatrix = mul(mul(In.Position, InstanceWorld), View);
	//float3 viewPosition = mul(modelViewMatrix, localPosition);


	//float3x3 ViewRotateY = float3x3(
	//	1, View._m01, 0,
	//	0, View._m11, 0,
	//	0, View._m21, -1// Zの符号を反転して右手系に変換
	//	);

	//viewPosition += mul(ViewRotateY, WorldPos);
	//Out.Position = mul(Projection, float4(viewPosition, 1.f));
	
	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;
}