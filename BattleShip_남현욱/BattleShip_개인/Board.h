#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"

class Board
{
public:
	Board();
	~Board();
	void Init();
	BoardState GetBoardState(Point pos);
	void SetBoardState(Point pos, BoardState state);
	bool IsInBoard(Point pos);
	void Print();
	int GetAloneGrade(Point pos);

private:
	BoardState m_BoardStates[BOARD_WIDTH][BOARD_HEIGHT];
};

