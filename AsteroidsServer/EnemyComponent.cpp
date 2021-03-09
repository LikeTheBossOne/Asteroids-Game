#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(GameObject* gameObject) : GenericComponent(gameObject)
{

}

EnemyComponent::EnemyComponent(const EnemyComponent& other, GameObject* gameObject) : GenericComponent(gameObject)
{
}

void EnemyComponent::update(int deltaTime)
{
}