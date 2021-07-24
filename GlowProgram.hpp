#pragma once

#include "vec3.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace Renderer
{


class GlowProgram: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:
	
	virtual void vertexShader(int i, IVertexInput tVertexInput);
	virtual Fragment fragmentShader(void);
};


}
