#pragma once
#include "Rect.h"
#include "GameObject.h"

class PhysicsEngineSettings;

class Laser : public GameObject
{
public:
	Laser(PhysicsEngineSettings* physSettings, Rect position, float velocityX, float velocityY);
	Laser(const Laser& other, PhysicsEngineSettings* physSettings);
	
	std::string getType() override { return "Laser"; }
};

