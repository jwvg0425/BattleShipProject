#pragma once
#include "DataType.h"
#include "Enums.h"
#include "Macro.h"
#include "Board.h"
#include "ShipData.h"

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
	ShipData& GetShipPosList();
	void GetMapData(char* mapData_) const;
	

	void RecieveAttackResult(Point pos, HitResult res);
	HitResult SendAttackResult(Point pos);

	void PrintShipData();
	void PrintEnemyBoardData();

	//�� ��ġ Ÿ��.
	//RANDOM = ���� ����
	//CORNER = 4 �ڳʿ� ���� �ϳ��� ��ġ, destroyer �ϳ��� ���� ��ġ
	//CORNER_FAVORED = ���� �Ʒ� �ڳ� �αٿ� ���������� ��ġ
	enum PlaceType
	{
		RANDOM,
		CORNER,
		CORNER_FAVORED,
		PLACE_TYPE_NUM = 3,
	};

	

private:

	//�⺻ �÷��̾� ������
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPos;
	Point m_AttackStartPos;

	//���� AI�� ���� ������
	static const int SAVING_DATA_NUM = 4000;
	static const int SIMULATE_NUM = 1000;
	static const int CHANCE_NUM = 100;
	ClientDirection m_AttackDir;
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
	Point GetPosByExperience();
	void UpdateMonteCarloBoard();
	void UpdateGameDataBoard();
	void UpdateAIState(HitResult prevRes);
	bool ChangeAttackDir();
	
	//��ġ AI���� �Լ�
	void GetPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetRandomPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetCornerPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetCornerFavoredPlace(OUT Point* pos, OUT ClientDirection* dir);

	//AIó���� ���� �����͸� �ܺη� ���� �ҷ��鿩 �д´�.
	void LoadData();

	ShipData m_ShipData;
	char mapdata[64];
};