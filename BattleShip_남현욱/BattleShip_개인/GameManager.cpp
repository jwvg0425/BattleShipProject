#include "stdafx.h"
#include <time.h>
#include "GameManager.h"
#include "Player.h"
#include "Macro.h"
#define PRINT_DATA

GameManager* GameManager::m_Instance = nullptr;


GameManager::GameManager()
{
	RANDOMIZE();

	m_Player1 = new Player();
	m_Player2 = new Player();
}


GameManager::~GameManager()
{
}

GameManager* GameManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new GameManager;
	}

	return m_Instance;
}

void GameManager::ReleaseInstance()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

void GameManager::InitGame()
{
	m_Player1->Init();
	m_Player2->Init();
	m_Player2->PlaceShips();
	m_Turn = 0;
}

void GameManager::RunGame()
{
	int totalTurn = 0;
	int maxTurn = 0;
	int minTurn = 64;

	for (int i = 0; i < 1000; i++)
	{
		InitGame();
		Point AttackPos;
		HitResult HitRes;
		int HitNum = 0;

#ifdef PRINT_DATA
		system("cls");
		m_Player2->PrintShipData();
		printf("turn:%d\n", m_Turn);
		m_Player1->PrintEnemyBoardData();
		getchar();
#endif
		
		while (!m_Player2->IsDead())
		{
			AttackPos = m_Player1->GetNextAttackPoint();
			HitRes = m_Player2->SendAttackResult(AttackPos);
			m_Player1->RecieveAttackResult(AttackPos, HitRes);
			m_Turn++;
#ifdef PRINT_DATA
			system("cls");
			m_Player2->PrintShipData();
			printf("turn:%d\n", m_Turn);
			m_Player1->PrintEnemyBoardData();
			getchar();
#endif
		}
		printf("%d\n", m_Turn);
		totalTurn += m_Turn;

		if (m_Turn > maxTurn)maxTurn = m_Turn;
		if (m_Turn < minTurn)minTurn = m_Turn;
		

	}
	
	printf("average : %d.%03d max : %d min : %d\n", totalTurn / 1000, totalTurn % 1000, maxTurn, minTurn);
}
