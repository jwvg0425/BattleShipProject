#pragma once
#include "Ship.h"
class Destroyer :
	public Ship
{
public:
	Destroyer();
	virtual ~Destroyer();
	virtual HitResult HitCheck(Point pos);
};

