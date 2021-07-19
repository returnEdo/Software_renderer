#pragma once

#include <queue>
#include <set>
#include <iostream>


namespace Memory
{


template <typename T>
class PoolAllocator
{
	private:
	
	size_t 	mCount;		// Number of objects

	T* 	mHead { nullptr }; 

	std::queue<int> mIndexAvailable;

	public:

	PoolAllocator(size_t tCount):
		mCount(tCount)
	{
		mHead = (T*)(operator new(mCount * sizeof(T)));	

		for (int i = 0; i < mCount; i++)
		{
			mIndexAvailable.push(i);
		}
	}

	~PoolAllocator(void)
	{
		std::set<int> lFreeIndexes;

		while (not mIndexAvailable.empty())
		{
			lFreeIndexes.insert(mIndexAvailable.front());
			mIndexAvailable.pop();
		}

		for (int i = 0; i < mCount; i++)
		{
			if (lFreeIndexes.find(i) == lFreeIndexes.end())
			{
				free(mHead + i);
			}
		}

		operator delete(mHead);
	}

	template <class ... Types>
	T* allocate(Types ... tParams)
	{
		if (mIndexAvailable.empty())
		{
			return nullptr;
		}

		int lIndex = mIndexAvailable.front();
		mIndexAvailable.pop();

		return (new(mHead + lIndex) T(tParams ...));
	}

	void free(T* tMemory)
	{
		tMemory -> ~T();

		int lIndex = (tMemory - mHead);
		
		mIndexAvailable.push(lIndex);
	}
	
};


}
