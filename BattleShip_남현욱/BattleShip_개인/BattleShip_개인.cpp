// BattleShip_PP.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameManager.h"


int _tmain()
{

	GameManager::GetInstance()->InitGame();
	GameManager::GetInstance()->RunGame();
	getchar();
	return 0;
}