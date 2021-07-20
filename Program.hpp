#pragma once

#include "vec3.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace Renderer
{


struct NormalSampler: ISampler
{
	Texture*	mNormal;
};


class Program: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:
	
	virtual void vertexShader(int i, IVertexInput tVertexInput);
	virtual Math::vec3 fragmentShader(void);
};


}
