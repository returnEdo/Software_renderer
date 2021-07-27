#pragma once

#include "vec3.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace Renderer
{


struct FullSampler: ISampler
{
	Buffer<Math::vec3>*	mNormal;
	Buffer<float>*		mDiffuse;
	Buffer<float>*		mShadow;
};


class FullProgram: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:
	
	virtual bool vertexShader(int i, IVertexInput tVertexInput);
	virtual Fragment fragmentShader(void);
};


}
