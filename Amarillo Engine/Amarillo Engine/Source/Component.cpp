#include "Component.h"

Component::Component()
{

}

Component::~Component()
{
}

void Component::SetActive(bool active)
{
	if (this->active != active)
	{
		this->active = active;
	}
}
