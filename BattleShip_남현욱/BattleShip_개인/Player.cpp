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
		SAFE_DELETE(ship);
	}
	
	m_ShipList.clear();
	m_DestroyData.clear();
	SAFE_DELETE(m_MyBoard);
	SAFE_DELETE(m_EnemyBoard);
	
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

	//저장되어 있는 게임 데이터까지 완전 초기화
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
	m_NumOfEnemyShips[AIRCRAFT] = ClientShipData::GetNum(AIRCRAFT);
	m_NumOfEnemyShips[BATTLESHIP] = ClientShipData::GetNum(BATTLESHIP);
	m_NumOfEnemyShips[CRUISER] = ClientShipData::GetNum(CRUISER);
	m_NumOfEnemyShips[DESTROYER] = ClientShipData::GetNum(DESTROYER);
	m_DestroyData.clear();

	m_PlaceType = (PlaceType)RANDOM(PLACE_TYPE_NUM);

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
	_ASSERT(pos.x != 0 && pos.y != 0);
	_ASSERT(res != ERROR_RESULT);

	if (res == MISS)
	{
		m_EnemyBoard->SetCellState(pos, MISS_STATE);
	}
	else
	{
		m_EnemyBoard->SetCellState(pos, HIT_STATE);
		m_GameData[m_GameCount % SAVING_DATA_NUM].push_back(pos);
	}

	if (res >= DESTROY)
	{
		m_DestroyData[pos] = res;
		m_NumOfEnemyShips[ClientShipData::GetType(res)]--;
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
	FILE* file = fopen(DATA_FILE_NAME, "r");
	char buffer[BUF_SIZE];
	
	if (file == nullptr)
	{
		return;
	}

	while (fgets(buffer, BUF_SIZE, file) != NULL)
	{
		m_GameCount++;

		int total_turn = 0;

		for (int idx = 0; idx < ClientShipData::TYPE_NUM; idx++)
		{
			total_turn += ClientShipData::GetSize((ClientShipType)idx);
		}
		for (int i = 0; i < total_turn; i++)
		{
			m_GameData[m_GameCount].push_back(Point(buffer[i * 3], buffer[i * 3 + 1]));
		}
	}

	fclose(file);
}

ShipData& Player::GetShipPosList()
{
	return m_ShipData;
}

void Player::GetMapData(char* mapData_) const
{
	_ASSERT(mapData_ != nullptr);

	if (mapData_ == nullptr)
	{
		return;
	}

	for (int i = 0; i < Board::WIDTH*Board::HEIGHT; i++)
	{
		mapData_[i] = mapdata[i];
	}
}