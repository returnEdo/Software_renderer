#pragma once

#include "Entity.hpp"
#include "vec3.hpp"

namespace Renderer
{

void lookAt(ecs::Entity tCameraId, const Math::vec3& tFocus);


}
