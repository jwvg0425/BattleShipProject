// BattleShip_PP.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameManager.h"


int _tmain()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameManager::GetInstance()->InitGame();
	GameManager::GetInstance()->RunNetworkGame();
	GameManager::ReleaseInstance();
	getchar();
	return 0;
}