#pragma once
#include "GenericComponent.h"

class ProjectileComponent : public GenericComponent
{
public:
	ProjectileComponent(GameObject* gameObject, int timeToLive);
	ProjectileComponent(const ProjectileComponent& other, GameObject* gameObject);

	void update(int deltaTime) override;
	ComponentTypes getType() override { return ComponentTypes::ProjectileComponent; }
private:
	int _timeToLive;
};

