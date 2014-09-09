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
	int idx;
	Point attackPos;

	if (m_AIState == AIState::SEARCH)
	{
		UpdateFindPos();
		std::vector<Point> posList;
		GetPriorityPos(posList, 6);

		do
		{	
			attackPos.x = 'a' + RANDOM(BOARD_WIDTH);
			attackPos.y = '1' + RANDOM(BOARD_HEIGHT);

			if (!posList.empty())
			{
				idx = RANDOM(posList.size());
				attackPos = posList[idx];
			}
			
			if (m_AttackCount<16)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!m_SecondFindPos[i].empty())
					{
						idx = RANDOM(m_SecondFindPos[i].size());
						attackPos = m_SecondFindPos[i][idx];
						break;
					}
				}

				for (int i = 0; i < 4; i++)
				{
					if (!m_FindPos[i].empty())
					{
						idx = RANDOM(m_FindPos[i].size());
						attackPos = m_FindPos[i][idx];
						break;
					}
				}
			}

		} while (!m_EnemyBoard->IsValidAttackPos(attackPos));
	}
	else if (m_AIState == AIState::HUNT)
	{
		attackPos = m_PrevAttackPos;
		attackPos.ChangeByDir(m_AttackDir);
	}

	return attackPos;

}

void Player::ChangeAIState(HitResult prevRes)
{
	m_EnemyBoard->UpdateCellData(m_DestroyData,m_NumOfEnemyShips);
	
	if (prevRes > HitResult::DESTROY)
	{	
		Point hitPos = m_EnemyBoard->GetHitCell(true);
		if (hitPos.x != 0)
		{
			m_AIState = HUNT;
			prevRes = HIT;
			m_HitCount = 0;

			if (m_EnemyBoard->GetCellState(m_AttackStartPos) != DESTROY_STATE)
			{
				m_PrevAttackPos = m_AttackStartPos;
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
					m_PrevAttackPos = hitPos;
					m_AttackStartPos = hitPos;
					hitPos = m_EnemyBoard->GetHitCell(false);
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
			m_AttackStartPos = m_PrevAttackPos;
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

			nextAttackPoint = m_AttackStartPos;
			m_PrevAttackPos = m_AttackStartPos;

			if (!ChangeAttackDir())
			{
				m_AIState = AIState::SEARCH;
			}
			else if (m_HitCount > 0)
			{
				nextAttackDir = (Direction)((nextAttackDir + 2) % 4);
				nextAttackPoint.ChangeByDir(nextAttackDir);
				if (m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
				{
					m_AttackDir = nextAttackDir;
				}
			}
		}
		else if (prevRes == HitResult::HIT)
		{
			Point nextAttackPoint = m_PrevAttackPos;
			Direction nextAttackDir = m_AttackDir;
			nextAttackPoint.ChangeByDir(nextAttackDir);
			m_HitCount++;

			if (!m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
			{
				m_PrevAttackPos = m_AttackStartPos;
				nextAttackPoint = m_AttackStartPos;
				nextAttackDir = (Direction)((m_AttackDir + 2) % 4);

				nextAttackPoint.ChangeByDir(nextAttackDir);
				if (m_EnemyBoard->IsValidAttackPos(nextAttackPoint))
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
		length = m_EnemyBoard->GetLengthToNotNoneCell(m_AttackStartPos, dir);
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

void Player::UpdateFindPos()
{
	std::vector<Point>::iterator iter;

	for (int i = 0; i < 4; i++)
	{
		if (m_FindPos[i].empty())
			continue;

		for (iter = m_FindPos[i].begin(); iter != m_FindPos[i].end();)
		{
			Point pos = *iter;
			if (m_EnemyBoard->GetCellState(pos) != NONE_STATE)
			{
				iter = m_FindPos[i].erase(iter);
			}
			else if (m_EnemyBoard->GetAloneGrade(pos) < 4)
			{
				iter = m_FindPos[i].erase(iter);
				m_SecondFindPos[i].push_back(pos);
			}
			else
			{
				++iter;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_SecondFindPos[i].empty())
			continue;

		for (iter = m_SecondFindPos[i].begin(); iter != m_SecondFindPos[i].end();)
		{
			Point pos = *iter;
			if (m_EnemyBoard->GetCellState(pos) != NONE_STATE)
			{
				iter = m_SecondFindPos[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void Player::InitFindPos()
{
	for (int i = 0; i < 4; i++)
	{
		m_FindPos[i].clear();
	}

	//type1
	m_FindPos[0].push_back(Point('b', '2'));
	m_FindPos[0].push_back(Point('b', '4'));
	m_FindPos[0].push_back(Point('b', '6'));
	m_FindPos[0].push_back(Point('c', '5'));
	m_FindPos[0].push_back(Point('c', '7'));
	m_FindPos[0].push_back(Point('d', '2'));
	m_FindPos[0].push_back(Point('d', '4'));
	m_FindPos[0].push_back(Point('e', '3'));
	m_FindPos[0].push_back(Point('e', '5'));
	m_FindPos[0].push_back(Point('e', '7'));
	m_FindPos[0].push_back(Point('f', '2'));
	m_FindPos[0].push_back(Point('f', '6'));
	m_FindPos[0].push_back(Point('g', '3'));
	m_FindPos[0].push_back(Point('g', '5'));
	m_FindPos[0].push_back(Point('g', '7'));


	m_FindPos[1].push_back(Point('a', '3'));
	m_FindPos[1].push_back(Point('a', '7'));
	m_FindPos[1].push_back(Point('c', '1'));
	m_FindPos[1].push_back(Point('d', '8'));
	m_FindPos[1].push_back(Point('g', '1'));
	m_FindPos[1].push_back(Point('h', '4'));

	m_FindPos[2].push_back(Point('c', '3'));
	m_FindPos[2].push_back(Point('d', '6'));
	m_FindPos[2].push_back(Point('f', '4'));

	m_FindPos[3].push_back(Point('h', '8'));
	m_FindPos[3].push_back(Point('a', '1'));
	m_FindPos[3].push_back(Point('a', '5'));
	m_FindPos[3].push_back(Point('b', '8'));
	m_FindPos[3].push_back(Point('e', '1'));
	m_FindPos[3].push_back(Point('f', '8'));
	m_FindPos[3].push_back(Point('h', '2'));
	m_FindPos[3].push_back(Point('h', '6'));
	

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

