#include "main.h"
#include "renderer.h"
#include "Win_Window.h"
#include <io.h>

D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::m_Device = NULL;
ID3D11DeviceContext* Renderer::m_DeviceContext = NULL;
IDXGISwapChain* Renderer::m_SwapChain = NULL;
ID3D11RenderTargetView* Renderer::m_RenderTargetView = NULL;
ID3D11RenderTargetView* Renderer::m_RenderTextureView[(int)RENDER_::NUM] = { NULL };;

ID3D11DepthStencilView* Renderer::m_DepthStencilView = NULL;
ID3D11DepthStencilView* Renderer::m_ShadowDepthStencilView = NULL;

ID3D11Buffer* Renderer::m_WorldBuffer = NULL;
ID3D11Buffer* Renderer::m_ViewBuffer = NULL;
ID3D11Buffer* Renderer::m_ProjectionBuffer = NULL;
ID3D11Buffer* Renderer::m_MaterialBuffer = NULL;
ID3D11Buffer* Renderer::m_LightBuffer = NULL;
ID3D11Buffer* Renderer::m_CameraBuffer = NULL;
ID3D11Buffer* Renderer::m_ParameterBuffer = NULL;
ID3D11Buffer* Renderer::m_TimeBuffer = NULL;

ID3D11DepthStencilState* Renderer::m_DepthStateEnable = NULL;
ID3D11DepthStencilState* Renderer::m_DepthStateDisable = NULL;

ID3D11ShaderResourceView* Renderer::m_ShadowDepthShaderResourceView = NULL;
ID3D11ShaderResourceView* Renderer::m_RenderTextureShaderResourceView[(int)RENDER_::NUM] = { NULL };

//深度ステンシルビュー
ID3D11DepthStencilView* Renderer::m_RenderDepthStencilView;

//深度テクスチャの入れ物
ID3D11ShaderResourceView* Renderer::m_RenderDepthShaderResourceView;

D3DXVECTOR4 Renderer::Time = { 0.f,0.f,0.f,0.f };

float Renderer::ClearColor[4] = { 0.f,0.f,0.f,1.f };

