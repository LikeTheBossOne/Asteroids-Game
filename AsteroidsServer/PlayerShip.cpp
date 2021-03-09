#include "PlayerShip.h"
#include "PlayerShipInputComponent.h"
#include "MovementComponent.h"
#include "RectangleColliderComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpawnZone.h"
#include "TextureComponent.h"
#include "DieComponent.h"
#include "SpawnableComponent.h"
#include "FriendlyComponent.h"

PlayerShip::PlayerShip(PhysicsEngineSettings* physSettings, float width, float height, std::string textureName, SpawnZone* spawn)
{
	_transform = new TransformComponent(this,
		Rect(spawn->getTransform()->getPositionX(), spawn->getTransform()->getPositionX(), width, height), true);

	_rigidBody = new RigidBodyComponent(this, physSettings, false, false, false, false, false, false, false);

	_components[ComponentTypes::FriendlyComponent] = new FriendlyComponent(this);
	
	// Initialize Collider
	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		this, 0, 0, width, height);

	// Initialize Movement
	_components[ComponentTypes::MovementComponent] = new MovementComponent(this);

	// Initialize PlayerShipInput
	_components[ComponentTypes::PlayerShipInputComponent] = new PlayerShipInputComponent(this, 3, 0.2);

	// Initialize Texture
	_components[ComponentTypes::TextureComponent] = new TextureComponent(this, textureName);

	// Initialize Die Component
	_components[ComponentTypes::DieComponent] = new DieComponent(this, true);

	// Initialize Spawnable Component
	_components[ComponentTypes::SpawnableComponent] = new SpawnableComponent(this, spawn);
}

PlayerShip::PlayerShip(const PlayerShip& other, PhysicsEngineSettings* physSettings, SpawnZone* spawn)
{
	_transform = new TransformComponent(*other._transform, this);

	_rigidBody = new RigidBodyComponent(*other._rigidBody, this, physSettings);

	_components[ComponentTypes::FriendlyComponent] = new FriendlyComponent(
		*static_cast<FriendlyComponent*>(other._components.at(ComponentTypes::FriendlyComponent)), this
	);

	_components[ComponentTypes::RectangleColliderComponent] = new RectangleColliderComponent(
		*static_cast<RectangleColliderComponent*>(other._components.at(ComponentTypes::RectangleColliderComponent)), this
	);

	_components[ComponentTypes::MovementComponent] = new MovementComponent(
		*static_cast<MovementComponent*>(other._components.at(ComponentTypes::MovementComponent)), this
	);

	_components[ComponentTypes::PlayerInputComponent] = new PlayerShipInputComponent(
		*static_cast<PlayerShipInputComponent*>(other._components.at(ComponentTypes::PlayerShipInputComponent)), this
	);

	_components[ComponentTypes::TextureComponent] = new TextureComponent(
		*static_cast<TextureComponent*>(other._components.at(ComponentTypes::TextureComponent)), this
	);

	_components[ComponentTypes::DieComponent] = new DieComponent(
		*static_cast<DieComponent*>(other._components.at(ComponentTypes::DieComponent)), this
	);

	_components[ComponentTypes::SpawnableComponent] = new SpawnableComponent(
		*static_cast<SpawnableComponent*>(other._components.at(ComponentTypes::SpawnableComponent)), this, spawn
	);
}
