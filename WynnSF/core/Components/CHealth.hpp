#pragma once
#include "../ECS.hpp"

class CHealth : public Component<CHealth> {

public:
	int MaxHP = 0;
	int CurrHp = 0;
	CHealth() {};

	CHealth(int maxHp) {
		MaxHP = maxHp;
		CurrHp = maxHp;
	};

	~CHealth() {};
};