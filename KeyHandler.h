#pragma once
#include"Game.h"
#include<dinput.h>
#include"Mario.h"

class CKeyHandler : public CKeyEventHandler
{

	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
private:
	static CKeyHandler* instance;
public:
	static CKeyHandler* getInstance();
};

