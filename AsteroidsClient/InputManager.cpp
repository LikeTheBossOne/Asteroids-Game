#include "InputManager.h"
#include "Inputs.h"

InputManager* InputManager::_instance;

InputManager::InputManager()
{
	_inputs = std::map<int, std::bitset<int(Inputs::SIZE)>>();
}

InputManager* InputManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InputManager();
	}

	return _instance;
}

std::bitset<int(Inputs::SIZE)> InputManager::getInputs(int GUID)
{
	return _inputs[GUID];
}
