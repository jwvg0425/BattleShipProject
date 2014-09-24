#include "stdafx.h"
#include "Direction.h"


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
