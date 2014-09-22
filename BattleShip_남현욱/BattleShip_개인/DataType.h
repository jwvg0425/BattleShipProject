#pragma once
#include"Enums.h"

class ClientDirection
{
public:
	enum Type
	{
		DOWN,
		RIGHT,
		UP,
		LEFT,
		BEGIN = DOWN,
		END = LEFT + 1,
		ERROR_VALUE = -1,
	};

	ClientDirection(Type type);
	ClientDirection();
	~ClientDirection();

	//현재 방향의 다음 방향을 구한다(반시계 방향).
	ClientDirection& operator ++(int num);
	Type operator +(int num);

	//현재 방향의 반대방향을 구한다.
	Type operator ~();

	bool operator <(Type type);
	bool operator <(ClientDirection& dir);
	bool operator >(Type type);
	bool operator >(ClientDirection& dir);
	bool operator ==(Type type);
	bool operator ==(ClientDirection& dir);
	bool operator >=(Type type);
	bool operator >=(ClientDirection& dir);
	bool operator <=(Type type);
	bool operator <=(ClientDirection& dir);
	ClientDirection& operator =(Type type);
	ClientDirection& operator =(ClientDirection& dir);
	
	//방향에 따른 x,y 델타 값을 dx,dy에 저장해 돌려준다.
	void GetDeltaValue(int& dx, int& dy);

	Type GetValue()
	{
		return value;
	}

	Type GetReverseDir();
private:
	Type value;
};

struct Point
{
	char x, y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(char x_, char y_)
	{
		x = x_; y = y_;
	}

	bool operator == (const Point& pos) const
	{
		return (pos.x == this->x && pos.y == this->y);
	}

	bool operator <(const Point& pos) const
	{
		return (this->x + this->y) < (pos.x + pos.y);
	}

	Point ChangeByDir(ClientDirection dir);
};

//Ship에 괸련된 데이터를 저장하는 구조체다.
struct ClientShipData
{
	static const int TYPE_NUM = 4;
	static int GetSize(ClientShipType type);
	static int GetNum(ClientShipType type);
	static ClientShipType GetType(HitResult result);
	static int GetSize(HitResult result);
};