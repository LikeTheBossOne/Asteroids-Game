#include "SplitEvent.h"
#include "GameObject.h"

SplitEvent::SplitEvent(GameObject* obj) : Event(2, 0)
{
	_object = obj;
}
