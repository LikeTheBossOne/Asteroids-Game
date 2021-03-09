#include "Laser.h"
#include "TransformComponent.h"

Laser::Laser(int GUID, sf::FloatRect location) : GameObject(GUID)
{
	_transform = new TransformComponent(this, location, true);

	_sfRectangleShape = new sf::RectangleShape(sf::Vector2f(location.width, location.height));
	_sfRectangleShape->setPosition(location.left, location.top);
}
