#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer()
{
	m_Name = "Destroyer";
	m_Type = DESTROYER;
	m_MaxHP = 2;
}


Destroyer::~Destroyer()
{
}

HitResult Destroyer::HitCheck(Point pos)
{
	HitResult res = Ship::HitCheck(pos);

	if (res == DESTROY)
	{
		return HitResult::DESTROY_DESTROYER;
	}

	return res;
}
