#pragma once

#include <set>
#include <cstdint>

#include "IComponentPool.hpp"
#include "Entity.hpp"
#include "ECSConstants.hpp"


namespace ecs
{


template <typename T>
class ComponentPool: public IComponentPool
{
	private:

	size_t 	mCount;
	T*	mHead { nullptr };


	public:

	ComponentPool(size_t tCount = C_MAX_ENTITIES): mCount(tCount)
	{
		// Allocate some slots
		mHead = (T*)(operator new(mCount * sizeof(T)));
	}


	virtual ~ComponentPool(void)
	{
		// Releases the memory
		operator delete(mHead);	
	}


	T* getComponent(Entity tEntity)
	{
		return (mHead + tEntity);
	}


	T* addComponent(Entity tEntity, T tComponent)
	{
		// Instantiate the object
		return new(mHead + tEntity) T(tComponent);
	}


	virtual void removeComponent(Entity tEntity)
	{
		// Destroys the object
		(mHead + tEntity) -> ~T();
	}
};




}
