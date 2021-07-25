#pragma once

#include <string>
#include <vector>

#include "vec3.hpp"
#include "Components.hpp"
#include "Functor.hpp"


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

	bool read(const std::string& tAddress, Buffer<Math::vec3>& tVecBuffer, Buffer<float>& tFloatBuffer);

}

namespace PPM
{
	bool write(const std::string& tAddress, const std::vector<Math::vec3>& tColors, int tWidth, int tHeight);
	bool write(const std::string& tAddress, const Buffer<Math::vec3>& tBuffer);
	bool write(const std::string& tAddress, const Buffer<float>& tBuffer, IFunctor* tFunctor);
}


}

}
