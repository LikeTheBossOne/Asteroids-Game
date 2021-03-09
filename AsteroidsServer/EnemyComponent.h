#pragma once
#include "GenericComponent.h"
class EnemyComponent : public GenericComponent
{
public:
	EnemyComponent(GameObject* gameObject);
	EnemyComponent(const EnemyComponent& other, GameObject* gameObject);

	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::EnemyComponent; }
};

