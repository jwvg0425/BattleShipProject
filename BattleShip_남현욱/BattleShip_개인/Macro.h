#pragma once

#define RANDOMIZE() (srand((unsigned int)time(NULL)))
#define RANDOM(n) (rand()%(n))
#define SAFE_DELETE(ptr) if(ptr){delete ptr; ptr = nullptr;}
#define SAFE_DELETE_ARR(pArr) if(pArr){delete[] pArr; pArr = nullptr;}
#define DATA_FILE_NAME "data.txt"
#define BUF_SIZE 255