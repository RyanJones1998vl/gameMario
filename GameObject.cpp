#include "GameObject.h"
#include "Textures.h"
#include "Game.h"
#include <algorithm>
#include "Obstacle.h"

vector<LPANIMATION> CGameObject::animations;

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt)
{
	//this->dt = dt;
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void CGameObject::Render()
{
}

void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}



CGameObject::~CGameObject()
{
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
void CGameObject::GetSpeed(float& vx, float& vy, CGameObject* object) {
	if (object == NULL) {
		vx = this->vx; vy = this->vy;
	}
	else {
		dynamic_cast<CObstacle*>(this)->GetSpeed(vx, vy, object);
	}
}

LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);
	float pre_dy = this->dy;

	/*if (dynamic_cast<Ground*>(coO) && this->dy ==0) 
		this->dy = MARIO_GRAVITY*2 * dt;*/

	GetBoundingBox(ml, mt, mr, mb);
	
	
	CCollisionEvent* e;
	if ((mr >= sl && mr <= sr&& mt + MARIO_GRAVITY * 2 * dt <= sb +1 && mt + MARIO_GRAVITY * 2 * dt >= st)) {
		e = new CCollisionEvent(min(min(mr - sl, sr - mr) / 100, (sb - mt - MARIO_GRAVITY * 2 * dt)/ 100), 1.0f, -1.0f, coO);
		return e;
	}
	if ((mr >= sl && mr <= sr && mb + MARIO_GRAVITY * 2 * dt <= sb + 1 && mb+ MARIO_GRAVITY * 2 * dt >= st)) {
		e = new CCollisionEvent(min(min(mr - sl,sr-mr) / 100, (mb + MARIO_GRAVITY * 2 * dt - st) / 100), 1.0f, -1.0f, coO);
		return e;
	}
	if ((ml >= sl && ml <= sr && mt + MARIO_GRAVITY * 2 * dt <= sb + 1 && mt + MARIO_GRAVITY * 2 * dt >= st)) {
		e = new CCollisionEvent(min((min(sr - ml, mr-sl)) / 100, (sb - mt - MARIO_GRAVITY * 2 * dt) / 100), 1.0f, 1.0f, coO);
		return e;
	}
	if ((ml >= sl && ml <= sr && mb + MARIO_GRAVITY * 2 * dt <= sb + 1 && mb + MARIO_GRAVITY * 2 * dt >= st)) {
		e = new CCollisionEvent(min((min(sr - ml, mr - sl)) / 100, (mb + MARIO_GRAVITY * 2 * dt -st) / 100), 1.0f, -1.0f, coO);
		return e;
	}
	//if (dynamic_cast<Ground*>(coO) && this->dy == MARIO_GRAVITY * 2 * dt) this->dy = pre_dy;

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	if (dynamic_cast<CObstacle*>(coO)) {
		coO->GetSpeed(svx, svy, this);
	}
	else {
		coO->GetSpeed(svx, svy, NULL);
	}

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->vx * dt - sdx;
	float dy = this->vy * dt - sdy;

	CGame::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	e = new CCollisionEvent(t, nx, ny, coO);

	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	//DebugOut(L"CGameObject::CalcPotentialCollisions: %f\n", vy);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::RenderBoundingBox(float shiftTop, float shiftLeft)
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(TEX_ID_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x + shiftLeft, y + shiftTop, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}