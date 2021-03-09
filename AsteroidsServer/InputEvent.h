#pragma once
#include "Event.h"
#include <vector>
#include <bitset>
#include "Inputs.h"

class InputEvent : public Event
{
public:
	InputEvent(int playerGUID, std::bitset<int(Inputs::SIZE)> inputs);
	std::string getType() override { return "Input"; }

	int playerGUID;
	std::bitset<int(Inputs::SIZE)> inputs;
};

