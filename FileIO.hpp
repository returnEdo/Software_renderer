#pragma once

#include <string>
#include <vector>

#include "Components.hpp"
#include "Entity.hpp"
#include "vec3.hpp"


namespace Renderer
{

namespace IO
{

namespace OBJ
{

	bool read(const std::string& tAddress, ecs::Entity tMeshId);

}

namespace PNG
{

	bool read(const std::string& tAddress, ecs::Entity tTextureId);

}

namespace PPM
{
	bool write(const std::string& tAddress, const std::vector<Math::vec3>& tColors, unsigned int tWidth, unsigned int tHeight);
}


}

}
