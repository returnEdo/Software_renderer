#pragma once

#include <vector>

#include "Mask.hpp"
#include "Entity.hpp"


namespace ecs
{


class ISystem
{
	protected:

	std::vector<Entity> 	mEntities;
	Mask			mComponentMask { 0 };

	public:

	virtual ~ISystem(void) {}

	Mask& getMask(void)
	{
		return mComponentMask;
	}

	std::vector<Entity>& getEntities(void)
	{
		return mEntities;
	}
};


}
