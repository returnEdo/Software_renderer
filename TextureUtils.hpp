#pragma once

#include <string>
#include <vector>

#include "vec2.hpp"
#include "vec3.hpp"
#include "Components.hpp"


namespace Renderer
{


TextureSample sampleTexture(const Math::vec2& tTextureCoordinates, Texture& tTexture);



}
