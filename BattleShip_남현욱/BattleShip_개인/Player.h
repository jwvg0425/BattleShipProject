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
	Point GetNextAttackPos();
	bool IsDead();

	void RecieveAttackResult(Point pos, HitResult res);
	HitResult SendAttackResult(Point pos);

	void PrintShipData();
	void PrintEnemyBoardData();

private:
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPos;
	Point m_AttackStartPos;
	Direction m_AttackDir;
	AIState m_AIState;
	std::vector<Point> m_FindPos[4];
	std::vector<Point> m_SecondFindPos[4];
	std::map<Point,HitResult> m_DestroyData;
	int m_NumOfEnemyShips[4];
	int m_HitCount;
	int m_AttackCount;
	
	void ChangeAIState(HitResult prevRes);
	bool ChangeAttackDir();
	
	void UpdateFindPos();
	void InitFindPos();
	
};