// BattleShip_PP.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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