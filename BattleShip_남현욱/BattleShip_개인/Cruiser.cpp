#include "stdafx.h"
#include "Cruiser.h"


Cruiser::Cruiser()
{
	m_Name = "Cruiser";
	m_Type = CRUISER;
	m_MaxHP = 3;
}


Cruiser::~Cruiser()
{
}

HitResult Cruiser::HitCheck(Point pos)
{
	HitResult res = Ship::HitCheck(pos);

	if (res == DESTROY)
	{
		return HitResult::DESTROY_CRUISER;
	}

	return res;
}
