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
	if (pos.y < START_Y + HEIGHT && pos.y >= START_Y &&
		pos.x >= START_X && pos.x < START_X + WIDTH)
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
		printf("%-2c", START_X + x);
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
					printf("□");
					break;
				case MISS_STATE:
					SetConsoleTextAttribute(console, 0x000c);
					printf("■");
					break;
				case SHIP_STATE:
					SetConsoleTextAttribute(console, 0x0004);
					printf("■");
					break;
				case HIT_STATE:
					SetConsoleTextAttribute(console, 0x000a);
					printf("■");
					break;
				case DESTROY_STATE:
					SetConsoleTextAttribute(console, 0x0008);
					printf("■");
					break;
				}
				SetConsoleTextAttribute(console, 0x000f);
			}
		}
		printf("\n");
	}
	
}


int Board::GetAroundNoneStateNum(Point pos)
{
	Point around;
	int aloneGrade = 0;

	for (ClientDirection dir = ClientDirection::BEGIN; dir < ClientDirection::END; dir++)
	{
		around = pos;
		around = around.ChangeByDir(dir);
		if (GetCellState(around) == NONE_STATE ||
			GetCellState(around) == ERROR_STATE)
		{
			aloneGrade++;
		}
	}
	return aloneGrade;
}

int Board::GetAroundNoneStateNum(char x, char y)
{
	Point pos = Point(x, y);

	return GetAroundNoneStateNum(pos);
}

void Board::UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips)
{	
	UpdateDestroyCell(destroyDataList);
	UpdateInvaildShipCell(numOfEnemyShips);
}

int Board::GetHitSize(Point pos, ClientDirection dir)
{
	int size = 0;

	while (GetCellState(pos) == HIT_STATE)
	{
		size++;
		pos = pos.ChangeByDir(dir);
	}

	return size;
}

int Board::GetHitSize(char x, char y, ClientDirection dir)
{
	Point pos = Point(x, y);

	return GetHitSize(pos, dir);
}

int Board::GetMaxHitSize(Point pos, ClientDirection dir)
{
	int size;

	size = GetHitSize(pos, dir) + GetHitSize(pos, ~dir) - 1;

	return size;
}

int Board::GetMaxHitSize(char x, char y, ClientDirection dir)
{
	Point pos = Point(x, y);

	return GetMaxHitSize(pos, dir);
}

int Board::GetNoneSize(Point pos, ClientDirection dir)
{
	int size = 0;

	pos = pos.ChangeByDir(dir);
	while (GetCellState(pos) == NONE_STATE)
	{
		size++;
		pos = pos.ChangeByDir(dir);
	}
	return size;
}

int Board::GetNoneSize(char x, char y, ClientDirection dir)
{
	Point pos = Point(x, y);

	return GetNoneSize(pos, dir);
}

int Board::GetPossibleAttackRange(Point pos)
{
	int maxSize = 0;

	for (ClientDirection dir = ClientDirection::DOWN; dir <= ClientDirection::RIGHT; dir++)
	{
		Point rangePos = pos;
		int size = 0;

		while (GetCellState(rangePos) == NONE_STATE ||
			GetCellState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos = rangePos.ChangeByDir(dir);
		}

		rangePos = pos;
		while (GetCellState(rangePos) == NONE_STATE ||
			GetCellState(rangePos) == HIT_STATE)
		{
			size++;
			rangePos = rangePos.ChangeByDir(dir.GetReverseDir());
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
			size = ClientShipData::GetSize((ClientShipType)i);
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

bool Board::IsValidPlace(Point pos, ClientDirection dir, int length)
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

bool Board::IsValidPlace(char x, char y, ClientDirection dir, int length)
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
	//배가 파괴되었으나 파괴위치를 확정시키치 못한 게 남아있으면 해당 배의 위치를 확정시킬 수 있는지 확인해보고 가능하면 확정시킨다.

	while (!destroyDataList.empty())
	{
		std::list<Point> completePoints;

		for (auto& destroyData : destroyDataList)
		{
			Point pos = destroyData.first;
			HitResult res = destroyData.second;
			int size = ClientShipData::GetSize(res);

			//상하, 좌우 중 어느 한 쪽으로만 길이가 배 크기와 동일한 경우 확정 가능.
			if (GetMaxHitSize(pos, ClientDirection::DOWN) == size&&
				GetMaxHitSize(pos, ClientDirection::RIGHT) < size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos = pos.ChangeByDir(ClientDirection::UP);
				}
				for (int s = 0; s < size; s++)
				{
					pos = pos.ChangeByDir(ClientDirection::DOWN);
					SetCellState(pos, DESTROY_STATE);
				}
				completePoints.push_back(destroyData.first);
			}
			else if (GetMaxHitSize(pos, ClientDirection::DOWN) < size &&
				GetMaxHitSize(pos, ClientDirection::RIGHT) == size)
			{
				while (GetCellState(pos) == HIT_STATE)
				{
					pos = pos.ChangeByDir(ClientDirection::LEFT);
				}
				for (int s = 0; s < size; s++)
				{
					pos = pos.ChangeByDir(ClientDirection::RIGHT);
					SetCellState(pos, DESTROY_STATE);

				}

				completePoints.push_back(destroyData.first);
			}
			else
			{
				//현재 위치를 기준으로 4 방향 따져봤을 때 한쪽만 size 이상이고 나머진 아니라면 해당방향으로 확정 가능.
				for (ClientDirection dir = ClientDirection::BEGIN; dir < ClientDirection::END; dir++)
				{
					if (GetHitSize(pos, dir) >= size&&
						GetHitSize(pos, dir + 1) < size &&
						GetHitSize(pos, dir + 2) < size &&
						GetHitSize(pos, dir + 3) < size)
					{
						//destroy cell로 변경
						for (int s = 0; s < size; s++)
						{
							SetCellState(pos, DESTROY_STATE);
							pos = pos.ChangeByDir(dir);
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
