#include "stdafx.h"
#include "BattleShip.h"


BattleShip::BattleShip()
{
	m_Name = "BattleShip";
	m_Type = BATTLESHIP;
	m_MaxHP = 4;
}


BattleShip::~BattleShip()
{
}

HitResult BattleShip::HitCheck(Point pos)
{
	HitResult res = Ship::HitCheck(pos);

	if (res == DESTROY)
	{
		return HitResult::DESTROY_BATTLESHIP;
	}

	return res;
}
