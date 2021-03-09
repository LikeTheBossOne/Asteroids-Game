#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	Asteroid(int GUID, sf::FloatRect location);

	GameObjectTypes getType() override { return GameObjectTypes::Asteroid; }
};

