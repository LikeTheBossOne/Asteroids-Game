#pragma once
#include "GenericComponent.h"

class RigidBodyComponent;
class PlayerInputComponent;
class PhysicsEngineSettings;

class MovementComponent : public GenericComponent
{
public:
	static bool scriptInitialized;
	
	MovementComponent(GameObject* gameObject);
	MovementComponent(const MovementComponent& other, GameObject* gameObject);

	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::MovementComponent; };

	void setVelocityX(float velocityX) { _velocityX = velocityX; }
	void setVelocityY(float velocityY) { _velocityY = velocityY; }
	void setAccelerationX(float accelerationX) { _accelerationX = accelerationX; }
	void setAccelerationY(float accelerationY) { _accelerationY = accelerationY; }

	float getVelocityX() { return _velocityX; }
	float getVelocityY() { return _velocityY; }
	float getAccelerationX() { return _accelerationX; }
	float getAccelerationY() { return _accelerationY; }
	
private:
	float _accelerationX;
	float _accelerationY;
	
	float _velocityX;
	float _velocityY;
};