void Renderer::Init()
{
	HRESULT hr = S_OK;

	IDXGIFactory1* factory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory));

	IDXGIAdapter1* adapter;
	int GPUnum = 0;
	SIZE_T GPUpow = 0;

	for (int i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		if (GPUpow < desc.DedicatedVideoMemory)
		{
			GPUpow = desc.DedicatedVideoMemory;
			GPUnum = i;
		}
	}
	factory->EnumAdapters1(GPUnum, &adapter);

	factory->Release();

	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = WinGetWindowHandle();
	swapChainDesc.SampleDesc.Count = 1;//MSAA設定
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(adapter,//自動選択
		D3D_DRIVER_TYPE_UNKNOWN,//セットで変更
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_DeviceContext);

	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile = NULL;
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;//MSAA設定用
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	//画面を分割させたり、画面別に処理を変えたい場合はここをいじって分割や複製する
	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

	m_DeviceContext->RSSetState(rs);

	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;	//半透明になる
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[1].BlendEnable = TRUE;
	blendDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[2].BlendEnable = TRUE;
	blendDesc.RenderTarget[2].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[2].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[2].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[2].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[2].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[2].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[2].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_Device->CreateBlendState(&blendDesc, &blendState);
	m_DeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;//異方性フィルタリング
	samplerDesc.MaxAnisotropy = 4;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_Device->CreateSamplerState(&samplerDesc, &samplerState);

	m_DeviceContext->PSSetSamplers(0, 1, &samplerState);

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);

	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	bufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);

	bufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);

	bufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_TimeBuffer);
	m_DeviceContext->PSSetConstantBuffers(7, 1, &m_TimeBuffer);

	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = D3DXVECTOR4(0.f, -1.0f, 0.0f, 1.0f);
	light.Position = D3DXVECTOR4(0.f, 500.f, 0.f, 0.f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = D3DXCOLOR(2.f, 2.f, 2.f, 1.f);
	SetLight(light);

	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Shininess = 1.0f;
	SetMaterial(material);

	int MipLV = 6;
	{
		//シャドウバッファ作成
		ID3D11Texture2D* depthTexture[2] = { NULL };
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = swapChainDesc.BufferDesc.Width; //バックバッファのサイズを受けつぐ
		td.Height = swapChainDesc.BufferDesc.Height;
		td.MipLevels = MipLV;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;//32bitの自由な形式のデータとする
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT; // ↓デプス＆ステンシルバッファとして作成
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//光源から見た深度
		{
			hr = m_Device->CreateTexture2D(&td, NULL, &depthTexture[0]);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}

			//デプスステンシルターゲットビュー作成
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
			ZeroMemory(&dsvd, sizeof(dsvd));
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvd.Flags = 0;
			hr = m_Device->CreateDepthStencilView(depthTexture[0], &dsvd,
				&m_ShadowDepthStencilView);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}

			//シェーダーリソースビュー作成
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
			SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = MipLV;
			hr = m_Device->CreateShaderResourceView(depthTexture[0], &SRVDesc,
				&m_ShadowDepthShaderResourceView);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}
		}

		//カメラから見た深度
		{
			hr = m_Device->CreateTexture2D(&td, NULL, &depthTexture[1]);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}

			//デプスステンシルターゲットビュー作成
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
			ZeroMemory(&dsvd, sizeof(dsvd));
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvd.Flags = 0;
			hr = m_Device->CreateDepthStencilView(depthTexture[1], &dsvd,
				&m_RenderDepthStencilView);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}

			//シェーダーリソースビュー作成
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
			SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = MipLV;
			hr = m_Device->CreateShaderResourceView(depthTexture[1], &SRVDesc,
				&m_RenderDepthShaderResourceView);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}
		}
	}

	{
		//レンダーテクスチャー作成
			//2次元テクスチャの設定
		ID3D11Texture2D* mpTex[(int)RENDER_::NUM];
		D3D11_TEXTURE2D_DESC texDesc{};
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = swapChainDesc.BufferDesc.Width; //バックバッファのサイズを受けつぐ
		texDesc.Height = swapChainDesc.BufferDesc.Height;
		texDesc.MipLevels = MipLV;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc = swapChainDesc.SampleDesc;

		for (int i = 0; i < (int)RENDER_::NUM; i++)
		{
			//2次元テクスチャの生成
			hr = m_Device->CreateTexture2D(&texDesc, NULL, &mpTex[i]);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}

			//レンダーターゲットビューの設定
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
			memset(&rtvDesc, 0, sizeof(rtvDesc));
			rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			//レンダーターゲットビューの生成
			hr = m_Device->CreateRenderTargetView(mpTex[i], &rtvDesc, &m_RenderTextureView[i]);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}
			// シェーダリソースビューの設定
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			memset(&srvDesc, 0, sizeof(srvDesc));
			srvDesc.Format = rtvDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = MipLV;

			//シェーダーリソースビューの生成
			hr = m_Device->CreateShaderResourceView(mpTex[i], &srvDesc, &m_RenderTextureShaderResourceView[i]);
			if (FAILED(hr))
			{
				//失敗時の処理の記入
			}
		}
	}
}

void Renderer::Uninit()
{
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();

	for (int i = 0; i < (int)RENDER_::NUM; i++)
		m_RenderTextureShaderResourceView[i]->Release();

	m_DeviceContext->ClearState();
	for (int i = 0; i < (int)RENDER_::NUM; i++)
		m_RenderTextureView[i]->Release();

	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();
}

void Renderer::Update()
{
	Time.w += 1.f / 60.f;
	Time.x = Time.w / 8.f;
	Time.y = Time.w / 4.f;
	Time.z = Time.w / 2.f;
	m_DeviceContext->UpdateSubresource(m_TimeBuffer, 0, NULL, &Time, 0, 0);
	Time.w = fmodf(Time.w, 10000.f);
}

void Renderer::Begin()
{
	// バックバッファクリア
	//デフォルトのバックバッファと深度バッファへ復帰させておく
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::End()
{
	m_SwapChain->Present(1, 0);
}

void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void Renderer::SetWorldMatrix(D3DXMATRIX* WorldMatrix)
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix(D3DXMATRIX* ViewMatrix)
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix)
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void Renderer::SetMaterial(MATERIAL Material)
{
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(D3DXVECTOR3 CameraPosition)
{
	D3DXVECTOR4 pos = D3DXVECTOR4(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &pos, 0, 0);
}

void Renderer::SetParameter(D3DXVECTOR4 Parameter)
{
	m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}

void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}

void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}