#include "InputEvent.h"
#include "Inputs.h"

InputEvent::InputEvent(int playerGUID, std::bitset<int(Inputs::SIZE)> inputs) : Event(2, 0)
{
	this->playerGUID = playerGUID;
	this->inputs = inputs;
}
