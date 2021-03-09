#include "SplitComponent.h"

SplitComponent::SplitComponent(GameObject* gameObject, int splitsLeft, int splitAmount) : GenericComponent(gameObject)
{
	_splitsLeft = splitsLeft;
	_splitAmount = splitAmount;
}

SplitComponent::SplitComponent(const SplitComponent& other, GameObject* gameObject) : GenericComponent(gameObject)
{
	_splitsLeft = other._splitsLeft;
	_splitAmount = other._splitAmount;
}

void SplitComponent::update(int deltaTime)
{
}
