#include "Asteroid.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "RectangleColliderComponent.h"
#include "TextureComponent.h"
#include "MovementComponent.h"
#include "KillComponent.h"
#include "SplitComponent.h"
#include "EnemyComponent.h"

Asteroid::Asteroid(PhysicsEngineSettings* physSettings, Rect position, std::string textureName, float velocityX, float velocityY, int splitsLeft)
{
	_transform = new TransformComponent(this, position, true);

	_rigidBody = new RigidBodyComponent(this, physSettings, false, false, false, false, false, false, false);

	_components[ComponentTypes::EnemyComponent] = new EnemyComponent(this);
	
	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		this, 0, 0, position.getWidth(), position.getHeight());

	_components[ComponentTypes::TextureComponent] = new TextureComponent(this, textureName);

	auto movement = new MovementComponent(this);
	movement->setVelocityX(velocityX);
	movement->setVelocityY(velocityY);
	_components[ComponentTypes::MovementComponent] = movement;

	_components[ComponentTypes::KillComponent] = new KillComponent(this);

	_components[ComponentTypes::SplitComponent] = new SplitComponent(this, splitsLeft, 2);
}

Asteroid::Asteroid(const Asteroid& other, PhysicsEngineSettings* physSettings)
{
	_transform = new TransformComponent(*other._transform, this);
	_rigidBody = new RigidBodyComponent(*other._rigidBody, this, physSettings);

	_components[ComponentTypes::EnemyComponent] = new EnemyComponent(
		*static_cast<EnemyComponent*>(other._components.at(ComponentTypes::EnemyComponent)), this
	);
	
	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		*static_cast<RectangleColliderComponent*>(other._components.at(ComponentTypes::RectangleColliderComponent)), this
	);

	_components[ComponentTypes::TextureComponent] = new TextureComponent(
		*static_cast<TextureComponent*>(other._components.at(ComponentTypes::TextureComponent)), this
	);

	_components[ComponentTypes::MovementComponent] = new MovementComponent(
		*static_cast<MovementComponent*>(other._components.at(ComponentTypes::MovementComponent)), this
	);

	_components[ComponentTypes::KillComponent] = new KillComponent(
		*static_cast<KillComponent*>(other._components.at(ComponentTypes::KillComponent)), this
	);

	_components[ComponentTypes::SplitComponent] = new SplitComponent(
		*static_cast<SplitComponent*>(other._components.at(ComponentTypes::SplitComponent)), this
	);
}
