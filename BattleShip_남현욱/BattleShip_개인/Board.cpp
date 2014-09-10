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
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
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

	return m_BoardStates[pos.x - 'a'][pos.y - '1'];
}

void Board::SetCellState(Point pos, BoardState state)
{
	if (!IsInBoard(pos))
	{
		return;
	}

	m_BoardStates[pos.x - 'a'][pos.y - '1'] = state;
}

bool Board::IsInBoard(Point pos)
{
	if (pos.y < '1' + BOARD_HEIGHT && pos.y >= '1' &&
		pos.x >= 'a' && pos.x < 'a' + BOARD_WIDTH)
	{
		return true;
	}

	return false;
}

void Board::Print()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("   ");
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		printf("%-2c", 'a' + x);
	}
	printf("\n");

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x <= BOARD_WIDTH; x++)
		{
			if (x == 0)
			{
				printf("%d ", y + 1);
			}
			else
			{
				switch (GetCellState(Point(x + 'a' - 1, y + '1')))
				{
				case NONE_STATE:
					printf("¡à");
					break;
				case MISS_STATE:
					SetConsoleTextAttribute(console, 0x000c);
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

	for (Direction dir = DOWN; dir <= LEFT; dir = (Direction)(dir + 1))
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

void Board::UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips)
{
	while (!destroyDataList.empty())
	{
		std::list<Point> completePoints;
		for (auto& destroyData : destroyDataList)
		{
			Point pos = destroyData.first;
			HitResult res = destroyData.second;
			int size = ShipData::GetShipSize(res);

			if (GetMaxHitSize(pos, DOWN) == size&&GetMaxHitSize(pos, RIGHT) < size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(UP);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(DOWN);
					SetCellState(pos, DESTROY_STATE);
				}
				completePoints.push_back(destroyData.first);
			}
			else if (GetMaxHitSize(pos, DOWN) < size &&GetMaxHitSize(pos, RIGHT) == size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos.ChangeByDir(LEFT);
				}
				for (int s = 0; s < size; s++)
				{
					pos.ChangeByDir(RIGHT);
					SetCellState(pos, DESTROY_STATE);

				}

				completePoints.push_back(destroyData.first);
			}
			else
			{
				for (Direction dir = DOWN; dir <= LEFT; dir = (Direction)(dir + 1))
				{
					if (GetHitSize(pos, dir) >= size&&
						GetHitSize(pos, (Direction)((dir + 1) % 4)) < size &&
						GetHitSize(pos, (Direction)((dir + 2) % 4)) < size &&
						GetHitSize(pos, (Direction)((dir + 3) % 4)) < size)
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
		if (completePoints.empty())
		{
			completePoints.clear();
			break;
		}
		completePoints.clear();
	}

	
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			Point pos = Point('a' + (char)x, '1' + (char)y);
			if (GetCellState(pos) != NONE_STATE)
				continue;

			if (!IsValidAttackStartPos(pos, numOfEnemyShips))
			{
				SetCellState(pos, MISS_STATE);
			}
		}
	}
	
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

int Board::GetMaxHitSize(Point pos, Direction dir)
{
	int size;
	Point head = pos, tail = pos;

	while (GetCellState(head) == HIT_STATE)
	{
		head.ChangeByDir(dir);
	}
	while (GetCellState(tail) == HIT_STATE)
	{
		tail.ChangeByDir((Direction)((dir + 2) % 4));
	}

	size = abs(head.x - tail.x + head.y - tail.y - 1);

	return size;
}

int Board::GetLengthToNotNoneCell(Point pos, Direction dir)
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

int Board::GetPossibleAttackRange(Point pos)
{
	int maxSize = 0;

	for (Direction dir = DOWN; dir <= RIGHT; dir = (Direction)(dir + 1))
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
			rangePos.ChangeByDir((Direction)((dir + 2) % 4));
		}
		if (size > maxSize)maxSize = size;
	}

	return maxSize - 1;
}

bool Board::IsValidAttackStartPos(Point pos,int* numOfEnemyShips)
{
	int size = 0;

	for (int i = 0; i < 4; i++)
	{
		if (numOfEnemyShips[i]>0)
		{
			size = ShipData::GetShipSize((ShipType)i);
			break;
		}
	}

	if (GetPossibleAttackRange(pos) >= size)
	{
		return true;
	}

	return false;
}

bool Board::IsValidPlace(Point pos, Direction dir, int length)
{
	int dx = 0, dy = 0;

	if (dir == Direction::DOWN)
	{
		dx = 0;
		dy = 1;
	}
	else
	{
		dx = 1;
		dy = 0;
	}


	for (int i = 0; i < length; i++)
	{
		if (GetCellState(pos) != BoardState::NONE_STATE)
		{
			return false;
		}

		pos.x += (char)dx;
		pos.y += (char)dy;
	}

	return true;
}

Point Board::GetHitCell(bool isFirst)
{
	static Point prevHitCell = Point('a', '0');

	if (isFirst)
	{
		prevHitCell = Point('a', '0');
	}

	for (int x = prevHitCell.x - 'a'; x < BOARD_WIDTH; x++)
	{
		for (int y = prevHitCell.y - '1'; y < BOARD_HEIGHT; y++)
		{
			Point checkPos = Point('a' + (char)x, '1' + (char)y);

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
