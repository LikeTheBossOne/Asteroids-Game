#include "Laser.h"
#include "KillComponent.h"
#include "MovementComponent.h"
#include "ColorComponent.h"
#include "RectangleColliderComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "FriendlyComponent.h"
#include "ProjectileComponent.h"

Laser::Laser(PhysicsEngineSettings* physSettings, Rect position, float velocityX, float velocityY)
{
	_transform = new TransformComponent(this, position, true);

	_rigidBody = new RigidBodyComponent(this, physSettings, false, false, false, false, false, false, false);

	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		this, 0, 0, position.getWidth(), position.getHeight());

	_components[ComponentTypes::ColorComponent] = new ColorComponent(this, 255, 0, 0);

	auto movement = new MovementComponent(this);
	movement->setVelocityX(velocityX);
	movement->setVelocityY(velocityY);
	_components[ComponentTypes::MovementComponent] = movement;

	_components[ComponentTypes::KillComponent] = new KillComponent(this);

	_components[ComponentTypes::FriendlyComponent] = new FriendlyComponent(this);

	_components[ComponentTypes::ProjectileComponent] = new ProjectileComponent(this, 800);
}

Laser::Laser(const Laser& other, PhysicsEngineSettings* physSettings)
{
	_transform = new TransformComponent(*other._transform, this);
	_rigidBody = new RigidBodyComponent(*other._rigidBody, this, physSettings);

	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		*static_cast<RectangleColliderComponent*>(other._components.at(ComponentTypes::RectangleColliderComponent)), this
	);

	_components[ComponentTypes::ColorComponent] = new ColorComponent(
		*static_cast<ColorComponent*>(other._components.at(ComponentTypes::ColorComponent)), this
	);

	_components[ComponentTypes::MovementComponent] = new MovementComponent(
		*static_cast<MovementComponent*>(other._components.at(ComponentTypes::MovementComponent)), this
	);

	_components[ComponentTypes::KillComponent] = new KillComponent(
		*static_cast<KillComponent*>(other._components.at(ComponentTypes::KillComponent)), this
	);

	_components[ComponentTypes::FriendlyComponent] = new FriendlyComponent(
		*static_cast<FriendlyComponent*>(other._components.at(ComponentTypes::FriendlyComponent)), this
	);

	_components[ComponentTypes::ProjectileComponent] = new ProjectileComponent(
		*static_cast<ProjectileComponent*>(other._components.at(ComponentTypes::ProjectileComponent)), this
	);
}
