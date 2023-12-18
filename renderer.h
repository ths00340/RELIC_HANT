//==============================================================================
// Filename: renderer.h
// Description: DirectX11のレンダラーの定義
//==============================================================================
#pragma once

enum class RENDER_
{
	DIFFUSE,//通常色
	NORMAL, //法線
	NUM,//最大数
};

struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};

struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXVECTOR4 Position;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
};

class Renderer
{
private:

	struct RenderSettings {
		DWORD oldTime=0;

		//シェーダー送信用の時間
		D3DXVECTOR4 Time{0.f,0.f,0.f,0.f};
		float deltaTime=0.f;

		//バックグラウンドの色
		float ClearColor[4] = {0.f,0.f,0.f,0.f};

		D3D_FEATURE_LEVEL       m_FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

		ID3D11Device* m_Device=nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;

		ID3D11Buffer* m_WorldBuffer = nullptr;
		ID3D11Buffer* m_ViewBuffer = nullptr;
		ID3D11Buffer* m_ProjectionBuffer = nullptr;
		ID3D11Buffer* m_MaterialBuffer = nullptr;
		ID3D11Buffer* m_LightBuffer = nullptr;
		ID3D11Buffer* m_CameraBuffer = nullptr;
		ID3D11Buffer* m_ParameterBuffer = nullptr;
		ID3D11Buffer* m_TimeBuffer = nullptr;

		ID3D11DepthStencilState* m_DepthStateEnable = nullptr;
		ID3D11DepthStencilState* m_DepthStateDisable = nullptr;

		//深度ステンシルビュー
		ID3D11DepthStencilView* m_ShadowDepthStencilView = nullptr;

		//深度テクスチャの入れ物
		ID3D11ShaderResourceView* m_ShadowDepthShaderResourceView = nullptr;

		//レンダーテクスチャ用ターゲットビュー
		ID3D11RenderTargetView* m_RenderTextureView[(int)RENDER_::NUM] = { nullptr };

		//レンダーテクスチャの入れ物
		ID3D11ShaderResourceView* m_RenderTextureShaderResourceView[(int)RENDER_::NUM] = { nullptr };

		//深度ステンシルビュー
		ID3D11DepthStencilView* m_RenderDepthStencilView = nullptr;

		//深度テクスチャの入れ物
		ID3D11ShaderResourceView* m_RenderDepthShaderResourceView = nullptr;

	};

	static RenderSettings m_Settings;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(D3DXVECTOR4 Parameter);

	static ID3D11Device* GetDevice(void) { return m_Settings.m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_Settings.m_DeviceContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static ID3D11ShaderResourceView* GetShadowDepthTexture()
	{
		return m_Settings.m_ShadowDepthShaderResourceView;
	}

	static ID3D11ShaderResourceView* GetRenderDepthTexture()
	{
		return m_Settings.m_RenderDepthShaderResourceView;
	}

	static ID3D11ShaderResourceView** GetRenderTexture() {
		return m_Settings.m_RenderTextureShaderResourceView;
	};

	static ID3D11ShaderResourceView* GetRenderTexture(RENDER_ LAYER) {
		return m_Settings.m_RenderTextureShaderResourceView[(int)LAYER];
	};

	//深度バッファ用Begin
	static void BeginDepth()//新規関数追加
	{
		//シャドウバッファを深度バッファに設定し、内容を1で塗りつぶしておく
		m_Settings.m_DeviceContext->OMSetRenderTargets(0, NULL, m_Settings.m_ShadowDepthStencilView);
		m_Settings.m_DeviceContext->ClearDepthStencilView(m_Settings.m_ShadowDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//レンダーテクスチャ用Begin
	static void  BeginTexture()
	{
		m_Settings.m_DeviceContext->OMSetRenderTargets((int)RENDER_::NUM, m_Settings.m_RenderTextureView, m_Settings.m_RenderDepthStencilView);

		for (int i = 0; i < (int)RENDER_::NUM; i++)
			m_Settings.m_DeviceContext->ClearRenderTargetView(m_Settings.m_RenderTextureView[i], m_Settings.ClearColor);

		m_Settings.m_DeviceContext->ClearDepthStencilView(m_Settings.m_RenderDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	static void SetTime(float inTime = 0.f) { m_Settings.Time.w = inTime; }

	static const Float4 GetTime() { return m_Settings.Time; }
	static const float GetDeltaTime() { return m_Settings.deltaTime; }
};
