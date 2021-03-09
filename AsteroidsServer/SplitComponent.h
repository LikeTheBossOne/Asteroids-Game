#pragma once
#include "GenericComponent.h"

class SplitComponent : public GenericComponent
{
public:
	SplitComponent(GameObject* gameObject, int splitsLeft, int splitAmount);
	SplitComponent(const SplitComponent& other, GameObject* gameObject);

	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::SplitComponent; }

	int getSplitsLeft() { return _splitsLeft; }
	int getSplitAmount() { return _splitAmount; }
private:

	int _splitsLeft;
	int _splitAmount;
};

