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
}

void Player::Init()
{
	m_PrevAttackPoint.x = 0;
	m_PrevAttackPoint.y = 0;
	m_AIState = AIState::SEARCH;
	m_HitCount = 0;

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
	InitFindPoints();
}

void Player::PlaceShips()
{
	Point randomPos;
	Direction dir;
	int startDx = 0, startDy = 0, dx = 0, dy = 0;

	for (auto& ship : m_ShipList)
	{
		do
		{
			startDx = 0;
			startDy = 0;

			dir = (Direction)RANDOM(2);

			if (dir == Direction::DOWN)
			{
				startDy = -ship->GetMaxHP();
				dy = 1;
				dx = 0;
			}
			else
			{
				startDx = -ship->GetMaxHP();
				dx = 1;
				dy = 0;
			}

			randomPos.x = 'a' + (char)RANDOM(BOARD_WIDTH + startDx);
			randomPos.y = '1' + (char)RANDOM(BOARD_HEIGHT + startDy);
		} while (!IsValidPlace(randomPos, dir, ship->GetMaxHP()));

		for (int i = 0; i < ship->GetMaxHP(); i++)
		{
			m_MyBoard->SetBoardState(randomPos, SHIP_STATE);
			ship->AddPosition(randomPos);
			randomPos.x += (char)dx;
			randomPos.y += (char)dy;
		}
	}
}

bool Player::IsValidPlace(Point pos, Direction dir, int length)
{
	int dx = 0, dy = 0;

	if (dir == Direction::DOWN)
	{
		dx = 0;
		dy = 1;
	}
	else
	{
		dx = 1;
		dy = 0;
	}


	for (int i = 0; i < length; i++)
	{
		if (m_MyBoard->GetBoardState(pos) != BoardState::NONE_STATE ||
			m_MyBoard->GetAloneGrade(pos)<4)
		{
			return false;
		}

		pos.x += (char)dx;
		pos.y += (char)dy;
	}

	return true;
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

	return HitResult::MISS;
}

void Player::RecieveAttackResult(Point pos, HitResult res)
{
	if (res == MISS)
	{
		m_EnemyBoard->SetBoardState(pos, BoardState::MISS_STATE);
	}
	else
	{
		m_EnemyBoard->SetBoardState(pos, BoardState::HIT_STATE);
	}

	if (res > DESTROY)
	{
		m_NumOfEnemyShips[ShipData::GetShipType(res)]--;
		m_DestroyData[pos] = res;
	}
	
	m_PrevAttackPoint = pos;

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

int Player::GetHitSize(Point pos, Direction dir)
{
	int size = 0;

	while (m_EnemyBoard->GetBoardState(pos) == HIT_STATE)
	{
		size++;
		pos.ChangeByDir(dir);
	}

	return size;
}

int Player::GetMaxHitSize(Point pos, Direction dir)
{
	int size;
	Point head = pos, tail = pos;

	while (m_EnemyBoard->GetBoardState(head) == HIT_STATE)
	{
		head.ChangeByDir(dir);
	}
	while (m_EnemyBoard->GetBoardState(tail) == HIT_STATE)
	{
		tail.ChangeByDir((Direction)((dir + 2) % 4));
	}

	size = abs(head.x - tail.x + head.y - tail.y - 1);

	return size;
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

int Player::GetLengthToNotNoneCell(Point pos, Direction dir)
{
	int size = 0;

	pos.ChangeByDir(dir);
	while (m_EnemyBoard->GetBoardState(pos) == NONE_STATE)
	{
		size++;
		pos.ChangeByDir(dir);
	}
	return size;
}

bool Player::IsValidAttackStartPos(Point pos)
{
	int size = 0;

	for (int i = 0; i < 4; i++)
	{
		if (m_NumOfEnemyShips[i]>0)
		{
			size = ShipData::GetShipSize((ShipType)i);
			break;
		}
	}

	if (GetPossibleAttackRange(pos)>=size)
	{
		return true;
	}

	return false;
}

int Player::GetPossibleAttackRange(Point pos)
{
	int maxSize = 0;

	for (Direction dir = DOWN; dir <= RIGHT; dir = (Direction)(dir + 1))
	{
		Point rangePos = pos;
		int size = 0;

		while (m_EnemyBoard->GetBoardState(rangePos) == NONE_STATE ||
			m_EnemyBoard->GetBoardState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos.ChangeByDir(dir);
		}

		rangePos = pos;
		while (m_EnemyBoard->GetBoardState(rangePos) == NONE_STATE ||
			m_EnemyBoard->GetBoardState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos.ChangeByDir((Direction)((dir + 2) % 4));
		}
		if (size > maxSize)maxSize = size;
	}

	return maxSize - 1;
}

