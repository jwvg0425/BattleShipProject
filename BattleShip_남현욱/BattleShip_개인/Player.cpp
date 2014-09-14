#include "stdafx.h"
#include "Player.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Macro.h"
#include "Board.h"


Player::Player()
{
	m_ShipList.push_back(new AirCraft());
	m_ShipList.push_back(new BattleShip());
	m_ShipList.push_back(new Cruiser());

	for (int i = 0; i < 2; ++i)
	{
		m_ShipList.push_back(new Destroyer());
	}

	m_MyBoard = new Board();
	m_EnemyBoard = new Board();
	m_GameCount = -1;
	LoadData();
}


Player::~Player()
{
	for (auto& ship : m_ShipList)
	{
		delete ship;
	}
	
	m_ShipList.clear();
	m_DestroyData.clear();
	delete m_MyBoard;
	delete m_EnemyBoard;
	
	for (int i = 0; i < SAVING_DATA_NUM; i++)
	{
		m_GameData[i].clear();
	}
}

void Player::Init(bool isReset)
{
	m_PrevAttackPos.x = 0;
	m_PrevAttackPos.y = 0;
	m_AIState = HUNT;
	m_HitCount = 0;
	m_AttackCount = 0;

	if (isReset)
	{
		m_GameCount = -1;
		for (int i = 0; i < SAVING_DATA_NUM; i++)
		{
			m_GameData[i].clear();
		}
		LoadData();
	}
	else
	{
		m_GameCount++;
		if (m_GameCount >= SAVING_DATA_NUM)
		{
			m_GameData[m_GameCount %SAVING_DATA_NUM].clear();
		}
	}

	for (auto& ship : m_ShipList)
	{
		ship->Init();
	}
	m_MyBoard->Init();
	m_EnemyBoard->Init();
	m_NumOfEnemyShips[AIRCRAFT] = ShipData::GetNum(AIRCRAFT);
	m_NumOfEnemyShips[BATTLESHIP] = ShipData::GetNum(BATTLESHIP);
	m_NumOfEnemyShips[CRUISER] = ShipData::GetNum(CRUISER);
	m_NumOfEnemyShips[DESTROYER] = ShipData::GetNum(DESTROYER);
	m_DestroyData.clear();
	m_PlaceType = RANDOM;
}

HitResult Player::SendAttackResult(Point pos)
{
	_ASSERT(pos.x != 0 && pos.y != 0);
	HitResult res;

	for (auto& ship : m_ShipList)
	{
		res = ship->HitCheck(pos);
		if ( res != MISS)
		{
			return res;
		}
	}

	return MISS;
}

void Player::RecieveAttackResult(Point pos, HitResult res)
{
	if (res == MISS)
	{
		m_EnemyBoard->SetCellState(pos, MISS_STATE);
	}
	else
	{
		m_EnemyBoard->SetCellState(pos, HIT_STATE);
		m_GameData[m_GameCount % SAVING_DATA_NUM].push_back(pos);
	}

	if (res > DESTROY)
	{
		m_DestroyData[pos] = res;
		m_NumOfEnemyShips[ShipData::GetType(res)]--;
	}
	
	m_PrevAttackPos = pos;

	UpdateAIState(res);
}

bool Player::IsDead()
{
	int playerHP = 0;

	for (auto& ship : m_ShipList)
	{
		playerHP += ship->GetHP();
	}
	
	return (playerHP == 0);
}

void Player::PrintShipData()
{
	for (auto& ship : m_ShipList)
	{
		ship->PrintData();
	}
}

void Player::PrintEnemyBoardData()
{
	m_EnemyBoard->Print();
}

void Player::LoadData()
{
	FILE* file = fopen("data.txt", "r");
	char buffer[256];
	
	if (file == nullptr)
	{
		return;
	}

	while (fgets(buffer, 256, file) != NULL)
	{
		m_GameCount++;

		for (int j = 0; j < 16; j++)
		{
			m_GameData[m_GameCount].push_back(Point(buffer[j*3], buffer[j*3+1]));
		}
	}

	fclose(file);
}

