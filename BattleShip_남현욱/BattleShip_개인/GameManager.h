#pragma once
#include "DataType.h"

class Player;

class GameManager
{
public:
	static GameManager* GetInstance();
	static void ReleaseInstance();

	void InitGame();
	void RunNetworkGame();
	void RunSingleGame();
	void Print();

private:
	GameManager();
	~GameManager();

private:
	static GameManager* m_Instance;
	Player* m_Player1;
	Player* m_Player2;
	int m_Turn;
};

