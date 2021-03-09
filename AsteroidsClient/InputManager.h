#pragma once
#include <bitset>
#include <map>
#include "Inputs.h"

class GameObject;

class InputManager
{
public:
	static InputManager* getInstance();

	void setInputs(int GUID, std::bitset<int(Inputs::SIZE)> inputs) { _inputs[GUID] = inputs; }
	
	std::bitset<int(Inputs::SIZE)> getInputs(int GUID);
	std::map<int, std::bitset<int(Inputs::SIZE)>> getAllInputs() { return _inputs; };
private:
	InputManager();
	static InputManager* _instance;

	std::map<int, std::bitset<int(Inputs::SIZE)>> _inputs;
};

