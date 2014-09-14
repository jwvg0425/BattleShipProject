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

	//�⺻ �÷��̾� ������
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPos;
	Point m_AttackStartPos;

	//���� AI�� ���� ������
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

	//�� ��ġ AI�� ���� ������
	PlaceType m_PlaceType;

	//���� AI���� �Լ�
	void GetPosByExperience(Point& pos);
	void UpdateMonteCarloBoard();
	void UpdateGameDataBoard();
	void UpdateAIState(HitResult prevRes);
	bool ChangeAttackDir();
	
	//��ġ AI���� �Լ�
	void GetPlace(Point& pos, Direction& dir);
	void GetRandomPlace(Point& pos, Direction& dir);
	void GetCornerPlace(Point& pos, Direction& dir);
	void GetCornerFavoredPlace(Point& pos, Direction& dir);

	//AIó���� ���� �����͸� �ܺη� ���� �ҷ��鿩 �д´�.
	void LoadData();
};