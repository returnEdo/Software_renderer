#pragma once

#include <string>
#include <vector>

#include "vec2.hpp"
#include "vec3.hpp"
#include "Entity.hpp"
#include "Components.hpp"


namespace Renderer
{


// origin bottom left (as in obj)
TextureSample sampleTexture(const Math::vec2& tTextureCoordinates, Texture& tTexture);



}
