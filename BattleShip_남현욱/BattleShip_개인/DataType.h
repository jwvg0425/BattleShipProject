#pragma once
#include "Enums.h"
#include "Direction.h"

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

	bool operator != (const Point& pos) const
	{
		return !(*this == pos);
	}

	bool operator <(const Point& pos) const
	{
		return (this->x + this->y) < (pos.x + pos.y);
	}

	Point ChangeByDir(ClientDirection dir);

	static Point GetNullPoint();
};

//Ship에 괸련된 데이터를 저장하는 구조체다.
struct ClientShipData
{
	static const int TYPE_NUM = 4;
	static int GetSize(ClientShipType type);
	static int GetNum(ClientShipType type);
	static ClientShipType GetType(HitResult result);
	static int GetSize(HitResult result);
};