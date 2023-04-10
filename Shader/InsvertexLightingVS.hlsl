#include "common.hlsl"

void main(in VS_IN_INSTANCE In, out PS_IN Out)
{
	float4x4 InstanceTest = GetInstanceTransform(In.InstanceId);
	matrix wvp,ww;
	ww = mul(InstanceTest, World);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, ww);
	worldNormal = normalize(worldNormal);
	worldNormal.w = 1.f;

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse += float4(0.5f, 0.5f, 0.5f, 0.5f);
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.Position = mul(In.Position, InstanceTest);
	Out.Position = mul(Out.Position, View);
	Out.Position = mul(Out.Position, Projection);




	Out.TexCoord = In.TexCoord;
}