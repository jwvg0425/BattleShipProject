#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"

class Ship;
class Board;


class Player
{
public:
	Player();
	~Player();
	void Init();
	void PlaceShips();
	Point GetNextAttackPoint();
	bool IsDead();

	void RecieveAttackResult(Point pos, HitResult res);
	HitResult SendAttackResult(Point pos);

	void PrintShipData();
	void PrintEnemyBoardData();

private:
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPoint;
	Point m_AttackStartPoint;
	Direction m_AttackDir;
	AIState m_AIState;
	std::vector<Point> m_FindPoints[4];
	std::vector<Point> m_SecondFindPoints[4];
	std::map<Point,HitResult> m_DestroyData;
	int m_NumOfEnemyShips[4];
	int m_HitCount;
	bool IsValidPlace(Point pos, Direction dir, int length);
	void ChangeAIState(HitResult prevRes);
	bool ChangeAttackDir();
	bool IsValidAttackPos(Point pos);
	void UpdateFindPoints();
	void InitFindPoints();
	void UpdateCellData();
	Point GetHitCell(bool isFirst);
	int GetHitSize(Point pos, Direction dir);
	int GetMaxHitSize(Point pos, Direction dir);
	int GetLengthToNotNoneCell(Point pos, Direction dir);
	int GetPossibleAttackRange(Point pos);
	bool IsValidAttackStartPos(Point pos);
};