#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "GameObject.h"

class ManagerTemp :
	public GameObject
{
protected:
	std::list<int*> Child;
private:
	static class Model* m_model;//g‚¤‰Â”\«’á
	Float3 m_vel;
	Float3 startpos;
	int Childs = 0;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set();
};
