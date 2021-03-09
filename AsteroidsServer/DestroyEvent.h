#pragma once
#include "Event.h"
class GameObject;

class DestroyEvent : public Event
{
public:
	DestroyEvent(GameObject* obj);
	
	std::string getType() override { return "Destroy"; }

	GameObject* getObject() { return _object; }
private:
	GameObject* _object;
};

