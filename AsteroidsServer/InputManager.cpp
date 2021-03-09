#include "InputManager.h"
#include "GameObject.h"
#include "PlayerInputComponent.h"
#include "EventManager.h"
#include "InputEvent.h"
#include <iostream>

InputManager* InputManager::_instance;

InputManager::InputManager()
{
	_inputs = std::map<int, std::bitset<int(Inputs::SIZE)>>();

	EventManager::getInstance()->registerFor(this, "Input");
}

InputManager* InputManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InputManager();
	}

	return _instance;
}

void InputManager::onEvent(Event* e)
{
	if (e->getType() == "Input")
	{
		handleInputEvent(static_cast<InputEvent*>(e));
	}
}

void InputManager::handleInputEvent(InputEvent* e)
{
	_inputs[e->playerGUID] = e->inputs;
	for (int i = 0; i < int(Inputs::SIZE); i++)
	{
		std::cout << e->inputs[i] << ",";
	}
	std::cout << std::endl;
}

std::bitset<int(Inputs::SIZE)> InputManager::getInputs(int GUID)
{
	return _inputs[GUID];
}
