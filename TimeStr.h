//==============================================================================
// Filename: TimeStr.h
// Description: ���ԍ\���̂̒�`
//==============================================================================
#pragma once
class TimeStr
{
public:
	int Hour = 0;
	int Min = 0;
	int Sec = 0;

	TimeStr() {
		Hour = 0;
		Min = 0;
		Sec = 0;
	};

	TimeStr(int h, int m, int s) {
		Hour = h;
		Min = m;
		Sec = s;
	}

	//�b���w��
	TimeStr& operator=(int sec)
	{
		Hour = sec / 3600;
		Min = (sec % 3600) / 60;
		Sec = sec % 60;
		return *this;
	}

	//�b�������炷
	TimeStr& operator-=(int sec)
	{
		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		TimeStr a;
		a = total;
		this->Hour = a.Hour;
		this->Min = a.Min;
		this->Sec = a.Sec;
		return *this;
	}

	//�b�������炷
	TimeStr& operator-(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//�b���̒ǉ�
	TimeStr& operator+(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total += sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//�S�Ă̎��Ԃ�b���ŕԋp
	int GetTotal() {
		return (Hour * 3600) + (Min * 60) + Sec;
	}

	//�������Ԃ���r�i���\���́j
	bool operator==(TimeStr a)
	{
		return GetTotal() == a.GetTotal();
	}

	//�������Ԃ���r�i�b���j
	bool operator==(int a)
	{
		return GetTotal() == a;
	}

	//a�Ɣ�r�i�b���j
	bool operator>=(int a)
	{
		return GetTotal() >= a;
	}

	//a�Ɣ�r�i�b���j
	bool operator>(int a)
	{
		return GetTotal() > a;
	}

	//a�Ɣ�r�i�b���j
	bool operator<=(int a)
	{
		return GetTotal() <= a;
	}

	//a�Ɣ�r�i�b���j
	bool operator<(int a)
	{
		return GetTotal() < a;
	}
};
