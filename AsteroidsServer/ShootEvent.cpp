#include "ShootEvent.h"
#include "GameObject.h"

ShootEvent::ShootEvent(GameObject* sendingObject) : Event(0, 0)
{
	_sendingObject = sendingObject;
}
