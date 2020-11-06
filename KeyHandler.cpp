#include "KeyHandler.h"
CKeyHandler* CKeyHandler::instance = NULL;

void CKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		CMario::getInstance()->setStates(STATE_MARIO_JUMP);
		break;
	case DIK_LSHIFT:
		CMario::getInstance()->setStates(STATE_MARIO_RUN);
		break;
	case DIK_A:
		CMario::getInstance()->setStates(STATE_MARIO_KICK);
		break;
	}
}

void CKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode) {
	case DIK_LSHIFT:
		//CMario::getInstance()->setStates(STATE_MARIO_RUN);
		break;
	}

}

void CKeyHandler::KeyState(BYTE* states)
{
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
		CMario::getInstance()->setStates(STATE_MARIO_WALK, MARIO_TO_RIGHT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		CMario::getInstance()->setStates(STATE_MARIO_WALK, MARIO_TO_LEFT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
     		CMario::getInstance()->setStates(STATE_MARIO_JUMP);
	else if(CGame::GetInstance()->IsKeyDown(DIK_L))
		CMario::getInstance()->setStates(STATE_MARIO_TRANSFORM);
	else CMario::getInstance()->setStates(STATE_MARIO_NO_KEY);
}
CKeyHandler* CKeyHandler::getInstance() {
	if (instance == NULL) {
		instance = new CKeyHandler();
	}
	return instance;
}