#include"stdafx.h"
#include"DataType.h"

int ShipData::GetShipSize(ShipType type)
{
	switch (type)
	{
	case AIRCRAFT:
		return 5;
		break;
	case BATTLESHIP:
		return 4;
		break;
	case CRUISER:
		return 3;
		break;
	case DESTROYER:
		return 2;
		break;
	}

	return -1;
}

int ShipData::GetShipSize(HitResult result)
{
	return GetShipSize(GetShipType(result));
}

int ShipData::GetShipNum(ShipType type)
{
	switch (type)
	{
	case AIRCRAFT:
		return 1;
		break;
	case BATTLESHIP:
		return 1;
		break;
	case CRUISER:
		return 1;
		break;
	case DESTROYER:
		return 2;
		break;
	}
	return -1;
}

ShipType ShipData::GetShipType(HitResult result)
{
	switch (result)
	{
	case DESTROY_AIRCRAFT:
		return AIRCRAFT;
		break;
	case DESTROY_BATTLESHIP:
		return BATTLESHIP;
		break;
	case DESTROY_CRUISER:
		return CRUISER;
		break;
	case DESTROY_DESTROYER:
		return DESTROYER;
		break;
	}

	return (ShipType)-1;
}


void Point::ChangeByDir(Direction dir)
{
	switch (dir)
	{
	case Direction::DOWN:
		y += 1;
		break;
	case Direction::UP:
		y -= 1;
		break;
	case Direction::LEFT:
		x -= 1;
		break;
	case Direction::RIGHT:
		x += 1;
		break;
	}
}
