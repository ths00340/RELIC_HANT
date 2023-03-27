#include "ResourceManager.h"

nametype ResourceManager::Modelname;
std::vector<Model*> ResourceManager::models;

nametype ResourceManager::TextureName;
std::vector<ID3D11ShaderResourceView*> ResourceManager::Texture;

ID3D11BlendState* ResourceManager::B_State[(int)BLEND_S::BLEND_E];
ID3D11RasterizerState* ResourceManager::rs[(int)FRAME_S::FRAME_E];
ID3D11VertexShader* ResourceManager::m_VertexShader[(int)SHADER_S::SHADER_E];
ID3D11PixelShader* ResourceManager::m_PixelShader[(int)SHADER_S::SHADER_E];
ID3D11InputLayout* ResourceManager::m_VertexLayout[(int)SHADER_S::SHADER_E];

void ResourceManager::StartUp()
{
	//ブレンドステート設定
	D3D11_BLEND_DESC blend{};
	//SORT_TRUE
	{
		blend.AlphaToCoverageEnable = TRUE;
		blend.IndependentBlendEnable = TRUE;
		for (int i = 0; i < (int)RENDER_::NUM; i++)
		{
			blend.RenderTarget[i].BlendEnable = TRUE;
			blend.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::SORT_TRUE]);
	}

	//SORT_FALSE
	{
		blend.AlphaToCoverageEnable = FALSE;
		blend.IndependentBlendEnable = TRUE;
		for (int i = 0; i < (int)RENDER_::NUM; i++)
		{
			blend.RenderTarget[i].BlendEnable = TRUE;
			blend.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::SORT_FALSE]);
	}

	//OBJ_TRANS
	{
		blend.AlphaToCoverageEnable = FALSE;
		blend.IndependentBlendEnable = TRUE;

		for (int i = 0; i < (int)RENDER_::NUM; i++)
		{
			blend.RenderTarget[i].BlendEnable = TRUE;
			blend.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[i].DestBlend = D3D11_BLEND_DEST_COLOR;
			blend.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::OBJ_TRANS]);
	}

	//OBJ_OPAQUE
	{
		blend.AlphaToCoverageEnable = FALSE;
		blend.IndependentBlendEnable = TRUE;
		for (int i = 0; i < (int)RENDER_::NUM; i++)
		{
			blend.RenderTarget[i].BlendEnable = TRUE;
			blend.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::OBJ_OPAQUE]);
	}

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	//WIRE_FRAME
	{
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::WIRE_FRAME]);
	}

	//CULL_BACK
	{
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_BACK]);
	}

	//CULL_FLONT
	{
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_FLONT]);
	}

	//CULL_NONE
	{
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_NONE]);
	}

	//シェーダー設定
		//LIGHT_OFF
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_OFF], &m_VertexLayout[(int)SHADER_S::LIGHT_OFF], "unlitTextureVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_OFF], "unlitTexturePS.cso");
	}

	//LIGHT_ON
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_ON], &m_VertexLayout[(int)SHADER_S::LIGHT_ON], "vertexLightingVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_ON], "vertexLightingPS.cso");
	}

	//LIGHT_LIM
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_LIM], &m_VertexLayout[(int)SHADER_S::LIGHT_LIM], "RimLightingVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_LIM], "RimLightingPS.cso");
	}

	//NORMAL_TOON
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL_TOON], &m_VertexLayout[(int)SHADER_S::NORMAL_TOON], "toonVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL_TOON], "toonPS.cso");
	}

	//NORMAL_FOG
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL_FOG], &m_VertexLayout[(int)SHADER_S::NORMAL_FOG], "FogVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL_FOG], "FogPS.cso");
	}

	//INVISIBLE_FAR
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::INVISIBLE_FAR], &m_VertexLayout[(int)SHADER_S::INVISIBLE_FAR], "FarInvisibleVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::INVISIBLE_FAR], "FarInvisiblePS.cso");
	}

	//LOD_SHADOW
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LOD_SHADOW], &m_VertexLayout[(int)SHADER_S::LOD_SHADOW], "LODShadowVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LOD_SHADOW], "LODShadowPS.cso");
	}

	//MOSAIC
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::MOSAIC], &m_VertexLayout[(int)SHADER_S::MOSAIC], "mosaicVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::MOSAIC], "mosaicPS.cso");
	}

	//GLITCH_NOISE
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::GLITCH_NOISE], &m_VertexLayout[(int)SHADER_S::GLITCH_NOISE], "GlitchNoiseVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::GLITCH_NOISE], "GlitchNoisePS.cso");
	}

	//JAGGY_NOISE
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::JAGGY_NOISE], &m_VertexLayout[(int)SHADER_S::JAGGY_NOISE], "GlitchJaggyNoiseVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::JAGGY_NOISE], "GlitchJaggyNoisePS.cso");
	}

	//VIDEO_NOISE
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::VIDEO_NOISE], &m_VertexLayout[(int)SHADER_S::VIDEO_NOISE], "VideoNoiseVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::VIDEO_NOISE], "VideoNoisePS.cso");
	}

	//EDGE
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::EDGE], &m_VertexLayout[(int)SHADER_S::EDGE], "edgeVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::EDGE], "edgePS.cso");
	}

	//METAMOL
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::METAMOL], &m_VertexLayout[(int)SHADER_S::METAMOL], "MetamolVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::METAMOL], "MetamolPS.cso");
	}

	//SCAN
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::SCAN], &m_VertexLayout[(int)SHADER_S::SCAN], "ScanVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::SCAN], "ScanPS.cso");
	}

	//DEPTH_SHADOW
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::DEPTH_SHADOW], &m_VertexLayout[(int)SHADER_S::DEPTH_SHADOW], "DepthShadowMappingVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::DEPTH_SHADOW], "DepthShadowMappingPS.cso");
	}

	//NORMAL_MAPING
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL_MAPING], &m_VertexLayout[(int)SHADER_S::NORMAL_MAPING], "normalMapingVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL_MAPING], "normalMapingPS.cso");
	}

	//NORMAL
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL], &m_VertexLayout[(int)SHADER_S::NORMAL], "normalVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL], "normalPS.cso");
	}

	//SIMPLE_SKY
	{
		Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::SIMPLE_SKY], &m_VertexLayout[(int)SHADER_S::SIMPLE_SKY], "SkyVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::SIMPLE_SKY], "SkyPS.cso");
	}
}

void ResourceManager::Init()
{
	StartUp();
}

void ResourceManager::Uninit()
{
	DeleteModels();
	DeleteTextures();
	ReleaseState();
}