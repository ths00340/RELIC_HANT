#include "main.h"
#include "renderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tools.h"
#include "cInputOperation.h"

class Bazooka;

void Camera::Init()
{
	m_pos = Float3(0.0f, 2.5f, -5.0f);
	m_tag = Float3(0.0f, 2.5f, 0.0f);
	m_up = Float3(0.0f, 1.0f, 0.0f);
	Dir = Float3(0.0f, 0.0f, 0.0f);
	Angle = Float3(0.0f, 0.0f, 0.0f);
	ViewAng = 1.0f;
	Range = 30.0f;
	Mode = CamMode::TPP;

	m_pInput = nullptr;
	if (m_pInput == nullptr)
	{
		m_pInput = object->LoadComponent<cInputOperation>();
	}
}

void Camera::Uninit() {
	if (m_pInput)
		m_pInput = nullptr;
}

void Camera::Update()
{
	if (Mode == CamMode::FREE)
		return;

	Dir = m_tag - m_pos;

	//Šp“x§ŒÀ
	Angle.x = TOOL::Limit(Angle.x, TOOL::AToR(60.0f), -TOOL::AToR(60.0f));
	if (!m_pInput == NULL)
		if (!tag)
		{
			{
				//mouse‚Å‰ñ“]@^‚ñ’†ŒÅ’è‚ÌˆÚ“®’lŽQÆ
				float x = 0.0f;
				float y = 0.0f;

				if (m_pInput != NULL)
				{
					y = (m_pInput->GetPov().x);
					x = (m_pInput->GetPov().y);
				}

				Angle.y += TOOL::AToR(5.0f) * y;
				Angle.x += TOOL::AToR(5.0f) * x;

				if (Angle.y < -TOOL::AToR(360))
					Angle.y = 0;

				if (Angle.y > TOOL::AToR(360))
					Angle.y = 0;
			}
		}
	if (tag)
		if (!TOOL::CanHit(object->Getpos(), tag->Getpos(), Range))
		{
			tag = NULL;
		}
	//ƒ^[ƒQƒbƒg‚ÉŽ‹“_‚ð‡‚í‚¹‚é
	if (tag)
	{
		Float3 tagpos;
		tagpos = tag->Getpos() - object->Getpos();
		float zeroFloat = 0.f;
		Angle.y = TOOL::SmoothDamp(Angle.y, Angle.y + TOOL::SubAngle(tagpos, Dir), zeroFloat, 0.025f, 1000.f, Renderer::GetDeltaTime());
		Angle.x = -atan2f(tagpos.y, hypotf(tagpos.z, tagpos.x));
		if (!tag->GetEnable())
		{
			tag = nullptr;
		}
	}

	if (Mode == CamMode::FPP)
	{
		m_tag = object->Getpos() + TOOL::GetForward(object->Getrot()) * 5.0f;
		m_pos = object->Getpos() + object->GetUp() * 0.5f;
	}
	if (Mode == CamMode::TPP)
	{
		//—h‚ê
		{
			time += Renderer::GetDeltaTime();
			if (shakeP)
			{
				{//ˆÊ’u—h‚ê
					float x, y, a;
					a = 1.f - (time / MaxRandP);
					x = shakeStrP * a * (TOOL::RandF() - 0.5f);
					y = shakeStrP * a * (TOOL::RandF() - 0.5f);

					Addpos.y = y;
					Addpos.x = cosf(Angle.y) * x;
					Addpos.z = -sinf(Angle.y) * x;
				}

				if (time >= MaxRandP)
				{
					shakeP = false;
					Addpos = TOOL::Uniform();
				}
			}
			if (shakeR)
			{
				{//Šp“x—h‚ê
					m_up = { 0.f, 1.f, 0.f };
					float a = 1.f - (time / MaxRandR);
					float i = shakeStrR * a * (TOOL::RandF() - 0.5f);
					m_up += { cosf(Angle.y)* i, -i, -sinf(Angle.y) * i };
				}

				if (time >= MaxRandR)
				{
					shakeR = false;
				}
			}

			if (!shakeP && !shakeR)
			{
				time = 0.f;
			}
		}

		float addrange = 1.0f;

		if (object->LoadComponent<Bazooka>())
			addrange = 2.0f;

		m_tag = object->Getpos() + (GetUp() * 3.f * addrange) * fabsf(cosf(Angle.x)) + Addpos;

		m_pos = -TOOL::GetForward(object->Getrot());
		m_pos.x = object->Getpos().x - (sinf(Angle.y) * (((50.f * addrange) * object->Getscl().z) * cosf(Angle.x)));
		m_pos.y = object->Getpos().y + m_pos.y + 1.5f - ((40.f * object->Getscl().z) * sinf(-Angle.x));
		m_pos.z = object->Getpos().z - (cosf(Angle.y) * (((50.f * addrange) * object->Getscl().z) * cosf(Angle.x)));

		m_pos += Addpos;
	}
}

void Camera::Draw() {}

void Camera::SetTag(D3DXVECTOR3 intag)
{
	m_tag = intag;
}

void Camera::SetUp(D3DXVECTOR3 inup)
{
	m_up = inup;
}

void Camera::SetShakePos(float max, float Str)
{
	shakeP = true;
	time = 0.f;
	MaxRandP = max;
	shakeStrP = Str;
}

void Camera::SetShakeRot(float max, float Str)
{
	shakeR = true;
	time = 0;
	MaxRandR = max;
	shakeStrR = Str;
}

const Float3 Camera::GetDir()
{
	return Dir;
}