#include "main.h"
#include "renderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tools.h"
#include "input.h"

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
}

void Camera::Uninit() {}

void Camera::Update()
{
	Dir = m_tag - m_pos;

	//�p�x����
	Angle.x = TOOL::Limit(Angle.x, TOOL::AToR(60.0f), -TOOL::AToR(60.0f));
	if (!tag)
	{
		if (!Input::GetPause())
		{
			//mouse�ŉ�] //���Έړ��̎擾
			//D3DXVECTOR2 relative = Input::MouseRelative();
			//Angle.y += ((relative.x) * 0.045f) / 60.0f;
			//Angle.x += ((relative.y) * 0.025f) / 60.0f;

			//mouse�ŉ�]�@�^�񒆌Œ�̈ړ��l�Q��
			float x = 0.0f;
			float y = 0.0f;

			y = (Input::GetCursor().x - (SCREEN_WIDTH / 2)) / SCREEN_WIDTH;
			x = (Input::GetCursor().y - (SCREEN_HEIGHT / 2)) / SCREEN_HEIGHT;

			Angle.y += TOOL::AToR(180.0f) * y * 0.5f;
			Angle.x += TOOL::AToR(180.0f) * x * 0.5f;

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
	//�^�[�Q�b�g�Ɏ��_�����킹��
	if (tag)
	{
		static Float3 tagpos;
		tagpos = tag->Getpos() - object->Getpos();
		//atan2f(tagpos.x, tagpos.z);
		Angle.y += TOOL::SubAngle(tagpos, Dir) / 3.0f;
		Angle.x = -atan2f(tagpos.y, hypotf(tagpos.z, tagpos.x));
	}

	if (Mode == CamMode::FPP)
	{
		m_tag = object->Getpos() + TOOL::GetForward(object->Getrot()) * 5.0f;
		m_pos = object->Getpos() + object->GetUp() * 0.5f;
	}
	if (Mode == CamMode::TPP)
	{
		//�h��
		{
			time++;
			if (shakeP)
			{
				{//�ʒu�h��
					float x, y, a;
					a = 1.f - ((float)time / (float)MaxRandP);
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
				{//�p�x�h��
					m_up = { 0.f, 1.f, 0.f };
					float a = 1.f - ((float)time / (float)MaxRandR);
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
				time = 0;
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

void Camera::SetShakePos(int max, float Str)
{
	shakeP = true;
	time = 0;
	MaxRandP = max;
	shakeStrP = Str;
}

void Camera::SetShakeRot(int max, float Str)
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