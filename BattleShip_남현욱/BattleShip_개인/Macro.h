#pragma once

#define RANDOMIZE() (srand((unsigned int)time(NULL)))
#define RANDOM(n) (rand()%(n))