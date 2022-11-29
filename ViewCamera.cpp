#include "main.h"
#include "renderer.h"
#include "Camera.h"
#include "Tools.h"
#include "ViewCamera.h"

Camera* NoneCam = NULL;

void ViewCamera::Init()
{
	name = "ViewCamera";
	NoneCam = new Camera(NULL);
	viewCam = NoneCam;
}

void ViewCamera::Uninit()
{
}

void ViewCamera::Update()
{
	if (viewCam == NULL)
		viewCam = NoneCam;
}

void ViewCamera::Draw()
{
	D3DXMatrixLookAtLH(viewCam->GetViewMatrixPoint(), viewCam->GetPosPoint(), viewCam->GetTagPoint(), viewCam->GetUpPoint());

	Renderer::SetViewMatrix(viewCam->GetViewMatrixPoint());

	D3DXMATRIX proMatrix;
	D3DXMatrixPerspectiveFovLH(&proMatrix, viewCam->GetViewAng(), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	Renderer::SetProjectionMatrix(&proMatrix);
	Renderer::SetCameraPosition(viewCam->GetPos());
}