#pragma once

#include <array>
#include <cstdint>

#include "Unique.hpp"
#include "ECSConstants.hpp"
#include "Entity.hpp"
#include "IComponentPool.hpp"
#include "ComponentPool.hpp"


namespace ecs
{


class ComponentManager
{
	private:

	std::array<Memory::Unique<IComponentPool>, C_MAX_COMPONENTS>	mComponentPools;
	
	int mComponentCounter { 0 }; 

	public:

	template <typename T>
	uint32_t getComponentId(void)
	{
		static int sThisComponentCounter = 0;
		static int sThisComponentIndex;

		// First occurrence of this component
		if (not sThisComponentCounter)
		{
			sThisComponentIndex = mComponentCounter;
			mComponentCounter ++;
			
			// Allocate the pertinent pool
			mComponentPools[sThisComponentIndex].allocate<ComponentPool<T>>();
		}
		sThisComponentCounter ++;

		return sThisComponentIndex;
	}


	template <typename T>
	ComponentPool<T>* getComponentPool(void)
	{
		return (ComponentPool<T>*) mComponentPools[getComponentId<T>()].get();
	}

	template <typename T>
	T* addComponent(Entity tEntity, T tComponent)
	{
		return (getComponentPool<T>() -> addComponent(tEntity, tComponent));
	}

	template <typename T>
	void removeComponent(Entity tEntity)
	{
		getComponentPool<T>() -> removeComponent(tEntity);	
	}

	void removeComponentByIndex(Entity tEntity, uint32_t tIndex)
	{
		mComponentPools[tIndex] -> removeComponent(tEntity);
	}


	template <typename T>
	T& getComponent(Entity tEntity)
	{
		return *(getComponentPool<T>() -> getComponent(tEntity));	
	}
};


}
