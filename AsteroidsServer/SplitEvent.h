#pragma once
#include "Event.h"
class GameObject;

class SplitEvent : public Event
{
public:
	SplitEvent(GameObject* obj);
	
	std::string getType() override { return "Split"; }

	GameObject* getObject() { return _object; }
private:
	GameObject* _object;
};

