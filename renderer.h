//==============================================================================
// Filename: renderer.h
// Description: DirectX11�̃����_���[�̒�`
//==============================================================================
#pragma once

enum class RENDER_
{
	DIFFUSE,//�ʏ�F
	NORMAL, //�@��
	NUM,//�ő吔
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
	BOOL		Dummy[3];//16byte���E�p
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
	static DWORD oldTime;

	//�V�F�[�_�[���M�p�̎���
	static D3DXVECTOR4 Time;
	static float deltaTime;

	//�o�b�N�O���E���h�̐F
	static float ClearColor[4];

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_CameraBuffer;
	static ID3D11Buffer* m_ParameterBuffer;
	static ID3D11Buffer* m_TimeBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	//�[�x�X�e���V���r���[
	static ID3D11DepthStencilView* m_ShadowDepthStencilView;

	//�[�x�e�N�X�`���̓��ꕨ
	static ID3D11ShaderResourceView* m_ShadowDepthShaderResourceView;

	//�����_�[�e�N�X�`���p�^�[�Q�b�g�r���[
	static ID3D11RenderTargetView* m_RenderTextureView[(int)RENDER_::NUM];

	//�����_�[�e�N�X�`���̓��ꕨ
	static ID3D11ShaderResourceView* m_RenderTextureShaderResourceView[(int)RENDER_::NUM];

	//�[�x�X�e���V���r���[
	static ID3D11DepthStencilView* m_RenderDepthStencilView;

	//�[�x�e�N�X�`���̓��ꕨ
	static ID3D11ShaderResourceView* m_RenderDepthShaderResourceView;

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

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static ID3D11ShaderResourceView* GetShadowDepthTexture()
	{
		return m_ShadowDepthShaderResourceView;
	}

	static ID3D11ShaderResourceView* GetRenderDepthTexture()
	{
		return m_RenderDepthShaderResourceView;
	}

	static ID3D11ShaderResourceView** GetRenderTexture() {
		return m_RenderTextureShaderResourceView;
	};

	static ID3D11ShaderResourceView* GetRenderTexture(RENDER_ LAYER) {
		return m_RenderTextureShaderResourceView[(int)LAYER];
	};

	//�[�x�o�b�t�@�pBegin
	static void BeginDepth()//�V�K�֐��ǉ�
	{
		//�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ��Ă���
		m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//�����_�[�e�N�X�`���pBegin
	static void  BeginTexture()
	{
		m_DeviceContext->OMSetRenderTargets((int)RENDER_::NUM, m_RenderTextureView, m_RenderDepthStencilView);

		for (int i = 0; i < (int)RENDER_::NUM; i++)
			m_DeviceContext->ClearRenderTargetView(m_RenderTextureView[i], ClearColor);

		m_DeviceContext->ClearDepthStencilView(m_RenderDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	static void SetTime(float inTime = 0.f) { Time.w = inTime; }

	static const Float4 GetTime() { return Time; }
	static const float GetDeltaTime() { return deltaTime; }
};
