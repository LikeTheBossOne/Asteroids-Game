#include "PlayerShipInputComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "EventManager.h"
#include "ShootEvent.h"

PlayerShipInputComponent::PlayerShipInputComponent(GameObject* gameObject, float rotationalVelocity, float velocity) : GenericComponent(gameObject)
{
	_rotationalVelocity = rotationalVelocity;
	_velocity = velocity;

	_timeTillShoot = 0;
}

PlayerShipInputComponent::PlayerShipInputComponent(const PlayerShipInputComponent& other, GameObject* gameObject) : GenericComponent(gameObject)
{
	_rotationalVelocity = other._rotationalVelocity;
	_velocity = other._velocity;
	
	_timeTillShoot = other._timeTillShoot;
}

void PlayerShipInputComponent::update(int deltaTime)
{
	const auto inputs = InputManager::getInstance()->getInputs(_gameObject->getGUID());

	const auto transform = _gameObject->getTransform();
	const auto gMovement = _gameObject->getComponent(ComponentTypes::MovementComponent);
	auto movement = gMovement != nullptr ? dynamic_cast<MovementComponent*>(gMovement) : nullptr;


	if (movement->getVelocityX() > 0)
	{
		movement->setVelocityX(movement->getVelocityX() - 0.001f);
	}
	else if (movement->getVelocityX() < 0)
	{
		movement->setVelocityX(movement->getVelocityX() + 0.001f);
	}
	if (movement->getVelocityY() > 0)
	{
		movement->setVelocityY(movement->getVelocityY() - 0.001f);
	}
	else if (movement->getVelocityY() < 0)
	{
		movement->setVelocityY(movement->getVelocityY() + 0.001f);
	}

	
	if (inputs[int(Inputs::ROT_CLOCK)] && transform != nullptr)
	{
		transform->setRotation(transform->getRotation() + _rotationalVelocity);
	}
	else if (inputs[int(Inputs::ROT_COUNTER_CLOCK)] && transform != nullptr)
	{
		transform->setRotation(transform->getRotation() - _rotationalVelocity);
	}

	if (inputs[int(Inputs::LEFT)] && movement != nullptr)
	{
		// if (movement->getVelocityX() > -_velocity)
		// {
		// 	movement->setVelocityX(movement->getVelocityX() - _velocity);
		// }
		if (movement->getVelocityX() > -_velocity)
		{
			movement->setAccelerationX(movement->getAccelerationX() - 0.001);
		}

		if (movement->getVelocityX() < -0.25f)
		{
			movement->setAccelerationX(0);
		}
	}
	else if (inputs[int(Inputs::RIGHT)] && movement != nullptr)
	{
		// if (movement->getVelocityX() < _velocity)
		// {
		// 	movement->setVelocityX(movement->getVelocityX() + _velocity);
		// }
		if (movement->getVelocityX() < _velocity)
		{
			movement->setAccelerationX(movement->getAccelerationX() + 0.001);
		}

		if (movement->getVelocityX() > 0.25f)
		{
			movement->setAccelerationX(0);
		}
	}
	else
	{
		movement->setAccelerationX(0);
	}

	if (inputs[int(Inputs::UP)] && movement != nullptr)
	{
		// if (movement->getVelocityY() < _velocity)
		// {
		// 	movement->setVelocityY(movement->getVelocityY() + _velocity);
		// }
		if (movement->getVelocityY() < _velocity)
		{
			movement->setAccelerationY(movement->getAccelerationY() + 0.001);
		}

		if (movement->getVelocityY() > 0.25f)
		{
			movement->setAccelerationY(0);
		}
	}
	else if (inputs[int(Inputs::DOWN)] && movement != nullptr)
	{
		// if (movement->getVelocityY() > -_velocity)
		// {
		// 	movement->setVelocityY(movement->getVelocityY() - _velocity);
		// }
		if (movement->getVelocityY() > -_velocity)
		{
			movement->setAccelerationY(movement->getAccelerationY() - 0.001);
		}

		if (movement->getVelocityY() < -0.25f)
		{
			movement->setAccelerationY(0);
		}
	}
	else
	{
		movement->setAccelerationY(0);
	}

	_timeTillShoot -= deltaTime;
	_timeTillShoot = std::max(_timeTillShoot, 0);
	
	if (inputs[int(Inputs::SPACE)] && _timeTillShoot <= 0)
	{
		EventManager::getInstance()->raiseEvent(new ShootEvent(_gameObject));
		_timeTillShoot = 100;
	}
}
