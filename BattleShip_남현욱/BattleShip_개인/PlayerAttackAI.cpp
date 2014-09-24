#include "stdafx.h"
#include "Player.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Macro.h"
#include "Board.h"

Point Player::GetNextAttackPos()
{
	Point attackPos;

	m_AttackCount++;

	if (m_AIState == AIState::HUNT)
	{
		UpdateMonteCarloBoard();
		UpdateGameDataBoard();

		attackPos = GetPosByExperience();

	}
	else if (m_AIState == AIState::TARGET)
	{
		attackPos = m_PrevAttackPos;
		attackPos = attackPos.ChangeByDir(m_AttackDir);
	}

	return attackPos;

}

void Player::UpdateAIState(HitResult prevRes)
{
	m_EnemyBoard->UpdateCellData(m_DestroyData,m_NumOfEnemyShips);
	
	//파괴됐을 경우에 다음 좌표 선택 처리
	if (prevRes >= DESTROY)
	{
		//공격 시작했던 위치가 destroy되지 않았다면 여기서 다시 시작한다
		if (m_EnemyBoard->GetCellState(m_AttackStartPos) != DESTROY_STATE)
		{
			if (ChangeAttackDir())
			{
				m_PrevAttackPos = m_AttackStartPos;
				m_AIState = TARGET;
				prevRes = HIT;
				m_HitCount = 0;
				return;
			}
		}

		//다른 hit인 위치가 있으면 거기서부터 다시 target 과정을 수행한다. 없으면 hunt.
		Point hitPos = m_EnemyBoard->GetHitCell(true);
		if (hitPos != Point::GetNullPoint())
		{
			m_AIState = TARGET;
			prevRes = HIT;
			m_HitCount = 0;

			do
			{
				if (hitPos != Point::GetNullPoint())
				{
					prevRes = MISS;
					m_AIState = HUNT;
					break;
				}
				m_PrevAttackPos = hitPos;
				m_AttackStartPos = hitPos;
				hitPos = m_EnemyBoard->GetHitCell(false);
			} while (!ChangeAttackDir());

			return;
		}

	}

	switch (m_AIState)
	{
	case HUNT:
		if (prevRes == HIT)
		{
			m_AttackStartPos = m_PrevAttackPos;
			m_HitCount = 0;

			m_AIState = TARGET;
			ChangeAttackDir();
		}
		break;
	case TARGET:
		if ((int)prevRes >= (int)DESTROY)
		{
			m_AIState = HUNT;
		}
		else if (prevRes == MISS)
		{
			Point nextAttackPoint;
			ClientDirection nextAttackDir = m_AttackDir;

			nextAttackPoint = m_AttackStartPos;
			m_PrevAttackPos = m_AttackStartPos;

			//시작점 기준으로 더이상 공격할 곳이 없다면 HUNT로 돌아간다.
			if (!ChangeAttackDir())
			{
				m_AIState = HUNT;
			}
			else
			{
				//공격하던 방향의 반대 방향을 우선적으로 공격한다.
				nextAttackDir = nextAttackDir.GetReverseDir();
				nextAttackPoint = nextAttackPoint.ChangeByDir(nextAttackDir);
				if (m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
				{
					m_AttackDir = nextAttackDir;
				}
			}
		}
		//공격 성공시 다음 공격 위치가 공격 가능하면 공격하고, 아니라면 다른 공격 위치로 바꾼다.
		else if (prevRes == HIT)
		{
			Point nextAttackPoint = m_PrevAttackPos;
			ClientDirection nextAttackDir = m_AttackDir;
			nextAttackPoint = nextAttackPoint.ChangeByDir(nextAttackDir);
			m_HitCount++;

			//다음 위치가 공격 불가능한 경우
			if (!m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
			{
				//MISS인 경우와 마찬가지로 공격 시작점에서 반대방향 우선으로 다른 공격 방향을 탐색한다.
				m_PrevAttackPos = m_AttackStartPos;
				nextAttackPoint = m_AttackStartPos;
				nextAttackDir = m_AttackDir.GetReverseDir();

				nextAttackPoint = nextAttackPoint.ChangeByDir(nextAttackDir);
				if (m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
				{
					m_AttackDir = nextAttackDir;
				}
				else if (!ChangeAttackDir())
				{
					m_AIState = HUNT;
				}
			}
		}
		break;
	}
}

bool Player::ChangeAttackDir()
{
	int maxLength = 0;
	int length;

	for (ClientDirection dir = ClientDirection::BEGIN; dir < ClientDirection::END; dir++)
	{
		length = m_EnemyBoard->GetNoneSize(m_AttackStartPos, dir);

		if (length > maxLength)
		{
			maxLength = length;
			m_AttackDir = dir;
		}
	}

	if (maxLength == 0)
	{
		return false;
	}
	return true;
}

void Player::UpdateMonteCarloBoard()
{
	Board tempBoard;
	Point randomPos;
	ClientDirection dir = ClientDirection::DOWN;
	int dx = 0, dy = 0;
	memset(m_MonteCarloBoard, 0, sizeof(m_MonteCarloBoard));

	for (int i = 0; i < SIMULATE_NUM; i++)
	{
		int count = 0;

		for (int x = 0; x < Board::WIDTH; x++)
		{
			for (int y = 0; y < Board::HEIGHT; y++)
			{
				BoardState state = m_EnemyBoard->GetCellState((char)x + Board::START_X, (char)y + Board::START_Y);
				
				tempBoard.SetCellState((char)x + Board::START_X, (char)y + Board::START_Y, state);
			}
		}

		for (int shipType = 0; shipType < ClientShipData::TYPE_NUM; shipType++)
		{
			for (int shipNum= 0; shipNum < m_NumOfEnemyShips[shipType]; shipNum++)
			{
				int size = ClientShipData::GetSize((ClientShipType)shipType);
				count = 0;

				do
				{
					if (count>CHANCE_NUM)
						break;

					dir = (ClientDirection::Type)RANDOM(2);
					GetRandomPlace(&randomPos, &dir);
					count++;
				} while (!tempBoard.IsValidPlace(randomPos, dir, size));

				if (count > CHANCE_NUM)
					break;

				dir.GetDeltaValue(dx, dy);
				

				for (int i = 0; i < size; i++)
				{
					tempBoard.SetCellState(randomPos, SHIP_STATE);

					m_MonteCarloBoard[randomPos.x - Board::START_X][randomPos.y - Board::START_Y]++;
					randomPos.x += (char)dx;
					randomPos.y += (char)dy;
				}
			}

			if (count>CHANCE_NUM)
				break;
		}
		if (count > CHANCE_NUM)
		{
			i--;
		}
	}
}

void Player::UpdateGameDataBoard()
{
	bool isPossible;

	memset(&m_GameDataBoard, 0, sizeof(m_GameDataBoard));
	m_GameDataCount = 0;

	for (int i = 0; i<((m_GameCount>SAVING_DATA_NUM) ? SAVING_DATA_NUM : m_GameCount); i++)
	{
		isPossible = true;

		for (int k = 0; k < (int)m_GameData[i].size(); k++)
		{
			if (m_EnemyBoard->GetCellState(m_GameData[i][k]) == MISS_STATE)
			{
				isPossible = false;
				break;
			}
		}
		if (isPossible)
		{
			m_GameDataCount++;
			for (int k = 0; k < (int)m_GameData[i].size(); k++)
			{
				m_GameDataBoard[m_GameData[i][k].x - Board::START_X][m_GameData[i][k].y - Board::START_Y]++;
			}
		}
	}
}

Point Player::GetPosByExperience()
{
	int maxValue = 0;
	Point returnPos;

	for (int y = 0; y < Board::HEIGHT; y++)
	{
		for (int x = 0; x < Board::WIDTH; x++)
		{

			int factor1 = m_MonteCarloBoard[x][y];
			int factor2 = m_GameDataBoard[x][y];

			//게임 데이터가 하나도 없는 경우 모든 value가 0이 되는 것을 방지
			if (factor2 == 0)
				factor2 = 1;

			int value = factor1 * factor2;

			if (value>maxValue &&
				m_EnemyBoard->IsValidAttackPos((char)x + Board::START_X, (char)y + Board::START_Y))
			{
				maxValue = value;
				returnPos = Point((char)x + Board::START_X, (char)y + Board::START_Y);
			}
		}
	}
	return returnPos;
}

