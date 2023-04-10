#include "manager.h"
#include "renderer.h"
#include "ViewCamera.h"
#include "TextureDrawScene.h"
#include "Display.h"
#include "CreateLogo.h"

void TextureDrawScene::Init()
{
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.f;
	view.TopLeftY = 0;
	DrawTexture = AddGameObject<Display>((int)OBJ_LAYER::UI);
	DrawTexture->SetTexture(Renderer::GetRenderTexture(RENDER_::DIFFUSE));
	for (int i = 1; i < (int)RENDER_::NUM; i++)
	{
		DrawTexture->SetAddTexture(Renderer::GetRenderTexture((RENDER_)i), i - 1);
	}
	DrawTexture->SetAddTexture(Renderer::GetRenderDepthTexture(), (int)RENDER_::NUM - 1);

	DrawTexture->SetShader(SHADER_S::EDGE);
}

void TextureDrawScene::Draw()
{
	Game::Draw();
	Manager::GetScene()->UIDraw();
}