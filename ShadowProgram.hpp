#pragma once

#include "vec3.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace Renderer
{


struct ShadowSampler: ISampler
{
	Buffer<float>*	mLightDepth;
};


class ShadowProgram: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:
	
	virtual void vertexShader(int i, IVertexInput tVertexInput);
	virtual Fragment fragmentShader(void);
};


}
