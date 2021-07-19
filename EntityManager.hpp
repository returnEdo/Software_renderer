#pragma once

#include <array>
#include <queue>

#include "Entity.hpp"
#include "Mask.hpp"
#include "Macros.hpp"
#include "ECSConstants.hpp"


namespace ecs
{


class EntityManager
{
	private:
	
	std::queue<uint32_t>			mAvailable;
	std::array<uint32_t, C_MAX_ENTITIES>	mComponentMask;

	public:

	EntityManager(void)
	{
		for (uint32_t i = 0; i < C_MAX_ENTITIES; i++)
		{
			mAvailable.push(i);
			reset(mComponentMask[i]);
		}
	}
	
	Entity createEntity(void)
	{
		R_ASSERT(not mAvailable.empty(), "Not enough enitites!!");

		uint32_t lIndex = mAvailable.front();

		mAvailable.pop();

		return lIndex;
	}

	void removeEntity(Entity tEntity)
	{	
		if (tEntity >= 0 and tEntity < C_MAX_ENTITIES)
		{
			mAvailable.push(tEntity);

			reset(mComponentMask[tEntity]);
		}
	}

	Mask& getMask(Entity tEntity)
	{
		return mComponentMask[tEntity];
	}

	friend class Manager;
};



};
