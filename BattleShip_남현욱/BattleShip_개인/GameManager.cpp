#include "stdafx.h"
#include <time.h>
#include "GameManager.h"
#include "Player.h"
#include "Macro.h"
#include "ShipData.h"
#include <locale.h>
#define PRINT_DATA

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
	SAFE_DELETE(m_Player1);
	SAFE_DELETE(m_Player2);
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
	SAFE_DELETE(m_Instance);
}

void GameManager::InitGame()
{
	Network::Initialize();
	m_Player1->Init(true);
	m_Player1->PlaceShips();

	m_Turn = 0;
}

void GameManager::RunNetworkGame()
{
	Network network;

	try
	{
		//network.Connect("10.73.43.26", 9001);
		network.Connect("10.73.42.117", 9000);
	}
	catch(Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("서버와 연결에 실패했습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		}
		return;
	}

	printf("연결됨\n");

	try
	{
		if (network.SubmitName(L"남현욱",141026) == ET_DUPLICATED_NAME)
		{
			printf("이름 중복\n");
			return;
		}
		printf("이름 제출\n");
		Network::GameStartData gameStartData;
		puts("게임 시작 대기중");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"매칭되었습니다. 상대방 이름: %s, 학번: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);

		printf("Game Start\n");

		bool allOver = false;

		while (!allOver)
		{
			PacketType type;
			ErrorType error;
			ShipData shipData;
			char map[64];

			printf("게임 준비\n");
			m_Player1->Init(false);
			m_Player1->PlaceShips();
			m_Turn = 0;
			m_Player1->GetShipPosList().ToMapData(map);
			m_Player1->GetMapData(map);

			if (network.SubmitMap(map) == ET_INVALID_MAP)
			{
				printf("맵 생성 에러\n");
				return;
			}

			bool gameOver = false;

			while (!gameOver)
			{
				Network::AttackResultData attackResult;
				Network::GameResultData gameResult;

				error = network.GetPacketType(&type);

#ifdef PRINT_DATA
				system("cls");
				m_Player1->PrintShipData();
				m_Player1->PrintEnemyBoardData();
#endif
				
				switch (type)
				{
				case PKT_SC_ERROR:
					printf("상대방 접속 종료");
					return;

				case PKT_SC_MY_TURN:

					while (true)
					{
						Point pos = m_Player1->GetNextAttackPos();

						error = network.SubmitAttack(Coord(pos.x, pos.y));
						printf("%c%c 공격 시도\n", pos.x, pos.y);
						if (error == ET_INVALID_ATTACK)
						{
							printf("거기 때리면 안댐\n");
						}
						else
						{
							break;
						}
					}
					break;

				case PKT_SC_ATTACK_RESULT:

					attackResult = network.GetAttackResult();
					
					if (attackResult.isMine)
					{
						Point pos = Point(attackResult.pos.mX + Board::START_X, attackResult.pos.mY + Board::START_Y);
						HitResult res = (HitResult)attackResult.attackResult;
						printf("%c%c 공격 결과 %d\n", pos.x, pos.y, attackResult.attackResult);
						m_Player1->RecieveAttackResult(pos, res);
					}
					else
					{
						Point pos = Point(attackResult.pos.mX + Board::START_X, attackResult.pos.mY + Board::START_Y);
						printf("%c%c 공격 당함\n", attackResult.pos.mX + Board::START_X, attackResult.pos.mY + Board::START_Y);
						m_Player1->SendAttackResult(pos);
					}
					break;
				case PKT_SC_GAME_OVER:
					gameResult = network.GetGameResult();
					printf("%s, turn:%d    \n", gameResult.isWinner ? "win" : "lose", gameResult.turns);
					gameOver = true;
					break;
				default:
					printf("Error : Packet Type %d received\n", type);
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			network.GetPacketType(&type);

			if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult;
				finalResult = network.GetFinalResult();
				printf("게임 전부 끝남, 결과 : 이긴 횟수 : %d , 평균 턴수 : %f\n", finalResult.winCount, finalResult.avgTurns);
				getchar();

				allOver = true;
			}
			else if (type == PKT_SC_NEXT_GAME)
			{
				//OK
				continue;
			}
			else
			{
				printf("Error : Packet Type %d received\n", type);
				throw Network::UNEXPECTED_PACKET;
				break;
			}
		}
	}
	catch (Network::Exception)
	{
		printf("에러\n");
		return;
	}
}

void GameManager::RunSingleGame()
{
	int totalTurn = 0;
	int maxTurn = 0;
	int minTurn = 64;
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

#ifdef PRINT_DATA
			Print();
#endif

			AttackPos = m_Player1->GetNextAttackPos();
			HitRes = m_Player2->SendAttackResult(AttackPos);
			m_Player1->RecieveAttackResult(AttackPos, HitRes);
			m_Turn++;
		}
		totalTurn += m_Turn;

		if (m_Turn > maxTurn)maxTurn = m_Turn;
		if (m_Turn < minTurn)minTurn = m_Turn;

		printf("Game #%d : %d\n", i + 1, m_Turn);
	}

	printf("average : %g max : %d min : %d \n", (double)totalTurn / GAME_NUMBER, maxTurn, minTurn);
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