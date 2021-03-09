#include "ReplayTick.h"
#include "Inputs.h"

ReplayTick::ReplayTick(std::map<int, std::bitset<int(Inputs::SIZE)>> inputs, int timeStamp)
{
	_inputs = inputs;
	_timeStamp = timeStamp;
}
