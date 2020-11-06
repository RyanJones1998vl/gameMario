#pragma once
#include"GameObject.h"
class CEnemy:public CGameObject
{
public:
	int form;
public:
	CEnemy(float x, float y, int form);
	void getEffect(int direction);
	void Update(DWORD dt);
};

class CTurtle :public CEnemy {
private:
	int left, right;
	int direction;
	int timesRevive;
	int timeTransition;
	int untouch;
public:
	CTurtle(float x, float y, int left, int right, int form = SPR_OO_TURTLE_GREEN);
	void Update(DWORD dt);
	void Render();
	void getEffect(int direction);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void setState(int state);
};

class CEnemies {
public:
	static vector<CEnemy*> enemies;
	static void generateEnemies();
	static void Update(DWORD dt);
	static void Render();
};