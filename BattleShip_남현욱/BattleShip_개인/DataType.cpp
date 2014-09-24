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
