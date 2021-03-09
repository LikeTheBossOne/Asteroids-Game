#define _USE_MATH_DEFINES

#include "EntityManager.h"
#include "Game.h"
#include "GameObject.h"
#include "PhysicsEngineSettings.h"
#include "GenericComponent.h"
#include "RigidBodyComponent.h"
#include "CollisionsDetector.h"
#include "MovementSystem.h"
#include "LifeSystem.h"
#include "SpawnSystem.h"
#include "TransformComponent.h"
#include <iostream>
#include "SpawnZone.h"
#include "EventManager.h"
#include "StartReplayEvent.h"
#include "Settings.h"
#include "CreateAsteroidsEvent.h"
#include <cmath>
#include "Asteroid.h"
#include "SplitEvent.h"
#include "SplitComponent.h"
#include "DestroyEvent.h"
#include "ShootEvent.h"
#include "Laser.h"

EntityManager::EntityManager(Game* game)
{
	_game = game;

	_collisionsManager = new CollisionsDetector();
	_movementSystem = new MovementSystem();
	_lifeSystem = new LifeSystem();
	_spawnSystem = new SpawnSystem();
	
	// Initialize Physics Engine
	_physicsEngineSettings = new PhysicsEngineSettings();
	_physicsEngineSettings->setGravityX(0);
	_physicsEngineSettings->setGravityY(0);
	_physicsEngineSettings->setMaxSpeed(100);

	_entities = std::map<int, GameObject*>();
	_totalEntitiesToDate = 0;

	_spawns = std::vector<SpawnZone*>();

	EventManager::getInstance()->registerFor(this, "START_REP");
	EventManager::getInstance()->registerFor(this, "CreateAsteroids");
	EventManager::getInstance()->registerFor(this, "Split");
	EventManager::getInstance()->registerFor(this, "Destroy");
	EventManager::getInstance()->registerFor(this, "Shoot");
}

void EntityManager::addEntity(GameObject* entity)
{
	_entitiesLock.lock();
	
	_entities.emplace(entity->getGUID(), entity);

	_totalEntitiesToDate++;

	_entitiesLock.unlock();
}

void EntityManager::deleteEntity(int GUID)
{
	_entitiesLock.lock();
	
	_entities.erase(GUID);

	_entitiesLock.unlock();
}

void EntityManager::addSpawn(SpawnZone* spawn)
{
	_spawns.emplace_back(spawn);
}

std::map<int, GameObject*> EntityManager::getEntities()
{
	std::lock_guard<std::mutex> lg(_entitiesLock);
	return _entities;
}

/**
 * Update Entities then Player
 */
