#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"

class Board
{
public:
	Board();
	virtual ~Board();
	void Init();
	BoardState GetCellState(Point pos);
	void SetCellState(Point pos, BoardState state);
	bool IsInBoard(Point pos);
	virtual void Print();
	int GetAloneGrade(Point pos);
	void UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips);
	int GetHitSize(Point pos, Direction dir);
	int GetMaxHitSize(Point pos, Direction dir);
	bool IsValidPlace(Point pos, Direction dir, int length);
	bool IsValidAttackPos(Point pos);
	Point GetHitCell(bool isFirst);
	int GetLengthToNotNoneCell(Point pos, Direction dir);
	int GetPossibleAttackRange(Point pos);
	bool IsValidAttackStartPos(Point pos,int* numOfEnemyShips);

protected:
	BoardState m_BoardStates[BOARD_WIDTH][BOARD_HEIGHT];
};

