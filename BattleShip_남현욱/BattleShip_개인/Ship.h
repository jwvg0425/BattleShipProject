#pragma once
#include "Enums.h"
#include "DataType.h"

class Ship
{
public:
	Ship();
	virtual ~Ship();
	void Init();
	virtual HitResult HitCheck(Point pos);
	int HasPosition(Point pos);
	void AddPosition(Point pos);
	int GetMaxHP();
	int GetHP();
	ClientShipType GetType(){return m_Type;}
	void PrintData();
protected:
	std::string m_Name;
	ClientShipType m_Type;
	int m_HP;
	int m_MaxHP;
	Point* m_Pos;
};