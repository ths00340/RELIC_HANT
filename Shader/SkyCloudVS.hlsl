#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"


//=============================================================================
// ���_�V�F�[�_
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

