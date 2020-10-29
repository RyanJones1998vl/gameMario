#pragma once

#include "GameObject.h"

#define MARIO_WIDTH 14

class CMario : public CGameObject
{
private:
	static CMario * instance;
	int form;
	bool direction;
	int jumpCount, jumpGap;
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
};
