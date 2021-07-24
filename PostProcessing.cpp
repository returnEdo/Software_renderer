#include "PostProcessing.hpp"

#include <cmath>

#include "vec3.hpp"
#include "MathUtils.hpp"
#include "Macros.hpp"

namespace Renderer
{

std::vector<float> initGaussian(int tSigmaWidth, float tSigmaGauss)
{
	std::vector<float> tWeights;

	for (int i = -tSigmaWidth; i <= tSigmaWidth; i++)
	{
		tWeights.push_back(std::exp(-std::pow(static_cast<float>(i), 2.0f) / (2.0f * std::pow(tSigmaGauss, 2.0f))));
	}
	return tWeights;
};

void filter(const LinearFilter& tFilter, const Texture& tTexture, Texture& tTarget)
{
	tTarget.mAlpha.erase(tTarget.mAlpha.begin(), tTarget.mAlpha.end());
	tTarget.mColors.erase(tTarget.mColors.begin(), tTarget.mColors.end());

	tTarget.mWidth = tTexture.mWidth;
	tTarget.mHeight = tTexture.mHeight;

	float lTotalWeight = 0.0f;
	for (auto lWeight: tFilter.mWeights)
	{
		lTotalWeight += lWeight;
	}
	
	for (int j = 0; j < tTexture.mHeight; j++)
	{
	for (int i = 0; i < tTexture.mWidth; i++)
	{
		float 		lAlphaAccum = 0.0f;
		Math::vec3 	lColorAccum {0.0f};

		for (int ky = -tFilter.mSigmaY; ky <= tFilter.mSigmaY; ky++)
		{
		for (int kx = -tFilter.mSigmaX; kx <= tFilter.mSigmaX; kx++)
		{
			int lIndex = (Math::clamp<int>(i + kx, 0, tTexture.mWidth) +
				      Math::clamp<int>(j + ky, 0, tTexture.mHeight) * tTexture.mWidth);
			int lWIndex = (tFilter.mSigmaX * 2 + 1) * (tFilter.mSigmaY + ky) + (tFilter.mSigmaX + kx);

			lAlphaAccum += (tTexture.mAlpha[lIndex] * tFilter.mWeights[lWIndex]);
			lColorAccum += (tTexture.mColors[lIndex] * tFilter.mWeights[lWIndex]);
		}
		}
		tTarget.mAlpha.push_back(lAlphaAccum / lTotalWeight);
		tTarget.mColors.push_back(lColorAccum / lTotalWeight);
	}
	}
};

void filterGaussian(float tSigma, const Texture& tTexture, Texture& tTarget)
{
	int lSigmaInt = static_cast<int>(tSigma);
	std::vector<float> lWeights = initGaussian(lSigmaInt, tSigma);

	LinearFilter lFilter { lSigmaInt, 0, lWeights };
	
	Texture lTempTexture;

	filter(lFilter, tTexture, lTempTexture);

	lFilter.mSigmaX = 0;
	lFilter.mSigmaY = lSigmaInt;

	filter(lFilter, lTempTexture, tTarget);
};

}
