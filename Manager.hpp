// One Manager to rule em all
#pragma once

#include "Unique.hpp"
#include "Mask.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "ISystem.hpp"


namespace ecs
{

class Manager
{
	private:
	
	Memory::Unique<EntityManager>		mEntityManager;
	Memory::Unique<ComponentManager>	mComponentManager;

	public:

	void init(void)
	{
		mEntityManager.allocate();
		mComponentManager.allocate();
	}

	void destroy(void)
	{
		std::set<Entity> lActiveEntities;

		for (uint32_t i = 0; i < C_MAX_ENTITIES; i++)
		{
			lActiveEntities.insert(i);
		}
		// Builds set of active entities
		while (not (mEntityManager -> mAvailable).empty()) 
		{
			lActiveEntities.erase((mEntityManager -> mAvailable).front());
			(mEntityManager -> mAvailable).pop();
		}
		// Explicitly destroys all the components
		for (Entity lEntity: lActiveEntities)
		{
			removeEntity(lEntity);
		}
	}

	Entity createEntity(void)
	{
		return (mEntityManager -> createEntity());
	}

	void removeEntity(Entity tEntity)
	{
		Mask lEntityMask = mEntityManager -> getMask(tEntity);	

		for (uint32_t i = 0; i < C_MAX_COMPONENTS; i++)
		{
			Mask lComponentMask = (1 << i);
			
			if ((lEntityMask & lComponentMask) == lComponentMask)
			{
				mComponentManager -> removeComponentByIndex(tEntity, i);
			}
		}
		mEntityManager -> removeEntity(tEntity);
	}

	template <typename T>
	Mask getComponentID(void)
	{
		return 1 << (mComponentManager -> getComponentId<T>());
	}

	template <typename T>
	T& addComponent(Entity tEntity, T tComponent)
	{
		// Get component id and entity mask
		Mask lComponentId = getComponentID<T>();
		Mask& lEntityMask = mEntityManager -> getMask(tEntity);

		// update entity mask
		lEntityMask += lComponentId;

		// add the component
		return *(mComponentManager -> addComponent<T>(tEntity, tComponent));
	}

	template <typename T>
	void removeComponent(Entity tEntity)
	{
		// Get component id and entity mask
		Mask lComponentId = getComponentID<T>();
		Mask& lEntityMask = mEntityManager -> getMask(tEntity);

		// Remove the component id
		lEntityMask -= lComponentId;

		mComponentManager -> removeComponent<T>(tEntity);
	}

	template <typename T>
	T& getComponent(Entity tEntity)
	{
		return (mComponentManager -> getComponent<T>(tEntity));
	}

	template <class ... Types>
	void registerSystem(ISystem& tSystem)
	{	
		// Gets system mask
		Mask& lSystemMask = tSystem.getMask();	
		reset(lSystemMask);
		
		// Populates system mask
		lSystemMask += getComponentID<Types ...>();
		
		updateEntityList(tSystem);
	}

	void sameMaskAs(ISystem& tSystem, Entity tEntity)
	{
		// Get and set masks
		Mask& lSystemMask = tSystem.getMask();
		reset(lSystemMask);

		Mask lEntityMask = mEntityManager -> getMask(tEntity);
		
		lSystemMask = lEntityMask;

		updateEntityList(tSystem);	
	}

	void updateEntityList(ISystem& tSystem)
	{
		// Update after entity removal
		Mask lSystemMask = tSystem.getMask();

		std::vector<Entity>& lSystemEntities = tSystem.getEntities();
		
		// Reset
		lSystemEntities.erase(lSystemEntities.begin(), lSystemEntities.end());

		for (uint32_t i = 0; i < (mEntityManager -> mComponentMask).size(); i++)
		{
			if ((lSystemMask & (mEntityManager -> mComponentMask)[i]) == lSystemMask)
			{
				lSystemEntities.push_back(i);
			}
		}
	}
};



}


