#include "stdafx.h"
#include <time.h>
#include "GameManager.h"
#include "Player.h"
#include "Macro.h"
//#define PRINT_DATA

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
	Point pos[64];

	for (int i = 0; i < 100000; i++)
	{
		m_Player1->Init(false);
		m_Player2->Init(false);
		m_Player2->PlaceShips();
		m_Turn = 0;

		Point AttackPos;
		HitResult HitRes;

#ifdef PRINT_DATA
			system("cls");
			m_Player2->PrintShipData();
			printf("turn:%d\n", m_Turn);
			m_Player1->PrintEnemyBoardData();
			getchar();
#endif
		
		while (!m_Player2->IsDead())
		{
			AttackPos = m_Player1->GetNextAttackPos();
			HitRes = m_Player2->SendAttackResult(AttackPos);
			m_Player1->RecieveAttackResult(AttackPos, HitRes);
			pos[m_Turn] = AttackPos;
			m_Turn++;

#ifdef PRINT_DATA

				system("cls");
				m_Player2->PrintShipData();
				printf("turn:%d\n", m_Turn);
				m_Player1->PrintEnemyBoardData();
				getchar();
#endif
		}
		//printf("%d\n", m_Turn);
		totalTurn += m_Turn;
		turns[m_Turn - 1]++;
		for (int a = 0; a<m_Turn; a++)
		{
			for (int b = a+1; b<m_Turn; b++)
			{
				if (pos[a] == pos[b])
				{
					m_Player2->PrintShipData();
					printf("shit! %d %d %c%c",a,b,pos[a].x,pos[a].y);
					getchar();
					for (int c = 0; c< m_Player1->m_SecondFindPos[0].size(); c++)
					{
						printf("%c%c\n", m_Player1->m_SecondFindPos[0][c].x, m_Player1->m_SecondFindPos[0][c].y);
					}
					printf("\n");
					for (int c = 0; c< m_Player1->m_FindPos[0].size(); c++)
					{
						printf("%c%c\n", m_Player1->m_FindPos[0][c].x, m_Player1->m_FindPos[0][c].y);
					}
				}
			}
		}

		if (m_Turn > maxTurn)maxTurn = m_Turn;
		if (m_Turn < minTurn)minTurn = m_Turn;
		if (i % 100 == 0)
		{
			printf("%d\n", i);
		}
	
	}
	
	printf("average : %d.%05d max : %d min : %d \n", totalTurn / 100000, totalTurn % 100000, maxTurn, minTurn);
	
	for (int i = 15; i < 64; i++)
	{
		printf("%d turn : %d\n", i + 1, turns[i]);
	}
}
