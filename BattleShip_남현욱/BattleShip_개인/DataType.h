#pragma once
#include"Enums.h"

struct Point
{
	char x, y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(char x_, char y_)
	{
		x = x_; y = y_;
	}

	bool operator == (const Point& pos) const
	{
		return (pos.x == this->x && pos.y == this->y);
	}

	bool operator <(const Point& pos) const
	{
		return (this->x + this->y) < (pos.x + pos.y);
	}
	void ChangeByDir(Direction dir);
};

struct ShipData
{
	static int GetShipSize(ShipType type);
	static int GetShipNum(ShipType type);
	static ShipType GetShipType(HitResult result);
	static int GetShipSize(HitResult result);
};
