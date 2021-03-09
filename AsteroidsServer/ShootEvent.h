#pragma once
#include "Event.h"
class GameObject;

class ShootEvent : public Event
{
public:
	ShootEvent(GameObject* sendingObject);
	
	std::string getType() override { return "Shoot"; }
	
	GameObject* getSendingObject() { return _sendingObject; }
private:
	GameObject* _sendingObject;
};

