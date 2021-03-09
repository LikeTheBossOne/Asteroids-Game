#pragma once
#include "Event.h"
class CreateAsteroidsEvent : public Event
{
public:
	CreateAsteroidsEvent(int amount);
	
	std::string getType() override { return "CreateAsteroids"; }

	void setAmount(int amount) { _amount = amount; }

	int getAmount() { return _amount; }
private:
	int _amount;
};

