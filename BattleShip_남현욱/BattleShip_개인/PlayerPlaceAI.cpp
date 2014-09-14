#include "stdafx.h"
#include "Player.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Macro.h"
#include "Board.h"


void Player::PlaceShips()
{
	Point randomPos;
	Direction dir;
	int dx = 0, dy = 0;

	for (auto& ship : m_ShipList)
	{
		do
		{
			GetPlace(randomPos, dir);
		} while (!m_MyBoard->IsValidPlace(randomPos, dir, ship->GetMaxHP()));

		dir.GetDeltaValue(dx, dy);

		for (int i = 0; i < ship->GetMaxHP(); i++)
		{
			m_MyBoard->SetCellState(randomPos, SHIP_STATE);
			ship->AddPosition(randomPos);
			randomPos.x += (char)dx;
			randomPos.y += (char)dy;
		}
	}
}

void Player::GetRandomPlace(Point& pos, Direction& dir)
{
	dir = (Direction::Type)RANDOM(2);

	pos.x = Board::START_X + (char)RANDOM(Board::WIDTH);
	pos.y = Board::START_Y + (char)RANDOM(Board::HEIGHT);
}

void Player::GetCornerPlace(Point& pos, Direction& dir)
{
	dir = (Direction::Type)RANDOM(2);

	if (!(m_MyBoard->GetCellState(Board::START_X,Board::START_Y) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X, Board::START_Y + Board::HEIGHT - 1) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X + Board::WIDTH - 1, Board::START_Y) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X + Board::WIDTH - 1, Board::START_Y + Board::HEIGHT - 1) == SHIP_STATE))
	{
		int idx;
		idx = RANDOM(4);

		pos.x = (idx % 2) ? 'h' : Board::START_X;
		pos.y = (idx / 2) ? '8' : Board::START_Y;
		if (idx % 2 == 1 && dir == Direction::RIGHT)
		{
			dir = ~dir;
		}
		if (idx / 2 == 1 && dir == Direction::DOWN)
		{
			dir = ~dir;
		}
	}
	else
	{
		pos.x = Board::START_X + (char)RANDOM(Board::WIDTH);
		pos.y = Board::START_Y + (char)RANDOM(Board::HEIGHT);
	}
}

void Player::GetPlace(Point& pos, Direction& dir)
{
	switch (m_PlaceType)
	{
	case RANDOM:
		GetRandomPlace(pos, dir);
		break;
	case CORNER:
		GetCornerPlace(pos, dir);
		break;
	case CORNER_FAVORED:
		GetCornerFavoredPlace(pos, dir);
		break;
	}
}

void Player::GetCornerFavoredPlace(Point& pos, Direction& dir)
{
	dir = (Direction::Type)(RANDOM(2) + 1);

	pos.x = Board::START_X + (char)RANDOM(Board::WIDTH - 4);
	pos.y = Board::START_Y + 4 + (char)RANDOM(Board::HEIGHT - 4);
}

