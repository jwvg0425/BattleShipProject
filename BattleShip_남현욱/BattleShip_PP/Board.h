#pragma once
class Board
{
public:
	Board();
	~Board();
private:
	int width, height;
	int** board;

public:
	void DrawBoard();
};

