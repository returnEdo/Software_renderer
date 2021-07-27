#pragma once

#include "Components.hpp"

namespace Renderer
{

namespace AfterEffects
{

struct AOsettings
{
	int 	mSampleCount { 8 };
	float	mRadius	     { .05f };
};
	

void SSAO(const Buffer<float>& tDepthBuffer, Buffer<float>& tTarget,
	  const Camera& tCamera, const AOsettings& tSettings);
void applySSAO(const Buffer<float>& tAO, Buffer<Math::vec3>& tFrameBuffer);

}

}
