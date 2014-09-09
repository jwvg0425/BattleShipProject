#include "stdafx.h"
#include "AirCraft.h"


AirCraft::AirCraft()
{
	m_Name = "AirCraft";
	m_Type = AIRCRAFT;
	m_MaxHP = 5;
}


AirCraft::~AirCraft()
{
}

HitResult AirCraft::HitCheck(Point pos)
{
	HitResult res = Ship::HitCheck(pos);

	if (res == DESTROY)
	{
		return HitResult::DESTROY_AIRCRAFT;
	}

	return res;
}
