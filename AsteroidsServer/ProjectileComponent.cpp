#include "ProjectileComponent.h"
#include "EventManager.h"
#include "DestroyEvent.h"

ProjectileComponent::ProjectileComponent(GameObject* gameObject, int timeToLive) : GenericComponent(gameObject)
{
	_timeToLive = timeToLive;
}

ProjectileComponent::ProjectileComponent(const ProjectileComponent& other, GameObject* gameObject) : GenericComponent(gameObject)
{
	_timeToLive = other._timeToLive;
}

void ProjectileComponent::update(int deltaTime)
{
	_timeToLive -= deltaTime;
	if (_timeToLive <= 0)
	{
		EventManager::getInstance()->raiseEvent(new DestroyEvent(_gameObject));
	}
}
