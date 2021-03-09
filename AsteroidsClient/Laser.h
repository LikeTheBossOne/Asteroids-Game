#pragma once
#include "GameObject.h"
class Laser : public GameObject
{
public:
	Laser(int GUID, sf::FloatRect location);

	GameObjectTypes getType() override { return GameObjectTypes::Laser; }
};

