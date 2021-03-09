#pragma once
#include "GenericComponent.h"
class FriendlyComponent : public GenericComponent
{
public:
	FriendlyComponent(GameObject* gameObject);
	FriendlyComponent(const FriendlyComponent& other, GameObject* gameObject);
	
	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::FriendlyComponent; }
};

