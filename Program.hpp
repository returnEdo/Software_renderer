#pragma once

#include "vec3.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace Renderer
{


struct NormalSampler: ISampler
{
	Buffer<Math::vec3>*	mNormal;
};


class Program: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:
	
	virtual bool vertexShader(int i, IVertexInput tVertexInput);
	virtual Fragment fragmentShader(void);
};


}
