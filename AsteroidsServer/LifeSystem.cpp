#include "LifeSystem.h"
#include "EventManager.h"
#include "CollisionEvent.h"
#include "ComponentTypes.h"
#include "GameObject.h"
#include "KillComponent.h"
#include "DieComponent.h"
#include "MovementComponent.h"
#include "DeathEvent.h"
#include "ScriptManager.h"
#include <dukglue/dukglue.h>
#include "CreateAsteroidsEvent.h"
#include "SplitComponent.h"
#include "SplitEvent.h"

bool LifeSystem::scriptInitialized = false;

LifeSystem::LifeSystem()
{
	if (!scriptInitialized)
	{
		// duk_context* ctx = ScriptManager::getInstance()->getContext();
		// dukglue_register_property<DeathEvent, GameObject*>(ctx, &DeathEvent::getDyingObject, &DeathEvent::setDyingObject, "dyingObject");
		
		scriptInitialized = true;
	}
	EventManager::getInstance()->registerFor(this, "Col");
}

void LifeSystem::onEvent(Event* e)
{
	if (e->getType() == "Col")
	{
		handleCollision(static_cast<CollisionEvent*>(e));
	}
}

void LifeSystem::handleCollision(CollisionEvent* e)
{
	auto object1 = e->first;
	auto object2 = e->second;

	if (!((object1->getComponent(ComponentTypes::FriendlyComponent) != nullptr &&
		object2->getComponent(ComponentTypes::EnemyComponent) != nullptr) ||
		(object2->getComponent(ComponentTypes::FriendlyComponent) != nullptr &&
		object1->getComponent(ComponentTypes::EnemyComponent) != nullptr)))
	{
		return;
	}
	
	// First check if Killer Object
	const auto gKill1 = object1->getComponent(ComponentTypes::KillComponent);
	const auto gKill2 = object2->getComponent(ComponentTypes::KillComponent);
	const auto gDie1 = object1->getComponent(ComponentTypes::DieComponent);
	const auto gDie2 = object2->getComponent(ComponentTypes::DieComponent);
	
	if (gKill1 != nullptr && gDie2 != nullptr)
	{
		auto death = new DeathEvent(object2);
		EventManager::getInstance()->raiseEvent(death);

		// respawn
		if (object2->getType() == "PlayerShip")
		{
			EventManager::getInstance()->raiseEvent(new CreateAsteroidsEvent(4));
		}
	}
	if (gKill2 != nullptr && gDie1 != nullptr)
	{
		auto death = new DeathEvent(object1);
		EventManager::getInstance()->raiseEvent(death);

		// respawn
		if (object1->getType() == "PlayerShip")
		{
			EventManager::getInstance()->raiseEvent(new CreateAsteroidsEvent(4));
		}
	}

	const auto gSplit1 = object1->getComponent(ComponentTypes::SplitComponent);
	const auto gSplit2 = object2->getComponent(ComponentTypes::SplitComponent);
	if (gKill1 != nullptr && gSplit2 != nullptr)
	{
		EventManager::getInstance()->raiseEvent(new DeathEvent(object1));
		if (static_cast<SplitComponent*>(gSplit2)->getSplitsLeft() == 0)
		{
			EventManager::getInstance()->raiseEvent(new DeathEvent(object2));
		}
		else
		{
			EventManager::getInstance()->raiseEvent(new SplitEvent(object2));
		}
	}
	if (gKill2 != nullptr && gSplit1 != nullptr)
	{
		EventManager::getInstance()->raiseEvent(new DeathEvent(object2));
		if (static_cast<SplitComponent*>(gSplit1)->getSplitsLeft() == 0)
		{
			EventManager::getInstance()->raiseEvent(new DeathEvent(object1));
		}
		else
		{
			EventManager::getInstance()->raiseEvent(new SplitEvent(object1));
		}
	}
}
