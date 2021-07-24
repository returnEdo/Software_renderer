#pragma once

#include <vector>

#include "Components.hpp"


namespace Renderer
{


struct LinearFilter
{
	int			mSigmaX;
	int			mSigmaY;
	
	std::vector<float>	mWeights;	// row first
};

std::vector<float> initGaussian(int tSigmaWidth, float tSigmaGauss);
void filter(const LinearFilter& tFilter, const Texture& tTexture, Texture& tTarget);
void filterGaussian(float tSigma, const Texture& tTexture, Texture& tTarget);


}