void EntityManager::update(int deltaTime)
{
	// Grab current entities so nothing changes (multithreading) during the update process.
	_entitiesLock.lock();
	auto currentEntities = _entities;
	auto totalEntities = _totalEntitiesToDate;
	_entitiesLock.unlock();

	// Decide whether to create asteroids.
	bool createAsteroids = true;
	for (const std::pair<int, GameObject*> entity : currentEntities)
	{
		if (entity.second->getType() == "Asteroid") createAsteroids = false;
	}
	if (createAsteroids)
	{
		EventManager::getInstance()->raiseEvent(new CreateAsteroidsEvent(4));
	}

	for (const std::pair<int, GameObject*> entity : currentEntities)
	{
		auto object = entity.second;
		
		// Update prevX and prevY
		auto gComponent = object->getComponent(ComponentTypes::PlayerShipInputComponent);
		if (gComponent != nullptr)
		{
			gComponent->update(deltaTime);
		}
	}
	
	// Update all entities that can't stick to a surface
	for (const std::pair<int, GameObject*> entity : currentEntities)
	{
		auto object = entity.second;
		if (!object->getRigidBody()->getCanStickToSurface())
		{
			// Update prevX and prevY
			TransformComponent* transform = object->getTransform();
			transform->update(deltaTime);
			
			// Let RigidBodyComponent set MovementComponent (Gravity)
			RigidBodyComponent* rigidBody = object->getRigidBody();
			rigidBody->update(deltaTime);

			// Let JumpComponent set MovementComponent
			auto gComponent = object->getComponent(ComponentTypes::JumpComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}
			
			// Let PatternComponent set MovementComponent
			gComponent = object->getComponent(ComponentTypes::PatternComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Let MovementComponent set TransformComponent
			gComponent = object->getComponent(ComponentTypes::MovementComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Do Collider last because it is based on delta between prev transform and current
			gComponent = object->getComponent(ComponentTypes::RectangleColliderComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Then do Projectile
			gComponent = object->getComponent(ComponentTypes::ProjectileComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}
		}
	}

	
	// Update Movement based on surface stick if entity can stick to surface
	for (const std::pair<int, GameObject*> entity : currentEntities)
	{
		auto object = entity.second;
		if (object->getRigidBody()->getCanStickToSurface())
		{
			// Update prevX and prevY
			TransformComponent* transform = object->getTransform();
			transform->update(deltaTime);

			// Let RigidBodyComponent set MovementComponent (Gravity)
			RigidBodyComponent* rigidBody = object->getRigidBody();
			rigidBody->update(deltaTime);
		}
	}
	
	// Update Position based on Movement for entities that can stick to surface
	for (const std::pair<int, GameObject*> entity : currentEntities)
	{
		auto object = entity.second;
		if (object->getRigidBody()->getCanStickToSurface())
		{
			// Let JumpComponent set MovementComponent
			auto gComponent = object->getComponent(ComponentTypes::JumpComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}
			
			// Let PatternComponent set MovementComponent
			gComponent = object->getComponent(ComponentTypes::PatternComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Let MovementComponent set TransformComponent
			gComponent = object->getComponent(ComponentTypes::MovementComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Do Collider last because it is based on delta between prev transform and current
			gComponent = object->getComponent(ComponentTypes::RectangleColliderComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}

			// Then do Projectile
			gComponent = object->getComponent(ComponentTypes::ProjectileComponent);
			if (gComponent != nullptr)
			{
				gComponent->update(deltaTime);
			}
		}
	}

	// Do physics/collisions
	_collisionsManager->update(currentEntities, totalEntities);

	// Update Textures
}

void EntityManager::onEvent(Event* e)
{
	if (e->getType() == "START_REP")
	{
		handleStartReplay(static_cast<StartReplayEvent*>(e));
	}
	else if (e->getType() == "CreateAsteroids")
	{
		handleCreateAsteroids(static_cast<CreateAsteroidsEvent*>(e));
	}
	else if (e->getType() == "Split")
	{
		handleSplit(static_cast<SplitEvent*>(e));
	}
	else if (e->getType() == "Destroy")
	{
		handleDestroy(static_cast<DestroyEvent*>(e));
	}
	else if (e->getType() == "Shoot")
	{
		handleShoot(static_cast<ShootEvent*>(e));
	}
}

void EntityManager::handleStartReplay(StartReplayEvent* e)
{
	overwriteEntities(e->entities);
}

void EntityManager::handleCreateAsteroids(CreateAsteroidsEvent* e)
{
	// Destroy all old Asteroids
	auto entities = _entities;
	for (auto ePair : entities)
	{
		if (ePair.second->getType() == "Asteroid")
		{
			deleteEntity(ePair.first);
		}
	}
	

	// Create all new asteroids
	std::vector<std::pair<float, float>> possibleLocs;
	possibleLocs.emplace_back(0, 0);
	possibleLocs.emplace_back(0, WINDOW_HEIGHT);
	possibleLocs.emplace_back(WINDOW_WIDTH, 0);
	possibleLocs.emplace_back(WINDOW_WIDTH, WINDOW_HEIGHT);

	for (int i = 0; i < e->getAmount(); i++)
	{
		auto loc = possibleLocs[rand() % possibleLocs.size()];
		auto position = Rect(loc.first, loc.second, 64, 64);
		
		auto phi = 2 * M_PI * (rand() / float(RAND_MAX));
		auto vx = 0.15 * cos(phi);
		auto vy = 0.15 * sin(phi);
		addEntity(new Asteroid(_physicsEngineSettings, position, "asteroid", vx, vy, 2));
	}
}

void EntityManager::handleSplit(SplitEvent* e)
{
	GameObject* obj = e->getObject();
	auto split = static_cast<SplitComponent*>(obj->getComponent(ComponentTypes::SplitComponent));

	Rect position = obj->getTransform()->getPosition();
	position.setWidth(position.getWidth() / 2);
	position.setHeight(position.getHeight() / 2);
	position.setX(position.getX() + position.getWidth() / 2);
	position.setY(position.getY() + position.getHeight() / 2);

	
	const int amount = split->getSplitAmount();
	const int futureSplitsLeft = split->getSplitsLeft() - 1;
	
	float velocityMultiplier = 1;
	switch (futureSplitsLeft)
	{
	case 0:
		velocityMultiplier = 2;
		break;
	case 1:
		velocityMultiplier = 1.5;
		break;
	default:
		velocityMultiplier = 1;
		break;
	}
	
	for (int i = 0; i < amount; i++)
	{
		if (obj->getType() == "Asteroid")
		{
			auto phi = 2 * M_PI * (rand() / float(RAND_MAX));
			auto vx = velocityMultiplier * 0.1 * cos(phi);
			auto vy = velocityMultiplier * 0.1 * sin(phi);
			addEntity(new Asteroid
			(
				_physicsEngineSettings,
				position,
				"asteroid",
				vx,
				vy,
				futureSplitsLeft
			));
		}
	}

	// Delete previous entity
	deleteEntity(obj->getGUID());
}

void EntityManager::handleDestroy(DestroyEvent* e)
{
	deleteEntity(e->getObject()->getGUID());
}

void EntityManager::handleShoot(ShootEvent* e)
{
	auto sendingObject = e->getSendingObject();
	auto position = sendingObject->getTransform()->getPosition();

	position.setX(position.getX() + position.getWidth() / 2);
	position.setY(position.getY() + position.getHeight() / 2);
	position.setWidth(4);
	position.setHeight(4);
	auto vx = 0.6 * sin(sendingObject->getTransform()->getRotation() * M_PI / 180);
	auto vy = 0.6 * cos(sendingObject->getTransform()->getRotation() * M_PI / 180);
	
	addEntity(new Laser(_physicsEngineSettings, position, vx, vy));
}
