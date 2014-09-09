#include "stdafx.h"
#include "Player.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Macro.h"
#include "Board.h"

Point Player::GetNextAttackPoint()
{
	int idx;
	Point attackPos;

	if (m_AIState == AIState::SEARCH)
	{
		UpdateFindPoints();

		do
		{	
			attackPos.x = 'a' + RANDOM(BOARD_WIDTH);
			attackPos.y = '1' + RANDOM(BOARD_HEIGHT);

			for (int i = 0; i < 4; i++)
			{
				if (!m_SecondFindPoints[i].empty())
				{
					idx = RANDOM(m_SecondFindPoints[i].size());
					attackPos = m_SecondFindPoints[i][idx];
					break;
				}

			}

			for (int i = 0; i < 4; i++)
			{
				if (!m_FindPoints[i].empty())
				{
					idx = RANDOM(m_FindPoints[i].size());
					attackPos = m_FindPoints[i][idx];
					break;
				}
				
			}
		} while (!IsValidAttackPos(attackPos));
	}
	else if (m_AIState == AIState::HUNT)
	{
		attackPos = m_PrevAttackPoint;
		attackPos.ChangeByDir(m_AttackDir);
	}

	return attackPos;

}

void Player::ChangeAIState(HitResult prevRes)
{
	UpdateCellData();
	
	if (prevRes > HitResult::DESTROY)
	{	
		Point hitPos = GetHitCell(true);
		if (hitPos.x != 0)
		{
			m_AIState = HUNT;
			prevRes = HIT;
			m_HitCount = 0;

			if (m_EnemyBoard->GetBoardState(m_AttackStartPoint) != DESTROY_STATE)
			{
				m_PrevAttackPoint = m_AttackStartPoint;
			}
			else
			{
				do
				{
					if (hitPos.x == 0)
					{
						prevRes = MISS;
						m_AIState = SEARCH;
						break;
					}
					m_PrevAttackPoint = hitPos;
					m_AttackStartPoint = hitPos;
					hitPos = GetHitCell(false);
				} while (!ChangeAttackDir());	
			}
			
			ChangeAttackDir();
			return;
		}
	}

	switch (m_AIState)
	{
	case AIState::SEARCH:
		if (prevRes == HitResult::HIT)
		{
			m_AttackStartPoint = m_PrevAttackPoint;
			m_HitCount = 0;

			if (ChangeAttackDir())
			{
				m_AIState = AIState::HUNT;
			}
		}
		break;
	case AIState::HUNT:
		if ((int)prevRes > (int)HitResult::DESTROY)
		{
			m_AIState = AIState::SEARCH;
		}
		else if (prevRes == HitResult::MISS)
		{
			Point nextAttackPoint;
			Direction nextAttackDir = m_AttackDir;

			nextAttackPoint = m_AttackStartPoint;
			m_PrevAttackPoint = m_AttackStartPoint;

			if (!ChangeAttackDir())
			{
				m_AIState = AIState::SEARCH;
			}
			else if (m_HitCount > 0)
			{
				nextAttackDir = (Direction)((nextAttackDir + 2) % 4);
				nextAttackPoint.ChangeByDir(nextAttackDir);
				if (IsValidAttackPos(nextAttackPoint))
				{
					m_AttackDir = nextAttackDir;
				}
			}
		}
		else if (prevRes == HitResult::HIT)
		{
			Point nextAttackPoint = m_PrevAttackPoint;
			Direction nextAttackDir = m_AttackDir;
			nextAttackPoint.ChangeByDir(nextAttackDir);
			m_HitCount++;

			if (!IsValidAttackPos(nextAttackPoint))
			{
				m_PrevAttackPoint = m_AttackStartPoint;
				nextAttackPoint = m_AttackStartPoint;
				nextAttackDir = (Direction)((m_AttackDir + 2) % 4);

				nextAttackPoint.ChangeByDir(nextAttackDir);
				if (IsValidAttackPos(nextAttackPoint))
				{
					m_AttackDir = nextAttackDir;
				}
				else if (!ChangeAttackDir())
				{
					m_AIState = AIState::SEARCH;
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

	for (Direction dir = DOWN; dir <= LEFT; dir = (Direction)(dir + 1))
	{
		length = GetLengthToNotNoneCell(m_AttackStartPoint, dir);
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
bool Player::IsValidAttackPos(Point pos)
{
	if (m_EnemyBoard->GetBoardState(pos) == BoardState::NONE_STATE)
	{
		return true;
	}

	return false;
}

void Player::UpdateFindPoints()
{
	std::vector<Point>::iterator iter;

	for (int i = 0; i < 4; i++)
	{
		if (m_FindPoints[i].empty())
			continue;

		for (iter = m_FindPoints[i].begin(); iter != m_FindPoints[i].end();)
		{
			Point pos = *iter;
			if (m_EnemyBoard->GetBoardState(pos) != NONE_STATE)
			{
				iter = m_FindPoints[i].erase(iter);
			}
			else if (m_EnemyBoard->GetAloneGrade(pos) < 4)
			{
				iter = m_FindPoints[i].erase(iter);
				m_SecondFindPoints[i].push_back(pos);
			}
			else
			{
				++iter;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_SecondFindPoints[i].empty())
			continue;

		for (iter = m_SecondFindPoints[i].begin(); iter != m_SecondFindPoints[i].end();)
		{
			Point pos = *iter;
			if (m_EnemyBoard->GetBoardState(pos) != NONE_STATE)
			{
				iter = m_SecondFindPoints[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void Player::InitFindPoints()
{
	for (int i = 0; i < 4; i++)
	{
		m_FindPoints[i].clear();
	}

	//type1
	m_FindPoints[0].push_back(Point('b', '2'));
	m_FindPoints[0].push_back(Point('b', '4'));
	m_FindPoints[0].push_back(Point('b', '6'));
	m_FindPoints[0].push_back(Point('c', '5'));
	m_FindPoints[0].push_back(Point('c', '7'));
	m_FindPoints[0].push_back(Point('d', '2'));
	m_FindPoints[0].push_back(Point('d', '4'));
	m_FindPoints[0].push_back(Point('e', '3'));
	m_FindPoints[0].push_back(Point('e', '5'));
	m_FindPoints[0].push_back(Point('e', '7'));
	m_FindPoints[0].push_back(Point('f', '2'));
	m_FindPoints[0].push_back(Point('f', '6'));
	m_FindPoints[0].push_back(Point('g', '3'));
	m_FindPoints[0].push_back(Point('g', '5'));
	m_FindPoints[0].push_back(Point('g', '7'));


	m_FindPoints[1].push_back(Point('a', '3'));
	m_FindPoints[1].push_back(Point('a', '7'));
	m_FindPoints[1].push_back(Point('c', '1'));
	m_FindPoints[1].push_back(Point('d', '8'));
	m_FindPoints[1].push_back(Point('g', '1'));
	m_FindPoints[1].push_back(Point('h', '4'));

	m_FindPoints[2].push_back(Point('c', '3'));
	m_FindPoints[2].push_back(Point('d', '6'));
	m_FindPoints[2].push_back(Point('f', '4'));

	m_FindPoints[3].push_back(Point('h', '8'));
	m_FindPoints[3].push_back(Point('a', '1'));
	m_FindPoints[3].push_back(Point('a', '5'));
	m_FindPoints[3].push_back(Point('b', '8'));
	m_FindPoints[3].push_back(Point('e', '1'));
	m_FindPoints[3].push_back(Point('f', '8'));
	m_FindPoints[3].push_back(Point('h', '2'));
	m_FindPoints[3].push_back(Point('h', '6'));
	

	//type2
	/*
	m_FindPoints[0].push_back(Point('b', '2'));
	m_FindPoints[0].push_back(Point('b', '4'));
	m_FindPoints[0].push_back(Point('b', '6'));
	m_FindPoints[0].push_back(Point('c', '5'));
	m_FindPoints[0].push_back(Point('d', '2'));
	m_FindPoints[0].push_back(Point('d', '4'));
	m_FindPoints[0].push_back(Point('d', '6'));
	m_FindPoints[0].push_back(Point('e', '3'));
	m_FindPoints[0].push_back(Point('e', '7'));
	m_FindPoints[0].push_back(Point('f', '2'));
	m_FindPoints[0].push_back(Point('f', '4'));
	m_FindPoints[0].push_back(Point('f', '6'));
	m_FindPoints[0].push_back(Point('g', '5'));
	m_FindPoints[0].push_back(Point('g', '7'));

	m_FindPoints[1].push_back(Point('a', '3'));
	m_FindPoints[1].push_back(Point('a', '7'));
	m_FindPoints[1].push_back(Point('b', '8'));
	m_FindPoints[1].push_back(Point('c', '1'));
	m_FindPoints[1].push_back(Point('d', '8'));
	m_FindPoints[1].push_back(Point('g', '1'));
	m_FindPoints[1].push_back(Point('h', '2'));
	m_FindPoints[1].push_back(Point('h', '4'));
	
	m_FindPoints[2].push_back(Point('c', '3'));
	m_FindPoints[2].push_back(Point('c', '7'));
	m_FindPoints[2].push_back(Point('e', '5'));
	m_FindPoints[2].push_back(Point('g', '3'));

	m_FindPoints[3].push_back(Point('h', '8'));
	m_FindPoints[3].push_back(Point('a', '1'));
	m_FindPoints[3].push_back(Point('a', '5'));
	m_FindPoints[3].push_back(Point('e', '1'));
	m_FindPoints[3].push_back(Point('f', '8'));
	m_FindPoints[3].push_back(Point('h', '6'));
	*/
}

void Player::UpdateCellData()
{
	while (!m_DestroyData.empty())
	{
		std::list<Point> completePoints;
		for (auto& destroyData : m_DestroyData)
		{
			Point pos = destroyData.first;
			HitResult res = destroyData.second;
			int size = ShipData::GetShipSize(res);

			if (GetMaxHitSize(pos, DOWN) == size&&GetMaxHitSize(pos, RIGHT) < size)
			{
				while (m_EnemyBoard->GetBoardState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(UP);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(DOWN);
					m_EnemyBoard->SetBoardState(pos, DESTROY_STATE);
				}
				completePoints.push_back(destroyData.first);
			}
			else if (GetMaxHitSize(pos, DOWN) < size &&GetMaxHitSize(pos, RIGHT) == size)
			{
				while (m_EnemyBoard->GetBoardState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(LEFT);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(RIGHT);
					m_EnemyBoard->SetBoardState(pos, DESTROY_STATE);

				}

				completePoints.push_back(destroyData.first);
			}
			else
			{
				for (Direction dir = DOWN; dir <= LEFT; dir = (Direction)(dir + 1))
				{
					if (GetHitSize(pos, dir) >= size&&
						GetHitSize(pos, (Direction)((dir + 1) % 4)) < size &&
						GetHitSize(pos, (Direction)((dir + 2) % 4)) < size &&
						GetHitSize(pos, (Direction)((dir + 3) % 4)) < size)
					{
						//destroy cell로 변경
						for (int s = 0; s < size; s++)
						{
							m_EnemyBoard->SetBoardState(pos, DESTROY_STATE);
							pos.ChangeByDir(dir);
						}
						completePoints.push_back(destroyData.first);
						break;
					}
				}
			}
		}

		for (auto point : completePoints)
		{
			m_DestroyData.erase(point);
		}
		if (completePoints.empty())
		{
			completePoints.clear();
			break;
		}
		completePoints.clear();
	}

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			Point pos = Point('a' + (char)x, '1' + (char)y);
			if (m_EnemyBoard->GetBoardState(pos) != NONE_STATE)
				continue;

			if (!IsValidAttackStartPos(pos))
			{
				m_EnemyBoard->SetBoardState(pos, MISS_STATE);
			}
		}
	}
}

Point Player::GetHitCell(bool isFirst)
{
	static Point prevHitCell = Point('a','1');

	if (isFirst)
	{
		prevHitCell = Point('a', '1');
	}

	for (int x = prevHitCell.x-'a'; x < BOARD_WIDTH; x++)
	{
		for (int y = prevHitCell.y-'1'; y < BOARD_HEIGHT; y++)
		{
			Point checkPos = Point('a' + (char)x, '1' + (char)y);

			if (checkPos == prevHitCell)
			{
				continue;
			}

			if (m_EnemyBoard->GetBoardState(checkPos) == BoardState::HIT_STATE)
			{
				prevHitCell = checkPos;
				return checkPos;
			}
		}
	}

	return Point(0, 0);
}