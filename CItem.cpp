#include "CItem.h"
vector<CItem*> CItems::items;
void Mushroom::Update(DWORD dt) {
	CGameObject::Update(dt);
	vector<LPGAMEOBJECT>* coObjects = new vector<LPGAMEOBJECT>();
	coObjects->clear();
	coObjects->insert(coObjects->end(), CObstacles::obstacles.begin(), CObstacles::obstacles.end());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (state == STATE_ITEM_ENABLE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		

		if (!Utils::equalTo(vy, MUSHROOM_MAX_FALLING_SPEED))
			SetVY(Utils::getAcceleratedSpeed(vy, MARIO_GRAVITY, MUSHROOM_MAX_FALLING_SPEED));
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

				CObstacle* obstacle = dynamic_cast<CObstacle*>(e->obj);
				if (abs(e->nx) == 1.0f && abs(e->ny) == 1.0f) {
					if (vy < 0) {
						if (y > init_Y - BBOX_TILE_HEIGHT) y += dy;
						else {
							SetVY(0);
							vx = -MARIO_WALKING_SPEED;
						}
					}
					if (dynamic_cast<Ground*>(e->obj)) {
						SetVY(0);
						if (dy < 0) y += dy;
					}
					x += dx;
				}
		}
	}
}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_TILE_WIDTH ;
	bottom = y + BBOX_TILE_HEIGHT;
}
Mushroom::Mushroom(float x, float y) {
	this->x = x;
	this->y = y;
	this->init_Y = y;
	this->vy = -MUSHROOM_Y_SPEED;
	this->vx = 0;
	this->state = STATE_ITEM_ENABLE;
	CItems::items.push_back(this);
}

//void Mushroom::Update(DWORD dt) {
//	CItem::Update(dt);
//
//	/*switch (state) {
//	case STATE_ITEM_ENABLE:
//
//	}*/
//}
void Mushroom::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(3, SPR_OO_MUSHROOM, state));
	ani->Render(x, y);
	RenderBoundingBox();
}
void CItems::Update(DWORD dt) {
	for (CItem* e : items) {
		if (dynamic_cast<Mushroom*>(e)) dynamic_cast<Mushroom*>(e)->Update(dt);
	}
}
void CItems::Render() {
	for (CItem* e : items) {
		if (dynamic_cast<Mushroom*>(e))
			dynamic_cast<Mushroom*>(e)->Render();
	}
}