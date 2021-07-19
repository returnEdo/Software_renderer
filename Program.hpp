#pragma once

#include "IProgram.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Components.hpp"

namespace Renderer
{

using VertexInput = VertexOutput;

struct Uniform: IUniform
{
	Math::vec3	mLightPosition;
	Math::mat3	mLightRotationT;
};


class Program: public IProgram
{
	private:

	virtual void updateVarying(void);

	public:

	virtual void vertex(int i, const VertexInput& tVertexInput);
	virtual Math::vec3 fragment(void);
};

}
