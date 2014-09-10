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
	
	for (int i = 0; i < 1000; i++)
	{
		m_GameData[i].clear();
	}
}

void Player::Init(bool isReset)
{
	m_PrevAttackPos.x = 0;
	m_PrevAttackPos.y = 0;
	m_AIState = AIState::SEARCH;
	m_HitCount = 0;
	m_AttackCount = 0;
	if (isReset)
	{
		m_GameCount = -1;
		for (int i = 0; i < 1000; i++)
		{
			m_GameData[i].clear();
		}
	}
	else
	{
		m_GameCount++;
		if (m_GameCount >= 1000)
		{
			m_GameData[m_GameCount % 1000].clear();
		}
	}

	for (auto& ship : m_ShipList)
	{
		ship->Init();
	}
	m_MyBoard->Init();
	m_EnemyBoard->Init();
	m_NumOfEnemyShips[AIRCRAFT] = ShipData::GetShipNum(AIRCRAFT);
	m_NumOfEnemyShips[BATTLESHIP] = ShipData::GetShipNum(BATTLESHIP);
	m_NumOfEnemyShips[CRUISER] = ShipData::GetShipNum(CRUISER);
	m_NumOfEnemyShips[DESTROYER] = ShipData::GetShipNum(DESTROYER);
	m_DestroyData.clear();
	InitFindPos();
}

void Player::PlaceShips()
{
	Point randomPos;
	Direction dir;
	int dx = 0, dy = 0;
	int idx;
	int count = 0;

	for (auto& ship : m_ShipList)
	{
		do
		{
			dir = (Direction)RANDOM(2);

			if (dir == Direction::DOWN)
			{
				dy = 1;
				dx = 0;
			}
			else
			{
				dx = 1;
				dy = 0;
			}

			randomPos.x = 'a' + (char)RANDOM(BOARD_WIDTH);
			randomPos.y = '1' + (char)RANDOM(BOARD_HEIGHT);
			
			
			idx = RANDOM(4);
			if (count < 4)
			{
				randomPos.x = (idx % 2) ? 'h' : 'a';
				randomPos.y = (idx / 2) ? '8' : '1';
				if (idx % 2 == 1 && dir==RIGHT)
				{
					dir = LEFT;
					dx = -1;
				}
				if (idx / 2 == 1 && dir == DOWN)
				{
					dir = UP;
					dy = -1;
				}
			}
			else
			{
				randomPos.x = 'a' + (char)RANDOM(BOARD_WIDTH);
				randomPos.y = '1' + (char)RANDOM(BOARD_HEIGHT);
			}
		} while (!m_MyBoard->IsValidPlace(randomPos, dir, ship->GetMaxHP()));

		count++;
		for (int i = 0; i < ship->GetMaxHP(); i++)
		{
			m_MyBoard->SetCellState(randomPos, SHIP_STATE);
			ship->AddPosition(randomPos);
			randomPos.x += (char)dx;
			randomPos.y += (char)dy;
		}
	}
}



HitResult Player::SendAttackResult(Point pos)
{
	HitResult res;

	for (auto& ship : m_ShipList)
	{
		res = ship->HitCheck(pos);
		if ( res != HitResult::MISS)
		{
			return res;
		}
	}

	m_AttackCount++;

	return HitResult::MISS;
}

void Player::RecieveAttackResult(Point pos, HitResult res)
{
	if (res == MISS)
	{
		m_EnemyBoard->SetCellState(pos, BoardState::MISS_STATE);
	}
	else
	{
		m_EnemyBoard->SetCellState(pos, BoardState::HIT_STATE);
		m_GameData[m_GameCount % 1000].push_back(pos);
	}

	if (res > DESTROY)
	{
		m_DestroyData[pos] = res;
		m_NumOfEnemyShips[ShipData::GetShipType(res)]--;
	}
	
	m_PrevAttackPos = pos;

	ChangeAIState(res);
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


