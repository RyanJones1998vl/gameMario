#include "Obstacle.h"
vector<CObstacle*> CObstacles::obstacles;

void CObstacles::generateObstacles() {
	Ground::generateGround(80, 5, 140, 1);
	IronSquare* sq = new IronSquare(130, 80);
}

Ground::Ground(float x, float y, int position) {
	this->x = x;
	this->y = y;
	this->position = position;

	xDeflectSpeed = 0.0f;
	yDeflectSpeed = 0.0f;
	CObstacles::obstacles.push_back(this);
}
void Ground::generateGround(int x, int count_x, int y, int count_y) {
	Ground* g;
	for (int i = 0;i < count_x;i++) {
		for (int j = 0;j < count_y;j++) {
			 g = new Ground(x + i * TILE_SQUARE_MEASURE-1, y + j * TILE_SQUARE_MEASURE-1, 
				 i==0?STATE_GROUND_LEFT:
				 (i==count_x-1?STATE_GROUND_RIGHT: STATE_GROUND_MIDDLE));
		}
	}
}

void Ground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MEANINGLESS_BOUNDARY;
	top = y - MEANINGLESS_BOUNDARY;
	right = x + BBOX_TILE_WIDTH + MEANINGLESS_BOUNDARY;
	bottom = y + BBOX_TILE_HEIGHT+ MEANINGLESS_BOUNDARY;
}

void Ground::Render() {
	LPANIMATION ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(2, SPR_OO_GROUND, position));
	ani->Render(x, y);
	RenderBoundingBox();
}

IronSquare::IronSquare(float x, float y) {
	this->x = x;
	this->y = y;
	state = STATE_IRON_SQUARE_NEW;

	xDeflectSpeed = 0.0f;
	yDeflectSpeed = 0.0f;
	CObstacles::obstacles.push_back(this);
}

void IronSquare::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MEANINGLESS_BOUNDARY;
	top = y - MEANINGLESS_BOUNDARY;
	right = x + BBOX_TILE_WIDTH + MEANINGLESS_BOUNDARY;
	bottom = y + BBOX_TILE_HEIGHT + MEANINGLESS_BOUNDARY;
}

void IronSquare::Render() {
	LPANIMATION ani = CAnimations::GetInstance()->Get(Utils::getAnimationId(2, SPR_OO_IRON_SQUARE, state));
	ani->Render(x, y);
	RenderBoundingBox();
}
void IronSquare::generateItem(int marioForm) {
	if (state == STATE_IRON_SQUARE_NEW) {
		CItem* item;
		switch (marioForm) {
		case FORM_MARIO_SMALL:
			item = new Mushroom(x, y);
			state = STATE_IRON_SQUARE_BOUNDED;
			break;
		}
	} 
}


void CObstacles::Render() {
	for (CObstacle* e : obstacles) {
		if (dynamic_cast<Ground*>(e)) dynamic_cast<Ground*>(e)->Render();
		if (dynamic_cast<IronSquare*>(e)) dynamic_cast<IronSquare*>(e)->Render();
	}
}
void CObstacles::Update(DWORD dt) {
	for (CObstacle* e : obstacles) {
		if (dynamic_cast<Ground*>(e)) dynamic_cast<Ground*>(e)->Update(dt);
		if (dynamic_cast<IronSquare*>(e)) dynamic_cast<IronSquare*>(e)->Update(dt);
	}
}
void CObstacle::GetSpeed(float& vx, float& vy, CGameObject* obj) {
	//vx = (this->x > obj->x) ? -STATIC_SPEED : STATIC_SPEED;
	vx = 0;
	vy = 0;
}