#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <vector>
#include "Sprites.h"
#include "Game.h"
#include"debug.h"

using namespace std;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;	
	DWORD dt;


	int nx;

	int state;

	static vector<LPANIMATION> animations;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetVY(float vy) { this->vy = vy; /*DebugOut(L"set vy %f\n", vy);*/
	}
	void SetVX(float vx) { this->vx = vx; DebugOut(L"set vx %f\n", vx); }

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }
	float GetX() { return this->x; }
	float GetY() { return this->y; }


	static void AddAnimation(int aniId);

	CGameObject();

	void Update(DWORD dt);
	void Render();
	~CGameObject();

	void RenderBoundingBox(float shiftTop = 0.0f, float shiftLeft = 0.0f);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);
	void GetSpeed(float& vx, float& vy, CGameObject* object);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
};

