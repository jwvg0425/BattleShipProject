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

	//배 배치 타입.
	//RANDOM = 완전 랜덤
	//CORNER = 4 코너에 각각 하나씩 배치, destroyer 하나는 랜덤 배치
	//CORNER_FAVORED = 왼쪽 아래 코너 부근에 집중적으로 배치
	enum PlaceType
	{
		RANDOM,
		CORNER,
		CORNER_FAVORED,
		PLACE_TYPE_NUM = 3,
	};

	

private:

	//기본 플레이어 데이터
	std::vector<Ship*> m_ShipList;
	Board* m_MyBoard;
	Board* m_EnemyBoard;
	Point m_PrevAttackPos;
	Point m_AttackStartPos;

	//공격 AI를 위한 데이터
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

	//배 배치 AI를 위한 데이터
	PlaceType m_PlaceType;

	

	//공격 AI관련 함수
	Point GetPosByExperience();
	void UpdateMonteCarloBoard();
	void UpdateGameDataBoard();
	void UpdateAIState(HitResult prevRes);
	bool ChangeAttackDir();
	
	//배치 AI관련 함수
	void GetPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetRandomPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetCornerPlace(OUT Point* pos, OUT ClientDirection* dir);
	void GetCornerFavoredPlace(OUT Point* pos, OUT ClientDirection* dir);

	//AI처리를 위한 데이터를 외부로 부터 불러들여 읽는다.
	void LoadData();

	ShipData m_ShipData;
	char mapdata[64];
};