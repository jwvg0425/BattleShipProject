#pragma once
#include "Ship.h"
class BattleShip :
	public Ship
{
public:
	BattleShip();
	virtual ~BattleShip();
	virtual HitResult HitCheck(Point pos);
};

