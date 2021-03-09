#include "FriendlyComponent.h"

FriendlyComponent::FriendlyComponent(GameObject* gameObject) : GenericComponent(gameObject)
{
	
}

FriendlyComponent::FriendlyComponent(const FriendlyComponent& other, GameObject* gameObject) : GenericComponent(gameObject)
{
}

void FriendlyComponent::update(int deltaTime)
{
}
