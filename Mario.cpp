#include "Game.h"
#include "Mario.h"
CMario* CMario:: instance = NULL;
CMario::CMario(float x, float y, float vx)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = 0;
	state = STATE_MARIO_WALK;
	form = FORM_MARIO_SMALL;
	jumpGap = 0;
	jumpCount = 0;
	direction = true;
};

void CMario::Update(DWORD dt)
{
	/*x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}*/
	CGameObject::Update(dt);
	//if (vy != 0) DebugOut(L"dt %d", dt);
	// simple fall down
	/*vy += MARIO_GRAVITY;
	if (y > 100)
	{
		vy = 0; y = 100.0f;
	}*/

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	/*if (vx>0)*/ 
	/*int aID = Utils::getAnimationId(direction, SPR_OO_MARIO, state, form);
	CAnimations* anis = CAnimations::GetInstance();*/
	/*if (state == STATE_MARIO_LANDING) {
		DebugOut(L"AniId: %d, countFrame: %d, currentFrame: %d \n", aID, ani->frames.size(), ani->currentFrame);
	}*/
	ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction, SPR_OO_MARIO, state, form));
	int _x=x, _y=y;
	Utils::alignAnimationPosition(&_x, &_y);
	ani->Render(_x, _y);
}
CMario * CMario::getInstance() {
	if (CMario::instance == NULL) {
		instance = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX);
	}
	return instance;
}
void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case STATE_MARIO_WALK:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case STATE_MARIO:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (y == 100)
			vy = -MARIO_JUMP_SPEED_Y;

	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}*/
}
void CMario::setForm(int form) {
	this->getInstance()->form = form;
}
void CMario::setDirection(bool toRight) {
	this->getInstance()->direction = toRight;
}
void CMario::setStates(int new_sta, int toRight, int form) {
	int s = this->state;
	if (new_sta > -1) SetState(new_sta);
	if (form > -1) setForm(form);
	if (toRight > -1) setDirection(toRight == 1);
	switch (new_sta) {
	case STATE_MARIO_WALK:
		if (s == STATE_MARIO_LANDING || s == STATE_MARIO_JUMP) {
			moveDuringState(s, new_sta);
			break;
		}
		if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED))) {
			vx = Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED);
		}
		break;
	case STATE_MARIO_JUMP:
		if (Utils::equalTo(vy, 0)) {
			vy = -MARIO_JUMP_SPEED_Y; jumpCount = 0;
		}
		if (s == STATE_MARIO_JUMP) 	
			moveDuringState(s, new_sta);
		else if (s == STATE_MARIO_LANDING) {
			moveDuringState(s, new_sta);
			break;
		}
		else if (STATE_MARIO_WALK){
			moveDuringState(s, new_sta);
		}
		vy = Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0);
		if (Utils::equalTo(vy, 0)) 
			setStates(STATE_MARIO_LANDING);
		break;
	case STATE_MARIO_LANDING:
		vy = Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y);
		if (Utils::equalTo(vy, MARIO_JUMP_SPEED_Y)) 
			setStates(STATE_MARIO_STAND);
		break;
	case STATE_MARIO_STAND:
		if (s == STATE_MARIO_JUMP || (s == STATE_MARIO_LANDING && !Utils::equalTo(vy, MARIO_JUMP_SPEED_Y))) {
			moveDuringState(s, new_sta);
			break;
		} else 
			vy = 0;
		if (!Utils::equalTo(vx, 0)) {
			vx = Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, 0);
		}
		break;

	}

	/*if(state == STATE_MARIO_LANDING) 
		DebugOut(L"CMario::setStates new_sta: %f\n", state);
	if (state == STATE_MARIO_JUMP)
		DebugOut(L"CMario::setStates new_sta: %f\n", state);*/
}
void CMario::moveDuringState(int sta, int movement) {
	switch (sta) {
	case STATE_MARIO_JUMP:
		if (movement == STATE_MARIO_WALK) {
			vy = Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0);
			if (Utils::equalTo(vy, 0)) sta = STATE_MARIO_LANDING;
			if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP))) {
				vx = Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION_AT_JUMP, direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP);
			}
		}
		else if (movement == STATE_MARIO_STAND) {
			vy = Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0);
			if (Utils::equalTo(vy, 0)) {
				setStates(STATE_MARIO_LANDING);
				return;
			}
		}
		else if (movement == STATE_MARIO_JUMP) {
			if (!vy > -MARIO_Y_SPEED_BOUNDARY && jumpCount<MARIO_JUMP_COUNT_MAX && jumpGap==MARIO_JUMP_GAP_MAX) {
				vy -= MARIO_GRAVITY*2;
				jumpGap = 0;
			}
			else jumpGap++;
		}
		else if (movement == STATE_MARIO_WALK) {

		}
		break;
	case STATE_MARIO_LANDING:
		if (movement == STATE_MARIO_WALK) {
			if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP)))
				vx = Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION_AT_JUMP, direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP);
		}
		else if (movement == STATE_MARIO_STAND) {
		}
		else if (movement == STATE_MARIO_JUMP) {
		}
		vy = Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y);
		if (Utils::equalTo(vy, MARIO_JUMP_SPEED_Y)) {
			sta = STATE_MARIO_STAND; vy = 0;
		}
		break;
	}
	SetState(sta);
}