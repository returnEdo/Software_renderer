#pragma once

#include <string>
#include <vector>

#include "vec3.hpp"
#include "Components.hpp"


namespace Renderer
{

namespace IO
{

namespace OBJ
{

	bool read(const std::string& tAddress, Mesh& tMesh);

}

namespace PNG
{

	bool read(const std::string& tAddress, Texture& tTexture);

}

namespace PPM
{
	bool write(const std::string& tAddress, const std::vector<Math::vec3>& tColors, int tWidth, int tHeight);
}


}

}
