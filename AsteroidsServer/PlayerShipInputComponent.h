#pragma once
#include "GenericComponent.h"

class PlayerShipInputComponent : public GenericComponent
{
public:
	PlayerShipInputComponent(GameObject* gameObject, float rotationalVelocity, float velocity);
	PlayerShipInputComponent(const PlayerShipInputComponent& other, GameObject* gameObject);

	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::PlayerShipInputComponent; }
private:
	float _rotationalVelocity;
	float _velocity;

	int _timeTillShoot;
};

