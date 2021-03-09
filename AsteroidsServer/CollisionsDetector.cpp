#include "CollisionsDetector.h"
#include "GameObject.h"
#include "RectangleColliderComponent.h"
#include "RigidBodyComponent.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include <iostream>
#include "JumpComponent.h"
#include "DieComponent.h"
#include "EventManager.h"
#include "CollisionEvent.h"
#include "Settings.h"

CollisionsDetector::CollisionsDetector() = default;

void CollisionsDetector::update(std::map<int, GameObject*> entities, int totalEntitiesToDate)
{
	const float COLLISION_ERROR = 0.001f;
	
	std::vector<bool> hits(totalEntitiesToDate * totalEntitiesToDate, false);
	for (const std::pair<int, GameObject*> entitiesPair : entities)
	{
		// if it ignores collisions skip it.
		if (entitiesPair.second->getRigidBody()->getIgnoreCollisions()) continue;
		
		for (const std::pair<int, GameObject*> oEntitiesPair : entities)
		{
			// if it ignores collisions skip it.
			if (oEntitiesPair.second->getRigidBody()->getIgnoreCollisions()) continue;

			if (entitiesPair.first == oEntitiesPair.first) continue;
			
			// Check if pair has already been collison-checked
			if (hits[oEntitiesPair.first * (totalEntitiesToDate-1) + entitiesPair.first]) continue;
			hits[entitiesPair.first * (totalEntitiesToDate-1) + oEntitiesPair.first] = true;


			auto object1 = entitiesPair.second;
			auto object2 = oEntitiesPair.second;


			auto rigidBody1 = object1->getRigidBody();
			auto rigidBody2 = object2->getRigidBody();

			if (rigidBody1->isStatic() && rigidBody2->isStatic()) continue; // both static


			const auto gCollider1 = object1->getComponent(ComponentTypes::RectangleColliderComponent);
			if (gCollider1 == nullptr) continue;
			auto collider1 = dynamic_cast<RectangleColliderComponent*>(gCollider1);
			auto bounds1a = collider1->getBounds();
			bool useSecondBounds1 = false;
			auto bounds1b = collider1->getBounds();
			if (bounds1b.getX() < 0)
			{
				bounds1b.setPosition(bounds1b.getX() + WINDOW_WIDTH, bounds1b.getY());
				useSecondBounds1 = true;
			}
			else if (bounds1b.getX() + bounds1b.getWidth() > WINDOW_WIDTH)
			{
				bounds1b.setPosition(bounds1b.getX() - WINDOW_WIDTH, bounds1b.getY());
				useSecondBounds1 = true;
			}
			if (bounds1b.getY() < 0)
			{
				bounds1b.setPosition(bounds1b.getX(), bounds1b.getY() + WINDOW_HEIGHT);
				useSecondBounds1 = true;
			}
			else if (bounds1b.getY() + bounds1b.getHeight() > WINDOW_HEIGHT)
			{
				bounds1b.setPosition(bounds1b.getX(), bounds1b.getY() - WINDOW_HEIGHT);
				useSecondBounds1 = true;
			}

			const auto gCollider2 = object2->getComponent(ComponentTypes::RectangleColliderComponent);
			if (gCollider2 == nullptr) continue;
			auto collider2 = dynamic_cast<RectangleColliderComponent*>(gCollider2);
			auto bounds2a = collider2->getBounds();
			bool useSecondBounds2 = false;
			auto bounds2b = collider2->getBounds();
			if (bounds2b.getX() < 0)
			{
				bounds2b.setPosition(bounds2b.getX() + WINDOW_WIDTH, bounds2b.getY());
				useSecondBounds2 = true;
			}
			else if (bounds2b.getX() + bounds2b.getWidth() > WINDOW_WIDTH)
			{
				bounds2b.setPosition(bounds2b.getX() - WINDOW_WIDTH, bounds2b.getY());
				useSecondBounds2 = true;
			}
			if (bounds2b.getY() < 0)
			{
				bounds2b.setPosition(bounds2b.getX(), bounds2b.getY() + WINDOW_HEIGHT);
				useSecondBounds2 = true;
			}
			else if (bounds2b.getY() + bounds2b.getHeight() > WINDOW_HEIGHT)
			{
				bounds2b.setPosition(bounds2b.getX(), bounds2b.getY() - WINDOW_HEIGHT);
				useSecondBounds2 = true;
			}

			// Check if collision
			EngineRectangle collisionRect;
			if (bounds1a.intersects(bounds2a, collisionRect))
			{
				auto collision = new CollisionEvent(object1, object2);
				EventManager::getInstance()->raiseEvent(collision);
			}
			else if (useSecondBounds1 && bounds1b.intersects(bounds2a, collisionRect)) {
				auto collision = new CollisionEvent(object1, object2);
				EventManager::getInstance()->raiseEvent(collision);
			}
			else if (useSecondBounds2 && bounds1a.intersects(bounds2b, collisionRect)) {
				auto collision = new CollisionEvent(object1, object2);
				EventManager::getInstance()->raiseEvent(collision);
			}
			else if (useSecondBounds1 && useSecondBounds2 && bounds1b.intersects(bounds2b, collisionRect)) {
				auto collision = new CollisionEvent(object1, object2);
				EventManager::getInstance()->raiseEvent(collision);
			}
		}
	}
}
