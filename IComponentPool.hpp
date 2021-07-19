#pragma once

#include "Entity.hpp"


namespace ecs
{

class IComponentPool
{
	public:

	virtual ~IComponentPool(void) {};
	virtual void removeComponent(Entity tEntity) {};
};

}
