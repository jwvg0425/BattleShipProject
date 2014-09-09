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
	m_PrevAttackPos.x = 0;
	m_PrevAttackPos.y = 0;
	m_AIState = AIState::SEARCH;
	m_HitCount = 0;
	m_AttackCount = 0;

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
		} while (!m_MyBoard->IsValidPlace(randomPos, dir, ship->GetMaxHP()));

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

void Player::GetPriorityPos(std::vector<Point>& posList, int number)
{
	int priorityBoard[BOARD_WIDTH + 2][BOARD_HEIGHT + 2][5];

	for (auto& cols : priorityBoard)
	{
		for(auto& cell : cols)
		{
			for (auto& data : cell)
			{
				data = 0;
			}
		}
	}

	for (int x = 1; x <= BOARD_WIDTH; x++)
	{
		for (int y = 1; y <= BOARD_HEIGHT; y++)
		{
			if (m_EnemyBoard->GetCellState(Point(x -1 + 'a', y -1 + '1')) != NONE_STATE)
			{
				priorityBoard[x][y][4] = -1;
			}
		}
	}

	int weight,weight2;

	//left->right, right->left
	for (int y = 1; y <= BOARD_HEIGHT; y++)
	{
		weight = 1;
		weight2 = 1;
		for (int x = 1; x <= BOARD_WIDTH; x++)
		{
			if (priorityBoard[x][y][4] == -1)
			{
				weight = 1;
			}
			else
			{
				priorityBoard[x][y][RIGHT] = weight;
				weight++;
			}

			if (priorityBoard[BOARD_WIDTH - x + 1][y][4] == -1)
			{
				weight2 = 1;
			}
			else
			{
				priorityBoard[BOARD_WIDTH - x + 1][y][LEFT] = weight2;
				weight2++;
			}
		}
	}

	//up->down, down->up
	for (int x = 1; x <= BOARD_WIDTH; x++)
	{
		weight = 1;
		weight2 = 1;
		for (int y = 1; y <= BOARD_HEIGHT; y++)
		{
			if (priorityBoard[x][y][4] == -1)
			{
				weight = 1;
			}
			else
			{
				priorityBoard[x][y][DOWN] = weight;
				weight++;
			}

			if (priorityBoard[x][BOARD_HEIGHT - y + 1][4] == -1)
			{
				weight2 = 1;
			}
			else
			{
				priorityBoard[x][BOARD_HEIGHT - y + 1][UP] = weight2;
				weight2++;
			}
		}
	}

	for (int x = 1; x <= BOARD_WIDTH; x++)
	{
		for (int y = 1; y <= BOARD_HEIGHT; y++)
		{
			if (priorityBoard[x][y][4] != -1)
			{
				priorityBoard[x][y][4] =
					(priorityBoard[x][y][UP] + priorityBoard[x][y][DOWN] +
					abs(priorityBoard[x][y][UP] - priorityBoard[x][y][DOWN]))*
					(priorityBoard[x][y][LEFT] + priorityBoard[x][y][RIGHT] +
					abs(priorityBoard[x][y][LEFT] - priorityBoard[x][y][RIGHT]));

				
				if (priorityBoard[x][y][4]>48)
				{
					if (RANDOM(5) != 0)
					{
						priorityBoard[x][y][4] = 53;
					}
					else
					{
						priorityBoard[x][y][4] = 47;
					}
				}
				
			}
		}
	}

	for (int i = 0; i < number; i++)
	{
		Point maxPos = Point('a', '1');
		for (int x = 1; x <= BOARD_WIDTH; x++)
		{
			for (int y = 1 + (x % 2); y <= BOARD_HEIGHT; y += 2)
			{
				if (
					priorityBoard[maxPos.x - 'a' + 1][maxPos.y - '1' + 1][4]
					< priorityBoard[x][y][4])
				{
					maxPos = Point('a' + x - 1, '1' + y - 1);
				}
			}
		}
		if (priorityBoard[maxPos.x - 'a' + 1][maxPos.y - '1' + 1][4] != -1)
		{
			posList.push_back(maxPos);
			priorityBoard[maxPos.x - 'a' + 1][maxPos.y - '1' + 1][4] = -1;
		}
	}
}
