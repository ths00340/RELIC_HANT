#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	Out.Position = mul(In.Position, wvp);
	Out.WorldPosition = mul(In.Position, World);

	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord;
}

