#include "Mask.hpp"


namespace ecs
{


void reset(Mask& tMask)	{ tMask = 0; }

void set(Mask& tMask, uint32_t tBit)
{
	tMask += (1 << tBit);
}

void remove(Mask& tMask, uint32_t tBit)
{
	tMask -= (1 << tBit);
}

bool hasComponent(Mask tMask, Mask tComponent)
{
	return (tMask & tComponent) == tComponent;
}


}
