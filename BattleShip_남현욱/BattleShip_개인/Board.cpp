#include "stdafx.h"
#include "Board.h"
#include "Macro.h"
#include "DataType.h"


Board::Board()
{
}


Board::~Board()
{
}

void Board::Init()
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			m_BoardStates[i][j] = NONE_STATE;
		}
	}
}

BoardState Board::GetCellState(Point pos)
{
	if (!IsInBoard(pos))
	{
		return ERROR_STATE;
	}

	return m_BoardStates[pos.x - Board::START_X][pos.y - Board::START_Y];
}

BoardState Board::GetCellState(char x, char y)
{
	Point pos;
	pos.x = x;
	pos.y = y;

	return GetCellState(pos);
}

void Board::SetCellState(Point pos, BoardState state)
{
	if (!IsInBoard(pos))
	{
		return;
	}

	m_BoardStates[pos.x - Board::START_X][pos.y - Board::START_Y] = state;
}

void Board::SetCellState(char x, char y,BoardState state)
{
	Point pos;
	pos.x = x;
	pos.y = y;

	SetCellState(pos, state);
}

bool Board::IsInBoard(Point pos)
{
	if (pos.y < Board::START_Y + HEIGHT && pos.y >= Board::START_Y &&
		pos.x >= Board::START_X && pos.x < Board::START_X + WIDTH)
	{
		return true;
	}

	return false;
}

bool Board::IsInBoard(char x, char y)
{
	Point pos = Point(x, y);

	return IsInBoard(pos);
}

void Board::Print()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("   ");
	for (int x = 0; x < WIDTH; x++)
	{
		printf("%-2c", Board::START_X + x);
	}
	printf("\n");

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x <= WIDTH; x++)
		{
			if (x == 0)
			{
				printf("%d ", y + 1);
			}
			else
			{
				switch (GetCellState((char)x + START_X - 1, (char)y + START_Y))
				{
				case NONE_STATE:
					printf("¡à");
					break;
				case MISS_STATE:
					SetConsoleTextAttribute(console, 0x000c);
					printf("¡á");
					break;
				case SHIP_STATE:
					SetConsoleTextAttribute(console, 0x0004);
					printf("¡á");
					break;
				case HIT_STATE:
					SetConsoleTextAttribute(console, 0x000a);
					printf("¡á");
					break;
				case DESTROY_STATE:
					SetConsoleTextAttribute(console, 0x0008);
					printf("¡á");
					break;
				}
				SetConsoleTextAttribute(console, 0x000f);
			}
		}
		printf("\n");
	}
	
}


int Board::GetAloneGrade(Point pos)
{
	Point around;
	int aloneGrade = 0;

	for (Direction dir = Direction::BEGIN; dir < Direction::END; dir++)
	{
		around = pos;
		around.ChangeByDir(dir);
		if (GetCellState(around) == NONE_STATE ||
			GetCellState(around) == ERROR_STATE)
		{
			aloneGrade++;
		}
	}
	return aloneGrade;
}

int Board::GetAloneGrade(char x, char y)
{
	Point pos = Point(x, y);

	return GetAloneGrade(pos);
}

void Board::UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips)
{	
	UpdateDestroyCell(destroyDataList);
	UpdateInvaildShipCell(numOfEnemyShips);
}

int Board::GetHitSize(Point pos, Direction dir)
{
	int size = 0;

	while (GetCellState(pos) == HIT_STATE)
	{
		size++;
		pos.ChangeByDir(dir);
	}

	return size;
}

int Board::GetHitSize(char x, char y, Direction dir)
{
	Point pos = Point(x, y);

	return GetHitSize(pos, dir);
}

int Board::GetMaxHitSize(Point pos, Direction dir)
{
	int size;

	size = GetHitSize(pos, dir) + GetHitSize(pos, ~dir) - 1;

	return size;
}

int Board::GetMaxHitSize(char x, char y, Direction dir)
{
	Point pos = Point(x, y);

	return GetMaxHitSize(pos, dir);
}

int Board::GetNoneSize(Point pos, Direction dir)
{
	int size = 0;

	pos.ChangeByDir(dir);
	while (GetCellState(pos) == NONE_STATE)
	{
		size++;
		pos.ChangeByDir(dir);
	}
	return size;
}

int Board::GetNoneSize(char x, char y, Direction dir)
{
	Point pos = Point(x, y);

	return GetNoneSize(pos, dir);
}

int Board::GetPossibleAttackRange(Point pos)
{
	int maxSize = 0;

	for (Direction dir = Direction::DOWN; dir <= Direction::RIGHT; dir++)
	{
		Point rangePos = pos;
		int size = 0;

		while (GetCellState(rangePos) == NONE_STATE ||
			GetCellState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos.ChangeByDir(dir);
		}

		rangePos = pos;
		while (GetCellState(rangePos) == NONE_STATE ||
			GetCellState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos.ChangeByDir(~dir);
		}
		if (size > maxSize)maxSize = size;
	}

	return maxSize - 1;
}

