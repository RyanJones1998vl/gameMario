#include "KeyHandler.h"
CKeyHandler* CKeyHandler::instance = NULL;

void CKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		CMario::getInstance()->SetState(MARIO_STATE_JUMP);
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
		CMario::getInstance()->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		CMario::getInstance()->SetState(MARIO_STATE_WALKING_LEFT);
	else CMario::getInstance()->SetState(MARIO_STATE_IDLE);
}
CKeyHandler* CKeyHandler::getInstance() {
	if (instance == NULL) {
		instance = new CKeyHandler();
	}
	return instance;
}