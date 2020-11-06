#pragma once
#include "GameObject.h"
#include "CItem.h"
class CObstacle:public CGameObject
{
public:
	float xDeflectSpeed, yDeflectSpeed;
	bool static_vx, static_vy;
	float getDeflectXSpeed() { return xDeflectSpeed; }
	float getDeflectYSpeed() { return yDeflectSpeed; }
	void GetSpeed(float& vx, float& vy, CGameObject* obj);
};
class CObstacles {

public:
	static vector<CObstacle*> obstacles;
	static void generateObstacles();
	static vector<CObstacle*> getObstacles(){ return obstacles; }
	static void Render();
	static void Update(DWORD dt);
	
};
class Ground:public CObstacle {
public: 
	int position;
	static void generateGround(int fromX, int cx, int fromY, int cy);
	Ground(float x, float y, int position);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
class IronSquare :public CObstacle {
public:
	//static void generateSquares(int fromX, int cx, int fromY, int cy);
	IronSquare(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void generateItem(int marioForm);
};
