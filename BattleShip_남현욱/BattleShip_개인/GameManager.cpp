#include "stdafx.h"
#include <time.h>
#include "GameManager.h"
#include "Player.h"
#include "Macro.h"
#define PRINT_DATA 0

GameManager* GameManager::m_Instance = nullptr;


GameManager::GameManager()
{
	RANDOMIZE(); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000f);
	m_Player1 = new Player;
	m_Player2 = new Player;
}


GameManager::~GameManager()
{
	delete m_Player1;
	delete m_Player2;
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
	m_Player1->Init(true);
	m_Player2->Init(true);
	m_Player2->PlaceShips();
	m_Turn = 0;
}

void GameManager::RunGame()
{
	int totalTurn = 0;
	int maxTurn = 0;
	int minTurn = 64;
	int turns[64] = { 0, };
	const int GAME_NUMBER = 10;

	for (int i = 0; i < GAME_NUMBER; i++)
	{
		m_Player1->Init(false);
		m_Player2->Init(false);
		m_Player2->PlaceShips();
		m_Turn = 0;

		Point AttackPos;
		HitResult HitRes;

		while (!m_Player2->IsDead())
		{
			if (PRINT_DATA)
			{
				Print();
			}

			AttackPos = m_Player1->GetNextAttackPos();
			HitRes = m_Player2->SendAttackResult(AttackPos);
			m_Player1->RecieveAttackResult(AttackPos, HitRes);
			m_Turn++;
		}
		totalTurn += m_Turn;
		turns[m_Turn - 1]++;


		if (m_Turn > maxTurn)maxTurn = m_Turn;
		if (m_Turn < minTurn)minTurn = m_Turn;

		printf("Game #%d : %d\n", i + 1, m_Turn);
	}

	printf("average : %g max : %d min : %d \n", (double)totalTurn / GAME_NUMBER, maxTurn, minTurn);


	for (int i = 15; i < 64; i++)
	{
		printf("%d turn : %d\n", i + 1, turns[i]);
	}
}

void GameManager::Print()
{
	COORD start = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);
	m_Player2->PrintShipData();
	printf("turn:%3d\n", m_Turn);
	m_Player1->PrintEnemyBoardData();
	getchar();
}
