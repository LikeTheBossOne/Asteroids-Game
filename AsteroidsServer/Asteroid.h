#pragma once
#include "GameObject.h"
#include "Rect.h"

class PhysicsEngineSettings;

class Asteroid : public GameObject
{
public:
	Asteroid(PhysicsEngineSettings* physSettings, Rect position, std::string textureNames, float velocityX, float velocityY, int splitsLeft);
	Asteroid(const Asteroid& other, PhysicsEngineSettings* physSettings);

	std::string getType() override { return "Asteroid"; }
};

