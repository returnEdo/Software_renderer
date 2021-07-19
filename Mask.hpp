#pragma once

#include <cstdint>


namespace ecs
{

using Mask = uint32_t;


void reset(Mask& tMask);

void set(Mask& tMask, uint32_t tBit);

void remove(Mask& tMask, uint32_t tBit);

bool hasComponent(Mask tMask, Mask tComponent);


}
