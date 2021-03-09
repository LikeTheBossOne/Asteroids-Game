#pragma once
#include "GameObject.h"

class SpawnZone;
class PhysicsEngineSettings;

class PlayerShip : public GameObject
{
public:
	PlayerShip(PhysicsEngineSettings* physSettings, float width, float height, std::string textureName, SpawnZone* spawn);
	PlayerShip(const PlayerShip& other, PhysicsEngineSettings* physSettings, SpawnZone* spawn);

	std::string getType() override { return "PlayerShip"; }
};

