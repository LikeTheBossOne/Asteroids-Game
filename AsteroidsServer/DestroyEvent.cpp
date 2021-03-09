#include "DestroyEvent.h"
#include "GameObject.h"

DestroyEvent::DestroyEvent(GameObject* obj) : Event(0, 0)
{
	_object = obj;
}
