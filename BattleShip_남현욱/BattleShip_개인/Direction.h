#pragma once

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