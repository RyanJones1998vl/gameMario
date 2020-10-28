#include "Sprites.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom);
}

//void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
//{
//	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
//	sprites[id] = s;
//}

LPSPRITE CSprites::Get(int id)
{
	if (sprites.count(id) != 0) return sprites[id];
	else return NULL;
}
LPSPRITE CSprites::GetDump()
{
	return sprites[SPR_OO_DUMP];
}
// Modifications
void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	if (sprites.count(id)>0) {
		OutputDebugString(L"[PARA_ERROR] Csprites::Add() : id already exists;");
		return;
	}
	else {
		LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
		sprites[id] = s;
	}
	
}
void CSprites::AddForMario(string textPath, LPDIRECT3DTEXTURE9 tex) {
	string line;
	ifstream file(textPath);
	if(sprites.count(SPR_OO_DUMP)==0) 
		CSprites::GetInstance()->Add(SPR_OO_DUMP, 0, 0, 1, 1, tex);
	if (file.is_open())
	{
		vector<int> left, right;
		int spId, atLine, top, bottom, id;
		while (getline(file, line))
		{
			Utils().analyseAxesForMario(line,&atLine, &top, &bottom, &left, &right);
		/*	getline(file, line);
			Utils().analyseXAxis(line, &left, &right);*/
			for (int i = 0;i < left.size();i++) {
				id = i < left.size() / 2 ? (left.size() / 2 - i) : (i - left.size() / 2);
				spId = Utils::getSpriteId(left[i]< TEX_LEFT_CENTER_MARIO, SPR_OO_MARIO, id, atLine);
				CSprites::GetInstance()->Add(spId, left[i], top, right[i], bottom, tex);
			}
		}
		file.close();
	}

}

void CSprites::AddForEnemies(string textPath, LPDIRECT3DTEXTURE9 tex) {
	string line;
	ifstream file(textPath);
	if (file.is_open())
	{
		vector<int> left, right;
		int spId, emId, top, bottom, id, atLine=0;
		while (getline(file, line))
		{
			Utils().analyseAxesForEnemies(line, &emId, &top, &bottom, &left, &right);
			/*	getline(file, line);
				Utils().analyseXAxis(line, &left, &right);*/
			for (int i = 0;i < left.size();i++) {
				id = i < left.size() / 2 ? (left.size() / 2 - i) : (i - left.size() / 2);
				spId = Utils::getSpriteId(left[i] < TEX_LEFT_CENTER_ENEMIES, emId, id, atLine/10);
				CSprites::GetInstance()->Add(spId, left[i], top, right[i], bottom, tex);
			}
			atLine += 1;
		}
		file.close();
	}
}

// Modification 
void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t=this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame;
	if (sprite != NULL) frame = new CAnimationFrame(sprite, t);
	else frame = new CAnimationFrame(CSprites::GetInstance()->GetDump(), t);

	frames.push_back(frame);
}

void CAnimation::Render(float x, float y)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1) 
	{
		currentFrame = 0; 
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}
		
	}

	frames[currentFrame]->GetSprite()->Draw(x, y);
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	if (animations.count(id) != 0) {
		OutputDebugString(L"[PARA_ERROR] Csprites::Add() : id already exists;");
		return;
	}
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	if(animations.count(id)!=0)	return animations[id];
	return getDump();
}
LPANIMATION CAnimations::getDump() {
	if (animations.count(ANI_DUMP) == 0) {
		LPANIMATION ani = new CAnimation(100);
		ani->Add(ANI_DUMP);
		__instance->Add(ANI_DUMP, ani);
	}
	return animations[ANI_DUMP];
}
void CAnimations::AddMario() {
	LPANIMATION ani;

	for (int i = 0;i < 2;i++) {
		bool toRight = i % 2 == 0;

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 2));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_WALK, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 4));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 5));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_RUN, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 3));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_JUMP, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 7));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STOP, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 11));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_KICK, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STAND, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 9));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 10));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD_RUN, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 18));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_DIE, FORM_MARIO_SMALL), ani);
		
		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 12));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_SLIDE, FORM_MARIO_SMALL), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 6));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_QUICK_JUMP, FORM_MARIO_SMALL), ani);

		// Form BIG
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 2));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 3));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 2));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_WALK, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 6));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 7));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 8));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 7));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_RUN, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 5));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_JUMP, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 3));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_LANDING, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 10));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STOP, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 16));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_KICK, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STAND, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 13));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 13));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 14));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 15));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 14));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD_RUN, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 17));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_SLIDE, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 9));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_QUICK_JUMP, FORM_MARIO_BIG), ani);

		// Form FIRE
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 2));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 3));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 2));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_WALK, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 6));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 7));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 8));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 7));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_RUN, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 5));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_JUMP, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 3));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_LANDING, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 10));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STOP, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 16));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_KICK, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STAND, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 13));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 13));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 14));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 15));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 14));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD_RUN, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 17));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_SLIDE, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 9));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_QUICK_JUMP, FORM_MARIO_FIRE), ani);
		
		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 8, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 8, 10));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_ATTACK, FORM_MARIO_FIRE), ani);

		// Form Racoon
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 2));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 3));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 2));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_WALK, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 8));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 10));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 9));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_RUN, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 5));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_JUMP, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 3));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_JUMP, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 14));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STOP, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 4));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_KICK, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_STAND, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 2));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 3));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 2));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_HOLD_RUN, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 10, 5));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_SLIDE, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 11));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_QUICK_JUMP, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 11));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 12));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 13));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 12));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_FLY, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 16));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 15));
		ani->Add(Utils::getSpriteId(!toRight, SPR_OO_MARIO, 9, 16));
		ani->Add(Utils::getSpriteId(!toRight, SPR_OO_MARIO, 9, 17));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 16));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_ATTACK, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 11));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 12));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 13));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 12));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_FLOAT, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 6));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 3));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_START_FLOATING, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_BIG, FORM_MARIO_SMALL), ani);
		
		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 4, 9));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 1, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_SMALL, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_FIRE, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_RACOON, FORM_MARIO_BIG), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 7, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_FIRE, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 9, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_RACOON, FORM_MARIO_FIRE), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_BIG, FORM_MARIO_RACOON), ani);

		ani = new CAnimation(100);
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		ani->Add(Utils::getSpriteId(toRight, SPR_OO_MARIO, 3, 1));
		CAnimations::GetInstance()->Add(Utils::getAnimationId(toRight, SPR_OO_MARIO, STATE_MARIO_TRANSFORM_TO_BIG, FORM_MARIO_FIRE), ani);
	}
	
}