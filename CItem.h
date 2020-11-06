#pragma once
#include "GameObject.h"
#include"Obstacle.h"
class CItem:public CGameObject
{
public:
	//void Update(DWORD dt);
};

class Mushroom:public CItem {
	float init_Y;
public:
	Mushroom(float x, float y);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	void Render();
};
class CItems :public CGameObject
{
public:
	static CItems* instance;
public:
	static vector<CItem*> items;
	static void Update(DWORD dt);
	static void Render();
};

