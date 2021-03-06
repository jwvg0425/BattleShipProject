#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
	m_Pos = nullptr;
}


Ship::~Ship()
{
	SAFE_DELETE_ARR(m_Pos);
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
	_ASSERT(pos != Point::GetNullPoint());
	
	if (pos == Point::GetNullPoint())
	{
		return MISS;
	}

	int idx = HasPosition(pos);

	if ( idx != -1)
	{
		m_HP--;
		m_Pos[idx] = Point::GetNullPoint();

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
	_ASSERT(pos != Point::GetNullPoint());

	for (int i = 0; i < m_MaxHP; i++)
	{
		if (m_Pos[i].x == 0)
		{
			m_Pos[i] = pos;
			return;
		}
	}
}

int Ship::GetMaxHP() const
{
	return m_MaxHP;
}

void Ship::PrintData()
{
	printf("name : %s, MaxHP : %d, Pos :", m_Name.c_str(), m_MaxHP);

	for (int i = 0; i < m_MaxHP; i++)
	{
		printf("%c%c ", m_Pos[i].x,m_Pos[i].y);
	}
	printf("\n");
}

int Ship::GetHP() const
{
	return m_HP;
}

ClientShipType Ship::GetType() const
{
	return m_Type;
}

