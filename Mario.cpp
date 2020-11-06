#include "Game.h"
#include "Mario.h"
CMario* CMario:: instance = NULL;
CMario::CMario(float x, float y, float vx)
{
	this->x = x;
	this->y = 0;
	this->vx = vx;
	this->vy = 0;
	state = STATE_MARIO_WALK;
	form = FORM_MARIO_SMALL;
	jumpGap = 0;
	jumpCount = 0;
	direction = true;
	untouchable = -1;
	transition = -1;
	kick = -1;
	sudden_stop = false;
	turtle = NULL;
};

bool CMario::notTransforming() {

	switch (state) {
	case STATE_MARIO_TRANSFORM_TO_BIG:
		return (transition == SPR_MARIO_TRANSFORM_TO_BIG_FROM_SMALL_COUNT *2-1 && from_form == FORM_MARIO_SMALL) ||
			((transition == SPR_MARIO_TRANSFORM_TO_BIG_FROM_OTHER_COUNT*2-1 ) && from_form != FORM_MARIO_SMALL);
	case STATE_MARIO_TRANSFORM_TO_SMALL:
		return transition == SPR_MARIO_TRANSFORM_TO_SMALL_COUNT*2-1;
	case STATE_MARIO_TRANSFORM_TO_RACOON:
		return transition == SPR_MARIO_TRANSFORM_TO_RACOON_COUNT*2-1;
	case STATE_MARIO_TRANSFORM_TO_FIRE:
		transition == SPR_MARIO_TRANSFORM_TO_FIRE_COUNT*2-1;
	default: 
		return true;
	}

}
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
	//vy += MARIO_GRAVITY*2;
	CGameObject::Update(dt);

	//if (vy != 0) DebugOut(L"dt %d", dt);
	// simple fall down
	/*vy += MARIO_GRAVITY;
	if (y > 100)
	{
		vy = 0; y = 100.0f;
	}*/

	// simple screen edge collision!!!
	vector<LPGAMEOBJECT>* coObjects = new vector<LPGAMEOBJECT>();
	coObjects->clear();
	coObjects->insert(coObjects->begin(), CObstacles::obstacles.begin(), CObstacles::obstacles.end());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	//vy += MARIO_GRAVITY * 2;
	/*if(x >=76.299 && x <= 76.3)
		DebugOut(L"Mario::Update::no collision == 0 vy = %f \n", vy);*/


	if (state != STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	//vy -= MARIO_GRAVITY * 2;

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//DebugOut(L"Mario::Update::no collision == 0 vy = %f \n", vy);
		if (state == STATE_MARIO_STAND) {
			if (vy < 0) setStates(STATE_MARIO_JUMP);
			else if(vy>=0) setStates(STATE_MARIO_LANDING);
		}
		if (state == STATE_MARIO_STOP) {
			if (vy < 0) setStates(STATE_MARIO_JUMP);
			else if (vy >= 0) setStates(STATE_MARIO_LANDING);
		}
		else if (state == STATE_MARIO_RUN || state == STATE_MARIO_WALK)
			setStates(STATE_MARIO_LANDING);
		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		
		if (abs(nx) == 1.0f && abs(ny) == 1.0f) {
		}
		else {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}
	/*	if (nx != 0) vx = 0;
		if (ny != 0) SetVY(0;*/
		//DebugOut(L"Mario::Update:: coEvents.size() != 0 vy = %f \n", vy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<CEnemy*>(e->obj))
			{
				CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

				if (e->ny < 0)
				{
					enemy->getEffect(EFFECT_DIRECTION_Y);
				}
				else if (e->ny > 0) {

				}
				if (e->nx != 0)
				{
					if (untouchable==-1)
					{
						enemy->getEffect(EFFECT_DIRECTION_X);
					}
				}
			} */
			CObstacle* obstacle = dynamic_cast<CObstacle*>(e->obj);
			if (abs(e->nx) == 1.0f && abs(e->ny) == 1.0f) {
				//DebugOut(L"Mario::Update:: overlapping vy = %f \n", vy);
				if (!(((CGame::GetInstance()->IsKeyDown(DIK_LEFT) || CGame::GetInstance()->IsKeyDown(DIK_RIGHT)) && state!=STATE_MARIO_LANDING) ||
					CGame::GetInstance()->IsKeyDown(DIK_SPACE)) && notTransforming() && state != STATE_MARIO_KICK && state != STATE_MARIO_HOLD && state != STATE_MARIO_HOLD_RUN)
					setStates(STATE_MARIO_STAND);
				if(state == STATE_MARIO_LANDING) 
					setStates(STATE_MARIO_STAND);

				if (!CGame::GetInstance()->IsKeyDown(DIK_SPACE))
					SetVY(0);
				if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<IronSquare*>(e->obj)) {
					x += dx;
					if (dy < 0) y += dy;
				}
				continue;
			}

			if (e->ny < 0)
			{
				SetVY(obstacle->getDeflectYSpeed());
				if (Utils::equalTo(vy, 0)) {
					if (!(CGame::GetInstance()->IsKeyDown(DIK_LEFT) || CGame::GetInstance()->IsKeyDown(DIK_RIGHT)))
						setStates(STATE_MARIO_STAND);
				}
				else if (vy < 0) setStates(STATE_MARIO_JUMP);

			}
			else if (e->ny > 0) {
				SetVY(obstacle->getDeflectYSpeed());
				if (dynamic_cast<IronSquare*>(e->obj)) dynamic_cast<IronSquare*>(e->obj)->generateItem(this->form);
			}


			if (e->nx != 0)
				{
					//x -= min_tx * dx + nx * 0.4f;
					SetVX(obstacle->getDeflectXSpeed());
				}
		
		}
	} 
	
	coObjects->clear();
	coObjects->insert(coObjects->begin(), CEnemies::enemies.begin(), CEnemies::enemies.end());
	coEvents.clear();

	if (state != STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;

 		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (abs(nx) == 1.0f && abs(ny) == 1.0f) {
		}
		else {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}
		LPGAMEOBJECT obj = NULL;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (obj == e->obj) continue;
				CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

				if (GetTickCount() - untouchable > MARIO_UNTOUCHABLE_TIME) {
					if (e->ny < 0)
						if (abs(e->nx) != 1.0) enemy->getEffect(EFFECT_DIRECTION_Y);
						else enemy->getEffect(EFFECT_DIRECTION_X);

					else {
						if (untouchable == -1)
						{
							enemy->getEffect(EFFECT_DIRECTION_X);
						}
					}
				}
				obj = e->obj;
		}
	}

	coObjects->clear();
	coObjects->insert(coObjects->begin(), CItems::items.begin(), CItems::items.end());
	coEvents.clear();
	if (state != STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() != 0) {
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (abs(nx) == 1.0f && abs(ny) == 1.0f) {
		}
		else {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}
		LPGAMEOBJECT obj = NULL;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (obj == e->obj) continue;
			if (dynamic_cast<Mushroom*>(e->obj)) {
				setStates(STATE_MARIO_TRANSFORM_TO_BIG);
				CItems::items.pop_back();
				delete dynamic_cast<Mushroom*>(e->obj);
			}
		}
	}


	bool check;
	if (vy < 0 && notTransforming()) state = STATE_MARIO_JUMP;
	switch (state) {
	case STATE_MARIO_WALK:
		if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED))) {
			SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED));
		}
		if (CGame::GetInstance()->IsKeyDown(DIK_LSHIFT))
			setStates(STATE_MARIO_RUN);
		break;
	case STATE_MARIO_JUMP:
		SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0));
		if (Utils::equalTo(vy, 0))
			setStates(STATE_MARIO_LANDING);
		break;
	case STATE_MARIO_LANDING:
		SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y));
		/*if (Utils::equalTo(vy, MARIO_JUMP_SPEED_Y))
			setStates(STATE_MARIO_STAND);*/
		break;
	case STATE_MARIO_STAND:
		if (!Utils::equalTo(vx, 0)) {
			SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, 0));
		}
		break;
	case STATE_MARIO_STOP:
		if (Utils::equalTo(vx, 0)) {
			if (vy > 0) setStates(STATE_MARIO_LANDING);
			else if (vy < 0) setStates(STATE_MARIO_JUMP);
			else setStates(STATE_MARIO_STAND);
		}
	case STATE_MARIO_TRANSFORM_TO_BIG:
	case STATE_MARIO_TRANSFORM_TO_RACOON:
	case STATE_MARIO_TRANSFORM_TO_SMALL:
	case STATE_MARIO_TRANSFORM_TO_FIRE:
		transition += 1;
		if (notTransforming()) {
			if (state == STATE_MARIO_TRANSFORM_TO_SMALL)
				y += 15;
			if (state == STATE_MARIO_TRANSFORM_TO_BIG && from_form == FORM_MARIO_SMALL)
				y -= 15;
			setStates(STATE_MARIO_STAND);
		}

		SetVX(0); SetVY(0);
		break;
	case STATE_MARIO_KICK:
		kick += 1;
		if (kick == SPR_MARIO_KICK_COUNT * 2 - 1) {
			setStates(STATE_MARIO_STAND);
		}
		break;
	}
	//DebugOut(L"Mario::Update::state = %d \n", state);
}
void CMario::getDamaged() {
	switch (form) {
	case FORM_MARIO_FIRE:
	case FORM_MARIO_RACOON:
		setStates(STATE_MARIO_TRANSFORM_TO_BIG);
		untouchable = GetTickCount();
		break;
	case FORM_MARIO_BIG:
		setStates(STATE_MARIO_TRANSFORM_TO_SMALL);
		untouchable = GetTickCount();
		break;
	case FORM_MARIO_SMALL:
		setStates(STATE_MARIO_DIE);
		break;
	}
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
	
	/*
	else if (transition == -1) {
		ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction, SPR_OO_MARIO, state, form));*/

	if (transition > -1) {
		ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction > 0, SPR_OO_MARIO, state, from_form));
		ani->currentFrame = int((transition - 1) / 2) - 1;
		/*if (ani->currentFrame == ani->frames.size()) 
			ani->currentFrame = 0;*/
		//ani->currentFrame
	} else if (sudden_stop) {
		ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(!direction, SPR_OO_MARIO, STATE_MARIO_STOP, form));
	}
	else if (kick > -1) {
		ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction > 0, SPR_OO_MARIO, state, form));
		ani->currentFrame = int((kick - 1) / 2) - 1;
	}else ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction, SPR_OO_MARIO, state, form));


	int _x = x, _y = y;
	if (state == STATE_MARIO_TRANSFORM_TO_SMALL) 
		if (ani->currentFrame == 19 || ani->currentFrame == 11 || ani->currentFrame == 15) 
			_y += 15;
	if (state == STATE_MARIO_TRANSFORM_TO_BIG && from_form==FORM_MARIO_SMALL)
		if (!(ani->currentFrame == -1 || ani->currentFrame == 3 || ani->currentFrame == 7 || ani->currentFrame == ani->frames.size()-1))
			_y -= 15;
	
	int alpha = 255;
	if (GetTickCount()-untouchable<=MARIO_UNTOUCHABLE_TIME) 
		if (untouchable%2==0) alpha = 128;
	
	ani->Render(_x, _y, alpha);
	RenderBoundingBox(form==FORM_MARIO_SMALL?0:BBOX_MARIO_EMPTY_SPACE);
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
			SetVY(-MARIO_JUMP_SPEED_Y;

	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}*/
}
void CMario::setForm(int form) {
	transition = 1;
	this->getInstance()->form = form;
}
void CMario::setDirection(bool toRight) {
	this->getInstance()->direction = toRight;
}
void CMario::setStates(int new_sta, int toRight, int form) {
 	int s = this->state;
	if (!notTransforming() && new_sta == STATE_MARIO_JUMP)
		DebugOut(L"HEllo");
	if (new_sta > -1 && new_sta!=STATE_MARIO_NO_KEY && notTransforming())
		if (new_sta != STATE_MARIO_TRANSFORM && new_sta != STATE_MARIO_TRANSFORM_TO_SMALL &&
			new_sta != STATE_MARIO_TRANSFORM_TO_RACOON && new_sta != STATE_MARIO_TRANSFORM_TO_BIG && 
			new_sta != STATE_MARIO_TRANSFORM_TO_FIRE) {
			SetState(new_sta); 
			transition = -1;
		} else {
			transition = 0;
			from_form = this->form;
			if (this->form == FORM_MARIO_BIG) {
				this->form = FORM_MARIO_SMALL; 
				state = STATE_MARIO_TRANSFORM_TO_SMALL;
			}
			else if (this->form == FORM_MARIO_SMALL) {
				this->form = FORM_MARIO_BIG; 
				state = STATE_MARIO_TRANSFORM_TO_BIG;
			}
		}
	
	if (form > -1) setForm(form);
	if (toRight > -1) setDirection(toRight == 1);
	/*if (transition == STATE_MARIO_TRANSFORM_TO_BIG && new_sta == STATE_MARIO_STAND)
		DebugOut(L"Mario::Update::no collision == 0 vy = %f \n", vy);*/
	if (transition == -1 && new_sta!=STATE_MARIO_TRANSFORM) {
		switch (new_sta) {
		case STATE_MARIO_RUN:
			//DebugOut(L"STATE_MARIO_RUN");
			if (Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED)) &&
				!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED * 2 : -MARIO_WALKING_SPEED * 2))) {
				SetVX((direction ? MARIO_WALKING_SPEED * 2 : -MARIO_WALKING_SPEED * 2));
				break;
			}
			else if(Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED * 2 : -MARIO_WALKING_SPEED * 2))) {
				break;
			}
		case STATE_MARIO_WALK:
			if (s == STATE_MARIO_LANDING || s == STATE_MARIO_JUMP || s == STATE_MARIO_HOLD_RUN) {
				moveDuringState(s, new_sta);
				break;
			}
			else if (s == STATE_MARIO_HOLD) setStates(STATE_MARIO_HOLD_RUN);
			
			if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED))) {
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, direction ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED));
				if ((vx > 0 && CGame::GetInstance()->IsKeyDown(DIK_LEFT)
					|| vx < 0 && CGame::GetInstance()->IsKeyDown(DIK_RIGHT))&& state != STATE_MARIO_HOLD_RUN) sudden_stop = true;
				else sudden_stop = false;
			}
			if (kick > -1 && kick != SPR_MARIO_TRANSFORM_TO_BIG_FROM_SMALL_COUNT * 2 - 1) state = STATE_MARIO_KICK;
			break;
		case STATE_MARIO_JUMP:
			if (Utils::equalTo(vy, 0)) {
				SetVY(-MARIO_JUMP_SPEED_Y); jumpCount = 0;
			}
			if (s == STATE_MARIO_JUMP)
				moveDuringState(s, new_sta);
			else if (s == STATE_MARIO_LANDING) {
				moveDuringState(s, new_sta);
				break;
			}
			else if (STATE_MARIO_WALK) {
				moveDuringState(s, new_sta);
			}
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0));
			if (Utils::equalTo(vy, 0))
				setStates(STATE_MARIO_LANDING);
			break;
		case STATE_MARIO_LANDING:
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y));
			/*if (Utils::equalTo(vy, MARIO_JUMP_SPEED_Y))
				setStates(STATE_MARIO_STAND);*/
			break;
		case STATE_MARIO_STAND:
			/*if (s == STATE_MARIO_JUMP || (s == STATE_MARIO_LANDING && !Utils::equalTo(vy, MARIO_JUMP_SPEED_Y))) {
				moveDuringState(s, new_sta);
				break;
			} else
				SetVY(0;*/
			if (!Utils::equalTo(vx, 0)) {
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, 0));
			}
			kick = -1;
			break;
		case STATE_MARIO_HOLD:

			break;
		case STATE_MARIO_HOLD_RUN:
			break;
		case STATE_MARIO_KICK:
			if (turtle!=NULL) {
				turtle->setState(STATE_TURTLE_KICKED);
				turtle = NULL;
				kick = 0;
			}
			else state = s;
			break;
		case STATE_MARIO_NO_KEY:
			if (Utils::equalTo(vx, 0) && state == STATE_MARIO_STOP) {
				if (vy > 0) state == STATE_MARIO_LANDING;
				else if (vy < 0) state == STATE_MARIO_JUMP;
				else state = STATE_MARIO_STAND;
			} else moveDuringState(s, new_sta);
			if (!Utils::equalTo(vx, 0)) {
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION, 0));
			}
			break;
		}
	}
	

	/*if(state == STATE_MARIO_LANDING) 
		DebugOut(L"CMario::setStates new_sta: %f\n", state);
	if (state == STATE_MARIO_JUMP)
		DebugOut(L"CMario::setStates new_sta: %f\n", state);*/
	//DebugOut(L"CMario::setStates state: %f\n", this->state);
}
void CMario::moveDuringState(int sta, int movement) {
	switch (sta) {
	case STATE_MARIO_JUMP:
		if (movement == STATE_MARIO_WALK) {
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0));
			if (Utils::equalTo(vy, 0)) sta = STATE_MARIO_LANDING;
			if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP))) {
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION_AT_JUMP, direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP));
			}
			
		}
		else if (movement == STATE_MARIO_STAND || movement==STATE_MARIO_NO_KEY) {
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, 0));
			if (Utils::equalTo(vy, 0)) {
				setStates(STATE_MARIO_LANDING);
				return;
			}
		}
		else if (movement == STATE_MARIO_JUMP) {
			if (!vy > -MARIO_Y_SPEED_BOUNDARY && jumpCount<MARIO_JUMP_COUNT_MAX && jumpGap==MARIO_JUMP_GAP_MAX) {
				SetVY(vy - MARIO_GRAVITY*2);
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
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION_AT_JUMP, direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP));
		}
		else if (movement == STATE_MARIO_STAND || movement == STATE_MARIO_NO_KEY) {
		}
		else if (movement == STATE_MARIO_JUMP) {
		}
		SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y));
		/*if (Utils::equalTo(vy, MARIO_JUMP_SPEED_Y)) {
			sta = STATE_MARIO_STAND; SetVY(0;
		}*/
		break;
	case STATE_MARIO_WALK:
		sta = STATE_MARIO_STAND;
		break;
	case STATE_MARIO_HOLD_RUN:
	case STATE_MARIO_HOLD:
		if (movement == STATE_MARIO_WALK) {
			if (!Utils::equalTo(vx, (direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP)))
				SetVX(Utils::getAcceleratedSpeed(vx, MARIO_SMALL_FRICTION_AT_JUMP, direction ? MARIO_WALKING_SPEED_AT_JUMP : -MARIO_WALKING_SPEED_AT_JUMP));
			sta = STATE_MARIO_HOLD_RUN;
		}
		else if (movement == STATE_MARIO_NO_KEY) 
			sta = STATE_MARIO_HOLD;
	case STATE_MARIO_STOP:
		sta = STATE_MARIO_STAND;
	}
	SetState(sta);
}
void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;

	if (form == FORM_MARIO_SMALL)
	{
		top = y;
		right = x + BBOX_MARIO_SMALL_WIDTH + MEANINGLESS_BOUNDARY;
		bottom = y + BBOX_MARIO_SMALL_HEIGHT + MEANINGLESS_BOUNDARY;
	}
	else
	{
		top = y + BBOX_MARIO_EMPTY_SPACE;
		right = x + BBOX_MARIO_BIG_WIDTH + MEANINGLESS_BOUNDARY;
		bottom = y + BBOX_MARIO_BIG_HEIGHT + MEANINGLESS_BOUNDARY;
	}
}