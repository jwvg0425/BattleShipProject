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

BoardState Board::GetBoardState(Point pos)
{
	if (!IsInBoard(pos))
	{
		return ERROR_STATE;
	}

	return m_BoardStates[pos.x - 'a'][pos.y - '1'];
}

void Board::SetBoardState(Point pos, BoardState state)
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
	printf("  a b c d e f g h\n");
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
				switch (GetBoardState(Point(x + 'a' - 1, y + '1')))
				{
				case NONE_STATE:
					printf("бр");
					break;
				case MISS_STATE:
					SetConsoleTextAttribute(console, 0x000c);
					printf("бс");
					break;
				case HIT_STATE:
					SetConsoleTextAttribute(console, 0x000a);
					printf("бс");
					break;
				case DESTROY_STATE:
					SetConsoleTextAttribute(console, 0x0008);
					printf("бс");
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
		if (GetBoardState(around) == NONE_STATE ||
			GetBoardState(around) == ERROR_STATE)
		{
			aloneGrade++;
		}
	}
	return aloneGrade;
}
