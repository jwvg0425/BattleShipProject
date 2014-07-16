// BattleShip_PP.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <list>

const char* strDestroy = "Destroy";
const char* strHit = "Hit";
const char* strMiss = "Miss";

int _tmain()
{
	std::string strinput;
	std::string strResult[] = { strHit, strMiss, strDestroy };
	int kind, i;
	int destroyedKind;
	std::list<std::string> ships;
	std::list<std::string> destroyedShips;
	std::list<std::string> ::iterator it;
	ships.push_back("Aircraft");
	ships.push_back("Battleship");
	ships.push_back("Cruiser");
	ships.push_back("Destroyer");
	ships.push_back("Destroyer");
	ships.push_back("Destroyer");

	srand((unsigned int)time(NULL));

	while (!ships.empty())
	{
		std::cin >> strinput;
		kind = rand() % _countof(strResult);
		std::cout << strResult[kind];
		if (strResult[kind] == strDestroy)
		{
			destroyedKind = rand() % ships.size();
			std::cout << " - ";
			
			for (i = 0, it = ships.begin(); i < destroyedKind; i++, it++);

			destroyedShips.push_back(*it);
			ships.erase(it);

			for (it = destroyedShips.begin(); it != destroyedShips. end(); it++)
			{
				std::cout << *it << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "GAME OVER!" << std::endl;
	fflush(stdin);
	getchar();

	return 0;
}