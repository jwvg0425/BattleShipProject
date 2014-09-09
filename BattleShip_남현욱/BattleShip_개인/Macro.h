#pragma once

#define RANDOMIZE() (srand((unsigned int)time(NULL)))
#define RANDOM(n) (rand()%(n))
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8