int Board::GetPossibleAttackRange(char x, char y)
{
	Point pos = Point(x, y);

	return GetPossibleAttackRange(pos);
}

bool Board::IsValidAttackStartPos(Point pos,int* numOfEnemyShips)
{
	int size = 0;

	for (int i = DESTROYER; i > AIRCRAFT; i--)
	{
		if (numOfEnemyShips[i]>0)
		{
			size = ShipData::GetSize((ShipType)i);
			break;
		}
	}

	if (GetPossibleAttackRange(pos) >= size)
	{
		return true;
	}

	return false;
}

bool Board::IsValidAttackStartPos(char x, char y, int* numOfEnemyShips)
{
	Point pos = Point(x, y);

	return IsValidAttackStartPos(pos, numOfEnemyShips);
}

bool Board::IsValidPlace(Point pos, Direction dir, int length)
{
	int dx = 0, dy = 0;

	dir.GetDeltaValue(dx, dy);

	for (int i = 0; i < length; i++)
	{
		if (!(GetCellState(pos) == BoardState::NONE_STATE || GetCellState(pos) == HIT_STATE))
		{
			return false;
		}

		pos.x += (char)dx;
		pos.y += (char)dy;
	}

	return true;
}

bool Board::IsValidPlace(char x, char y, Direction dir, int length)
{
	Point pos = Point(x, y);

	return IsValidPlace(pos, dir, length);
}

Point Board::GetHitCell(bool isFirst)
{
	static Point prevHitCell = Point(START_X, START_Y-1);

	if (isFirst)
	{
		prevHitCell = Point(START_X, START_Y-1);
	}

	for (int x = prevHitCell.x - START_X; x < WIDTH; x++)
	{
		for (int y = prevHitCell.y - START_Y; y < HEIGHT; y++)
		{
			Point checkPos = Point(START_X + (char)x, START_Y + (char)y);

			if (checkPos == prevHitCell)
			{
				continue;
			}

			if (GetCellState(checkPos) == BoardState::HIT_STATE)
			{
				prevHitCell = checkPos;
				return checkPos;
			}
		}
	}

	return Point(0, 0);

}

bool Board::IsValidAttackPos(Point pos)
{
	if (GetCellState(pos) == BoardState::NONE_STATE)
	{
		return true;
	}

	return false;
}

bool Board::IsValidAttackPos(char x, char y)
{
	Point pos = Point(x, y);

	return IsValidAttackPos(pos);
}

void Board::UpdateDestroyCell(std::map<Point, HitResult>& destroyDataList)
{
	while (!destroyDataList.empty())
	{
		std::list<Point> completePoints;
		for (auto& destroyData : destroyDataList)
		{
			Point pos = destroyData.first;
			HitResult res = destroyData.second;
			int size = ShipData::GetSize(res);

			if (GetMaxHitSize(pos, Direction::DOWN) == size&&
				GetMaxHitSize(pos, Direction::RIGHT) < size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(Direction::UP);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(Direction::DOWN);
					SetCellState(pos, DESTROY_STATE);
				}
				completePoints.push_back(destroyData.first);
			}
			else if (GetMaxHitSize(pos, Direction::DOWN) < size &&
				GetMaxHitSize(pos, Direction::RIGHT) == size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(Direction::LEFT);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(Direction::RIGHT);
					SetCellState(pos, DESTROY_STATE);

				}

				completePoints.push_back(destroyData.first);
			}
			else
			{
				for (Direction dir = Direction::BEGIN; dir < Direction::END; dir++)
				{
					if (GetHitSize(pos, dir) >= size&&
						GetHitSize(pos, dir + 1) < size &&
						GetHitSize(pos, dir + 2) < size &&
						GetHitSize(pos, dir + 3) < size)
					{
						//destroy cell·Î º¯°æ
						for (int s = 0; s < size; s++)
						{
							SetCellState(pos, DESTROY_STATE);
							pos.ChangeByDir(dir);
						}
						completePoints.push_back(destroyData.first);
						break;
					}
				}
			}
		}

		for (auto point : completePoints)
		{
			destroyDataList.erase(point);
		}
	}
}

void Board::UpdateInvaildShipCell(int* numOfEnemyShips)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			Point pos = Point(START_X + (char)x, START_Y + (char)y);
			if (GetCellState(pos) != NONE_STATE)
				continue;

			if (!IsValidAttackStartPos(pos, numOfEnemyShips))
			{
				SetCellState(pos, MISS_STATE);
			}
		}
	}
}
