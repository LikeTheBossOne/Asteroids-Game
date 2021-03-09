#pragma once
#include <map>
#include <vector>
#include "Inputs.h"
#include "EventHandler.h"
#include <bitset>

class InputEvent;
class GameObject;

class InputManager : public EventHandler
{
public:
	static InputManager* getInstance();

	void onEvent(Event* e) override;
	void handleInputEvent(InputEvent* e);

	std::bitset<int(Inputs::SIZE)> getInputs(int GUID);
	std::map<int, std::bitset<int(Inputs::SIZE)>> getAllInputs() { return _inputs; };
private:
	InputManager();
	static InputManager* _instance;

	std::map<int, std::bitset<int(Inputs::SIZE)>> _inputs;
};

