#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"

class Board
{
public:
	static const char START_X = 'a';
	static const char START_Y = '1';
	static const int WIDTH = 8;
	static const int HEIGHT = 8;

	Board();
	~Board();
	void Init();

	//보드 정보 설정 및 얻어오기
	BoardState GetCellState(Point pos) const;
	BoardState GetCellState(char x, char y) const;
	void SetCellState(Point pos, BoardState state);
	void SetCellState(char x, char y, BoardState state);

	void Print();

	bool IsInBoard(Point pos) const;
	bool IsInBoard(char x, char y) const;
	
	//주변 4칸에 존재하는 NONE_STATE인 cell의 개수를 돌려준다. 
	int GetAroundNoneStateNum(Point pos);
	int GetAroundNoneStateNum(char x, char y);

	//전체 cell의 데이터를 현재 진행 상황에 맞게 갱신한다.
	void UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips);

	//해당 방향으로 일렬로 연속해서 존재하는 HIT_STATE인 cell의 길이를 구한다.
	int GetHitSize(Point pos, ClientDirection dir);
	int GetHitSize(char x, char y, ClientDirection dir);

	//상하 또는 좌우로 pos를 포함하며 연속해서 존재하는 HIT_STATE인 cell의 길이를 구한다. 
	int GetMaxHitSize(Point pos, ClientDirection dir);
	int GetMaxHitSize(char x, char y, ClientDirection dir);
	
	//배를 놓을 수 있는 위치인지를 판별해서 돌려준다.
	bool IsValidPlace(Point pos, ClientDirection dir, int length);
	bool IsValidPlace(char x, char y, ClientDirection dir, int length);

	//공격 가능한 위치인지를 판별해서 돌려준다.
	bool IsValidAttackPos(Point pos);
	bool IsValidAttackPos(char x, char y);

	//현재 board에 있는 HIT_STATE인 cell의 위치를 호출할 때마다 순서대로 찾아내 돌려준다.
	Point GetHitCell(bool isFirst);

	//해당 방향으로 일렬로 연속해서 존재하는 NONE_STATE인 cell의 길이를 구한다.
	int GetNoneSize(Point pos, ClientDirection dir);
	int GetNoneSize(char x, char y, ClientDirection dir);

	//해당 포지션을 중심으로 배가 존재할 수 있는 최대 길이를 구해 돌려준다.
	int GetPossibleAttackRange(Point pos);
	int GetPossibleAttackRange(char x, char y);

	//현재 남은 배 상황에서 해당 위치가 공격 시작점이 될 수 있는 지를 판별해 돌려준다.
	bool IsValidAttackStartPos(Point pos,int* numOfEnemyShips);
	bool IsValidAttackStartPos(char x, char y, int* numOfEnemyShips);

protected:
	BoardState m_BoardStates[Board::WIDTH][Board::HEIGHT];
	void UpdateDestroyCell(std::map<Point, HitResult>& destroyDataList);
	void UpdateInvaildShipCell(int* numOfEnemyShips);
};

