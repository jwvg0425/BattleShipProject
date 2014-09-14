#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"
#include "Board.h"

class Ship;


class Player
{
public:
	Player();
	~Player();
	void Init(bool isReset);
	void PlaceShips();
	bool IsDead();

	Point GetNextAttackPos();

	void RecieveAttackResult(Point pos, HitResult res);
	HitResult SendAttackResult(Point pos);

	void PrintShipData();
	void PrintEnemyBoardData();

private:
	enum PlaceType
	{
		RANDOM,
		CORNER,
		CORNER_FAVORED,
		EXPERIENCE,
	};

	//기본 플레이어 데이터
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPos;
	Point m_AttackStartPos;

	//공격 AI를 위한 데이터
	static const int SAVING_DATA_NUM = 4000;
	static const int SIMULATE_NUM = 1000;
	Direction m_AttackDir;
	AIState m_AIState;
	std::vector<Point> m_GameData[SAVING_DATA_NUM];
	std::map<Point,HitResult> m_DestroyData;
	int m_NumOfEnemyShips[4];
	int m_HitCount;
	int m_AttackCount;
	int m_GameCount;
	int m_MonteCarloBoard[Board::WIDTH][Board::HEIGHT];
	int m_GameDataBoard[Board::WIDTH][Board::HEIGHT];
	int m_GameDataCount;

	//배 배치 AI를 위한 데이터
	PlaceType m_PlaceType;

	//공격 AI관련 함수
	void GetPosByExperience(Point& pos);
	void UpdateMonteCarloBoard();
	void UpdateGameDataBoard();
	void UpdateAIState(HitResult prevRes);
	bool ChangeAttackDir();
	
	//배치 AI관련 함수
	void GetPlace(Point& pos, Direction& dir);
	void GetRandomPlace(Point& pos, Direction& dir);
	void GetCornerPlace(Point& pos, Direction& dir);
	void GetCornerFavoredPlace(Point& pos, Direction& dir);

	//AI처리를 위한 데이터를 외부로 부터 불러들여 읽는다.
	void LoadData();
};