#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
	m_Pos = nullptr;
}


Ship::~Ship()
{
	delete[] m_Pos;
}


void Ship::Init()
{
	if (m_Pos == nullptr)
	{
		m_Pos = new Point[m_MaxHP];
	}
	ZeroMemory(m_Pos, sizeof(Point)*m_MaxHP);
	m_HP = m_MaxHP;
}

int Ship::HasPosition(Point pos)
{
	for (int i = 0; i < m_MaxHP; i++)
	{
		if (pos == m_Pos[i])
		{
			return i;
		}
	}

	return -1;
}

HitResult Ship::HitCheck(Point pos)
{
	_ASSERT(pos.x != 0 && pos.y != 0);
	
	if (pos.x == 0 || pos.y == 0)
	{
		return MISS;
	}

	int idx = HasPosition(pos);

	if ( idx != -1)
	{
		m_HP--;
		m_Pos[idx].x = 0;
		m_Pos[idx].y = 0;

		if (m_HP <= 0)
		{
			return HitResult::DESTROY;
		}

		return HitResult::HIT;
	}

	return HitResult::MISS;
}

void Ship::AddPosition(Point pos)
{
	_ASSERT(pos.x != 0 && pos.y != 0);

	for (int i = 0; i < m_MaxHP; i++)
	{
		if (m_Pos[i].x == 0)
		{
			m_Pos[i] = pos;
			return;
		}
	}
}

int Ship::GetMaxHP()
{
	return m_MaxHP;
}

void Ship::PrintData()
{
	//printf("name : %s, MaxHP : %d, Pos :", m_Name.c_str(), m_MaxHP);

	for (int i = 0; i < m_MaxHP; i++)
	{
		printf("%c%c ", m_Pos[i].x,m_Pos[i].y);
	}
	//printf("\n");
}

int Ship::GetHP()
{
	return m_HP;
}

