#pragma once
#include "Ship.h"
class AirCraft :
	public Ship
{
public:
	AirCraft();
	virtual ~AirCraft();
	virtual HitResult HitCheck(Point pos);
};

