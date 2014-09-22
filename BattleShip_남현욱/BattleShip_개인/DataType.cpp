#include"stdafx.h"
#include"DataType.h"

int ClientShipData::GetSize(ClientShipType type)
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

int ClientShipData::GetSize(HitResult result)
{
	return GetSize(GetType(result));
}

int ClientShipData::GetNum(ClientShipType type)
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

ClientShipType ClientShipData::GetType(HitResult result)
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

	return (ClientShipType)-1;
}


Point Point::ChangeByDir(ClientDirection dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir < ClientDirection::END);
	int dx, dy;
	Point newPoint;

	dir.GetDeltaValue(dx, dy);

	newPoint.x += x + (char)dx;
	newPoint.y += y + (char)dy;

	return newPoint;
}

ClientDirection& ClientDirection::operator ++(int num)
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

ClientDirection::Type ClientDirection::operator ~()
{
	return GetReverseDir();
}

ClientDirection::Type ClientDirection::GetReverseDir()
{
	return (Type)((value + 2) % 4);
}

bool ClientDirection::operator<(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	return value < type;
}

bool ClientDirection::operator<(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	return value < dir.value;
}

bool ClientDirection::operator>(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	return value < type;
}

bool ClientDirection::operator>(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	return value > dir.value;
}

bool ClientDirection::operator==(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	return value == type;
}

bool ClientDirection::operator==(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	return value == dir.value;
}

bool ClientDirection::operator>=(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	return value >= dir.value;
}

bool ClientDirection::operator>=(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	return value >= type;
}

bool ClientDirection::operator<=(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	return value <= type;
}

bool ClientDirection::operator<=(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	return value <= dir.value;
}

ClientDirection& ClientDirection::operator=(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	value = type;

	return *this;
}

ClientDirection& ClientDirection::operator=(ClientDirection& dir)
{
	_ASSERT(dir >= ClientDirection::BEGIN && dir <= ClientDirection::END);

	value = dir.value;

	return *this;
}

ClientDirection::ClientDirection(Type type)
{
	_ASSERT(type >= ClientDirection::BEGIN && type <= ClientDirection::END);

	value = type;
}

ClientDirection::ClientDirection()
{
	value = ERROR_VALUE;
}

ClientDirection::~ClientDirection()
{

}

ClientDirection::Type ClientDirection::operator+ (int num)
{
	return (Type)((value + num) % 4);
}

void ClientDirection::GetDeltaValue(int& dx, int& dy)
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
