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

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, viewCam->GetViewAng(), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
	Renderer::SetCameraPosition(viewCam->GetPos());
}

bool ViewCamera::CheckView(Float3 Position)
{
	Float3 pos = viewCam->GetPos();

	D3DXMATRIX vp, invvp;

	vp = viewCam->GetViewMatrix() * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	Float3 vpos[4], wpos[4];

	vpos[0] = { -1.2f, 1.2f,1.f };
	vpos[1] = { 1.2f, 1.2f,1.f };
	vpos[2] = { -1.2f,-1.2f,1.f };
	vpos[3] = { 1.2f,-1.2f,1.f };

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;
	v = Position - pos;

	//ç∂ñ îªíË
	{
		v1 = wpos[0] - pos;
		v2 = wpos[2] - pos;
		D3DXVec3Cross(&n, &v1, &v2);

		if (D3DXVec3Dot(&n, &v) < 0.f)
			return false;
	}

	//âEñ îªíË
	{
		v1 = wpos[3] - pos;
		v2 = wpos[1] - pos;
		D3DXVec3Cross(&n, &v1, &v2);

		if (D3DXVec3Dot(&n, &v) < 0.f)
			return false;
	}

	//è„ñ îªíË
	{
		v1 = wpos[1] - pos;
		v2 = wpos[0] - pos;
		D3DXVec3Cross(&n, &v1, &v2);

		if (D3DXVec3Dot(&n, &v) < 0.f)
			return false;
	}

	//â∫ñ îªíË
	{
		v1 = wpos[2] - pos;
		v2 = wpos[3] - pos;
		D3DXVec3Cross(&n, &v1, &v2);

		if (D3DXVec3Dot(&n, &v) < 0.f)
			return false;
	}

	return true;
}