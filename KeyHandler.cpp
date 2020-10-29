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
	}
}

void CKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CKeyHandler::KeyState(BYTE* states)
{
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
		CMario::getInstance()->setStates(STATE_MARIO_WALK, MARIO_TO_RIGHT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		CMario::getInstance()->setStates(STATE_MARIO_WALK, MARIO_TO_LEFT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
		CMario::getInstance()->setStates(STATE_MARIO_JUMP);
	else CMario::getInstance()->setStates(STATE_MARIO_STAND);
}
CKeyHandler* CKeyHandler::getInstance() {
	if (instance == NULL) {
		instance = new CKeyHandler();
	}
	return instance;
}