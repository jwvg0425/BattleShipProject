#pragma once
#include "Ship.h"
class Cruiser :
	public Ship
{
public:
	Cruiser();
	virtual ~Cruiser();
	virtual HitResult HitCheck(Point pos);
};

