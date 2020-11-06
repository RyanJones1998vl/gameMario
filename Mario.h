#pragma once

#include "GameObject.h"
#include "CEnemy.h"
#include "Obstacle.h"
#include "CItem.h"
#include "debug.h"
#define MARIO_WIDTH 14

class CMario : public CGameObject
{
private:
	static CMario * instance;
	int form;
	bool direction;
	int jumpCount, jumpGap;
	int untouchable;
	int transition;
	int kick;
	int from_form;
	bool sudden_stop;
	CTurtle* turtle;
protected:
private:
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
	void setForm(int form);
	void setDirection(bool toRight);
	void setStates(int state=-1, int toRight = -1, int form=-1);
	void moveDuringState(int currentState, int move);
	int getForm() { return form; }
	int getDirection() { return direction; }
	static CMario * getInstance();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void getDamaged();
	bool notTransforming();
	void setTurtle(CTurtle* tt) { this->turtle = tt; }

};
