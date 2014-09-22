#include "stdafx.h"
#include "Player.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Macro.h"
#include "Board.h"
#include "ShipData.h"


void Player::PlaceShips()
{
	Point randomPos;
	ClientDirection dir;
	int dx = 0, dy = 0;
	bool isFirst = true;

	memset(&mapdata, 0, sizeof(mapdata));

	for (auto& ship : m_ShipList)
	{
		do
		{
			GetPlace(&randomPos, &dir);
		} while (!m_MyBoard->IsValidPlace(randomPos, dir, ship->GetMaxHP()));

		dir.GetDeltaValue(dx, dy);

		for (int i = 0; i < ship->GetMaxHP(); i++)
		{
			m_MyBoard->SetCellState(randomPos, SHIP_STATE);
			ship->AddPosition(randomPos);

			//네트워크 전송용 맵데이터 설정
			mapdata[(randomPos.x-'a') + (randomPos.y-'1') * 8] = (char)(ship->GetType() + 1 + !isFirst);

			randomPos.x += (char)dx;
			randomPos.y += (char)dy;
		}
		if (ship->GetType() == DESTROYER && isFirst)
		{
			isFirst = false;
		}
	}
}

void Player::GetRandomPlace(Point* pos, ClientDirection* dir)
{
	*dir = (ClientDirection::Type)RANDOM(2);

	pos->x = Board::START_X + (char)RANDOM(Board::WIDTH);
	pos->y = Board::START_Y + (char)RANDOM(Board::HEIGHT);
}

void Player::GetCornerPlace(Point* pos, ClientDirection* dir)
{
	*dir = (ClientDirection::Type)RANDOM(2);

	if (!(m_MyBoard->GetCellState(Board::START_X,Board::START_Y) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X, Board::START_Y + Board::HEIGHT - 1) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X + Board::WIDTH - 1, Board::START_Y) == SHIP_STATE &&
		m_MyBoard->GetCellState(Board::START_X + Board::WIDTH - 1, Board::START_Y + Board::HEIGHT - 1) == SHIP_STATE))
	{
		int idx;
		idx = RANDOM(4);

		pos->x = (idx % 2) ? 'h' : Board::START_X;
		pos->y = (idx / 2) ? '8' : Board::START_Y;
		if (idx % 2 == 1 && *dir == ClientDirection::RIGHT)
		{
			*dir = dir->GetReverseDir();
		}
		if (idx / 2 == 1 && *dir == ClientDirection::DOWN)
		{
			*dir = dir->GetReverseDir();
		}
	}
	else
	{
		pos->x = Board::START_X + (char)RANDOM(Board::WIDTH);
		pos->y = Board::START_Y + (char)RANDOM(Board::HEIGHT);
	}
}

void Player::GetPlace(Point* pos, ClientDirection* dir)
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

void Player::GetCornerFavoredPlace(Point* pos, ClientDirection* dir)
{
	*dir = (ClientDirection::Type)(RANDOM(2) + 1);

	pos->x = Board::START_X + (char)RANDOM(Board::WIDTH - 4);
	pos->y = Board::START_Y + 4 + (char)RANDOM(Board::HEIGHT - 4);
}
