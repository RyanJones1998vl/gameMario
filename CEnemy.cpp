#include "CEnemy.h"
#include "Mario.h"
vector<CEnemy*> CEnemies::enemies;

CEnemy::CEnemy(float x, float y, int form) {
	this->x = x;
	this->y = y;
	this->form = form;
}
void CEnemy::getEffect(int direction) {
	if (dynamic_cast<CTurtle*>(this)) dynamic_cast<CTurtle*>(this)->getEffect(direction);
}
void CEnemy::Update(DWORD dt) {
	CGameObject::Update(dt);
	vector<LPGAMEOBJECT>* coObjects = new vector<LPGAMEOBJECT>();
	coObjects->clear();
	coObjects->insert(coObjects->end(), CObstacles::obstacles.begin(), CObstacles::obstacles.end());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (state != STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (!Utils::equalTo(vy, MARIO_GRAVITY))
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MARIO_JUMP_SPEED_Y));
		
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CObstacle*>(e->obj)) {
				CObstacle* obstacle = dynamic_cast<CObstacle*>(e->obj);
				if (abs(e->nx) == 1.0f && abs(e->ny) == 1.0f) {
					SetVY(0);
					if (dynamic_cast<Ground*>(e->obj)) {
						x += dx;
						if (dy < 0) y += dy;
					}
					continue;
				}
				if (e->ny < 0)
				{
					if (dynamic_cast<Ground*>(e->obj)) SetVY(0);
				}
				
			}
		}
	}
}
CTurtle::CTurtle(float x, float y,int left, int right, int color):CEnemy::CEnemy(x, y, color) {
	this->vx = TUTLE_WALKING_SPEED;
	this->state = STATE_TURTLE_WALK;
	this->direction = true;
	this->left = left;
	this->right = right;
	this->timesRevive;
	CEnemies::enemies.push_back(this);
}
void CTurtle::Update(DWORD dt) {
	if (x > right) {
		direction = 0;
		vx = -TUTLE_WALKING_SPEED;
	} 
	if(x < left){
		direction = 1;
		vx = TUTLE_WALKING_SPEED;
	}
	CEnemy::Update(dt);

	switch (state) {
	case STATE_TURTLE_WALK:
		break;
	case STATE_TURTLE_DIE:
		if (GetTickCount() - this->timesRevive > TURTLE_REVIVE_TIME) {
			state = STATE_TURTLE_TRANSITION;
			timeTransition = GetTickCount();
		}
		break;
	case STATE_TURTLE_KICKED:
		break;
	case STATE_TURTLE_HOLDED:
		x = CMario::getInstance()->x + (CMario::getInstance()->getDirection() ? (BBOX_MARIO_WIDTH) :-BBOX_MARIO_WIDTH);
		break;
	case STATE_TURTLE_TRANSITION:
		if (GetTickCount() - timeTransition > TURTLE_TRANSITION_TIME) setState(STATE_TURTLE_WALK);
	}

}
void CTurtle::Render()
{
	LPANIMATION ani;

	int s = Utils::getAnimationId(direction, form, state);
	ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(direction, form, state));
	int _x = x, _y = y;
	Utils::alignAnimationPosition(&_x, &_y);

	ani->Render(_x, _y);
	RenderBoundingBox();
}
void CTurtle::getEffect(int direction) {
	/*if (direction == EFFECT_DIRECTION_Y) {
		if (state == STATE_TURTLE_WALK) {
			CMario::getInstance()->vy = -MARIO_JUMP_DEFLECT_SPEED;
			state = STATE_TURTLE_DIE;
		}
		else if (state == STATE_TURTLE_DIE) {
			vx = CMario::getInstance()->getDirection()?MARIO_WALKING_SPEED:-MARIO_WALKING_SPEED;
			CMario::getInstance()->state = STATE_MARIO_STAND;
		}
	}
	else {
		if (state == STATE_TURTLE_DIE) {
			if(Utils::equalTo(vx, 0)) vx = CMario::getInstance()->getDirection() ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED;
			else CMario::getInstance()->getDamaged();
		}
		else {
			CMario::getInstance()->getDamaged();
		}
	}*/
	if (GetTickCount() - untouch > TURTLE_UNTOUCHABLE_TIME) {
		if (direction == EFFECT_DIRECTION_Y) {
			if (state == STATE_TURTLE_WALK) {
				CMario::getInstance()->vy = -MARIO_JUMP_DEFLECT_SPEED;
				CMario::getInstance()->y = this->y - (CMario::getInstance()->getForm() == FORM_MARIO_SMALL ? BBOX_MARIO_SMALL_HEIGHT : BBOX_MARIO_BIG_HEIGHT);
				setState(STATE_TURTLE_DIE);
			}
			else if (state == STATE_TURTLE_DIE) {
				CMario::getInstance()->vy = -MARIO_JUMP_DEFLECT_SPEED;
				CMario::getInstance()->y = this->y - (CMario::getInstance()->getForm() == FORM_MARIO_SMALL ? BBOX_MARIO_SMALL_HEIGHT : BBOX_MARIO_BIG_HEIGHT);
				setState(STATE_TURTLE_KICKED);
			}
			else if (state == STATE_TURTLE_KICKED) {
				CMario::getInstance()->vy = -MARIO_JUMP_DEFLECT_SPEED;
				setState(STATE_TURTLE_DIE);
			}
		}
		else if (direction == EFFECT_DIRECTION_X) {
			if(state == STATE_TURTLE_WALK || state == STATE_TURTLE_KICKED)
				CMario::getInstance()->getDamaged();
			else if (state == STATE_TURTLE_DIE) {
				if (CGame::GetInstance()->IsKeyDown(DIK_A) && (CGame::GetInstance()->IsKeyDown(DIK_LEFT) || CGame::GetInstance()->IsKeyDown(DIK_RIGHT))) {
					CMario::getInstance()->setTurtle(this);
					CMario::getInstance()->setStates(STATE_MARIO_HOLD);
					setState(STATE_TURTLE_HOLDED);
				}
				else if((CMario::getInstance()->x  < x && CMario::getInstance()->x >x-BBOX_TURTLE_WIDTH_EXTENTSION)||
					(CMario::getInstance()->x  > x && CMario::getInstance()->x < x + BBOX_TURTLE_WIDTH_EXTENTSION)) {
					CMario::getInstance()->setTurtle(this);
					CMario::getInstance()->setStates(STATE_MARIO_KICK);
					setState(STATE_TURTLE_KICKED);
				}
			}
		}
	}	
}
void CTurtle::setState(int new_state) {
	switch (new_state) {
	case STATE_TURTLE_DIE:
		vx = 0;
		this->timesRevive = GetTickCount();
		untouch = GetTickCount();
		if (state == STATE_TURTLE_WALK) y += TURTLE_HEIGHT_DELTA;
		break;
	case STATE_TURTLE_HOLDED:
		vx = 0;
		break;
	case STATE_TURTLE_KICKED:
		vx = CMario::getInstance()->getDirection() ? MARIO_WALKING_SPEED : -MARIO_WALKING_SPEED;
		break;
	case STATE_TURTLE_TRANSITION:
		timeTransition = GetTickCount();
		break;
	case STATE_TURTLE_WALK:
		vx = GetTickCount() % 2 == 0 ? TUTLE_WALKING_SPEED : -TUTLE_WALKING_SPEED;
		y -= TURTLE_HEIGHT_DELTA;
	}
	state = new_state;
	untouch = GetTickCount();
}
void CTurtle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + BBOX_TURTLE_WIDTH;

	if (state == STATE_TURTLE_WALK) {
		top = y;
		bottom = top + BBOX_TURTLE_HEIGHT;
	}
	else {
		top = y + TURTLE_HEIGHT_DELTA;
		bottom = top + BBOX_TURTLE_HEIGHT_SHELL;
	}
	if (state == STATE_TURTLE_DIE) {
		left = x - BBOX_TURTLE_WIDTH_EXTENTSION;
		right = left + BBOX_TURTLE_WIDTH + BBOX_TURTLE_WIDTH_EXTENTSION*2;
	}
}

void CEnemies::generateEnemies() {
	CTurtle* t = new CTurtle(100, 100,90, 200, SPR_OO_TURTLE_RED);
}
void CEnemies::Update(DWORD dt) {
	for (CEnemy* e : enemies) {
		if (dynamic_cast<CTurtle*>(e)) dynamic_cast<CTurtle*>(e)->Update(dt);
	}
}
void CEnemies::Render() {
	for (CEnemy* e : enemies) {
		if (dynamic_cast<CTurtle*>(e)) 
			dynamic_cast<CTurtle*>(e)->Render();
	}
}