// BattleShip_PP.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>



int _tmain(int argc, _TCHAR* argv[])
{
	argc;
	argv;

	char buffer[256] = { 0, };
	char* output[] = { "miss", "hit", "destroy" };

	scanf_s("%s", buffer);

	srand((unsigned int)time(NULL));

	printf_s("%s", output[rand() % (sizeof(output) / sizeof(output[0]))]);

	fflush(stdin);
	getchar();

	return 0;
}

