#include"stdafx.h"
#include"DataType.h"

int ShipData::GetSize(ShipType type)
{
	switch (type)
	{
	case AIRCRAFT:
		return 5;
	case BATTLESHIP:
		return 4;
	case CRUISER:
		return 3;
	case DESTROYER:
		return 2;
	}

	return -1;
}

int ShipData::GetSize(HitResult result)
{
	return GetSize(GetType(result));
}

int ShipData::GetNum(ShipType type)
{
	switch (type)
	{
	case AIRCRAFT:
		return 1;
	case BATTLESHIP:
		return 1;
	case CRUISER:
		return 1;
	case DESTROYER:
		return 2;
	}

	return -1;
}

ShipType ShipData::GetType(HitResult result)
{
	switch (result)
	{
	case DESTROY_AIRCRAFT:
		return AIRCRAFT;
	case DESTROY_BATTLESHIP:
		return BATTLESHIP;
	case DESTROY_CRUISER:
		return CRUISER;
	case DESTROY_DESTROYER:
		return DESTROYER;
	}

	return (ShipType)-1;
}


void Point::ChangeByDir(Direction dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir < Direction::END);
	int dx, dy;

	dir.GetDeltaValue(dx, dy);

	x += (char)dx;
	y += (char)dy;
}

Direction& Direction::operator ++(int num)
{
	if (value == END)
	{
		value = BEGIN;
	}
	else
	{
		value = (Type)(value + 1);
	}

	return *this;
}

Direction::Type Direction::operator ~()
{
	return GetReverseDir();
}

Direction::Type Direction::GetReverseDir()
{
	return (Type)((value + 2) % 4);
}

bool Direction::operator<(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	return value < type;
}

bool Direction::operator<(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	return value < dir.value;
}

bool Direction::operator>(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	return value < type;
}

bool Direction::operator>(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	return value > dir.value;
}

bool Direction::operator==(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	return value == type;
}

bool Direction::operator==(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	return value == dir.value;
}

bool Direction::operator>=(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	return value >= dir.value;
}

bool Direction::operator>=(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	return value >= type;
}

bool Direction::operator<=(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	return value <= type;
}

bool Direction::operator<=(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	return value <= dir.value;
}

Direction& Direction::operator=(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	value = type;

	return *this;
}

Direction& Direction::operator=(Direction& dir)
{
	_ASSERT(dir >= Direction::BEGIN && dir <= Direction::END);

	value = dir.value;

	return *this;
}

Direction::Direction(Type type)
{
	_ASSERT(type >= Direction::BEGIN && type <= Direction::END);

	value = type;
}

Direction::Direction()
{
	value = ERROR_VALUE;
}

Direction::~Direction()
{

}

Direction::Type Direction::operator+ (int num)
{
	return (Type)((value + num) % 4);
}

void Direction::GetDeltaValue(int& dx, int& dy)
{
	switch (value)
	{
	case UP:
		dx = 0;
		dy = -1;
		break;
	case LEFT:
		dx = -1;
		dy = 0;
		break;
	case DOWN:
		dx = 0;
		dy = 1;
		break;
	case RIGHT:
		dx = 1;
		dy = 0;
		break;
	}
}
