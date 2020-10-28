#pragma once

#include "GameObject.h"

#define MARIO_WIDTH 14

class CMario : public CGameObject
{
private:
	static CMario * instance;
protected:
private:
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
	void SetState(int state);

	static CMario * getInstance();
};
