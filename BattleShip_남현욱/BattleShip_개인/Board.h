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

	//���� ���� ���� �� ������
	BoardState GetCellState(Point pos) const;
	BoardState GetCellState(char x, char y) const;
	void SetCellState(Point pos, BoardState state);
	void SetCellState(char x, char y, BoardState state);

	void Print();

	bool IsInBoard(Point pos) const;
	bool IsInBoard(char x, char y) const;
	
	//�ֺ� 4ĭ�� �����ϴ� NONE_STATE�� cell�� ������ �����ش�. 
	int GetAroundNoneStateNum(Point pos);
	int GetAroundNoneStateNum(char x, char y);

	//��ü cell�� �����͸� ���� ���� ��Ȳ�� �°� �����Ѵ�.
	void UpdateCellData(std::map<Point, HitResult>& destroyDataList, int* numOfEnemyShips);

	//�ش� �������� �Ϸķ� �����ؼ� �����ϴ� HIT_STATE�� cell�� ���̸� ���Ѵ�.
	int GetHitSize(Point pos, ClientDirection dir);
	int GetHitSize(char x, char y, ClientDirection dir);

	//���� �Ǵ� �¿�� pos�� �����ϸ� �����ؼ� �����ϴ� HIT_STATE�� cell�� ���̸� ���Ѵ�. 
	int GetMaxHitSize(Point pos, ClientDirection dir);
	int GetMaxHitSize(char x, char y, ClientDirection dir);
	
	//�踦 ���� �� �ִ� ��ġ������ �Ǻ��ؼ� �����ش�.
	bool IsValidPlace(Point pos, ClientDirection dir, int length);
	bool IsValidPlace(char x, char y, ClientDirection dir, int length);

	//���� ������ ��ġ������ �Ǻ��ؼ� �����ش�.
	bool IsValidAttackPos(Point pos);
	bool IsValidAttackPos(char x, char y);

	//���� board�� �ִ� HIT_STATE�� cell�� ��ġ�� ȣ���� ������ ������� ã�Ƴ� �����ش�.
	Point GetHitCell(bool isFirst);

	//�ش� �������� �Ϸķ� �����ؼ� �����ϴ� NONE_STATE�� cell�� ���̸� ���Ѵ�.
	int GetNoneSize(Point pos, ClientDirection dir);
	int GetNoneSize(char x, char y, ClientDirection dir);

	//�ش� �������� �߽����� �谡 ������ �� �ִ� �ִ� ���̸� ���� �����ش�.
	int GetPossibleAttackRange(Point pos);
	int GetPossibleAttackRange(char x, char y);

	//���� ���� �� ��Ȳ���� �ش� ��ġ�� ���� �������� �� �� �ִ� ���� �Ǻ��� �����ش�.
	bool IsValidAttackStartPos(Point pos,int* numOfEnemyShips);
	bool IsValidAttackStartPos(char x, char y, int* numOfEnemyShips);

protected:
	BoardState m_BoardStates[Board::WIDTH][Board::HEIGHT];
	void UpdateDestroyCell(std::map<Point, HitResult>& destroyDataList);
	void UpdateInvaildShipCell(int* numOfEnemyShips);
};

