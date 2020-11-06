#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Utils.h"
using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left; 
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public: 
	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

public:
	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	LPSPRITE GetDump();


	static CSprites * GetInstance();
	// Modification
	void AddForMario(string textPath, LPDIRECT3DTEXTURE9 tex);
	void AddForEnemies(string textPath, LPDIRECT3DTEXTURE9 tex);
	void AddForTiles(string textPath, LPDIRECT3DTEXTURE9 tex);
	void AddForItems(LPDIRECT3DTEXTURE9 tex);
};

/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
public:
	int currentFrame;
	int id;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	LPANIMATION getDump();

	static CAnimations * GetInstance();
	void AddMario();
	void AddEnemies();
	void AddTiles();
	void AddItems();
};

