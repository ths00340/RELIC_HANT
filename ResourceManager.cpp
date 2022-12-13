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

void ResourceManager::Init()
{
	StartUp();
}

void ResourceManager::Uninit()
{
	DeleteModels();
	ReleaseState();
}