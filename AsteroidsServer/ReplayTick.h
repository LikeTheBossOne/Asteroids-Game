#pragma once
#include <bitset>
#include <map>
#include "Inputs.h"

class ReplayTick
{
public:
	ReplayTick(std::map<int, std::bitset<int(Inputs::SIZE)>> inputs, int timeStamp);
	
	std::map<int, std::bitset<int(Inputs::SIZE)>> getInputs() { return _inputs; }
	int getTimestamp() { return _timeStamp; }
private:
	std::map<int, std::bitset<int(Inputs::SIZE)>> _inputs;

	int _timeStamp;
};

