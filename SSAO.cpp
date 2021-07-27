#include "SSAO.hpp"

#include <cmath>

#include "MathUtils.hpp"
#include "TextureUtils.hpp"

namespace Renderer
{

namespace AfterEffects
{

constexpr float C_TOLL = 0.000001f;

void SSAO(const Buffer<float>& tDepthBuffer, Buffer<float>& tTarget,
	  const Camera& tCamera, const AOsettings& tSettings)
{
	initBuffer<float>(tTarget, tDepthBuffer.mWidth, tDepthBuffer.mHeight, 0.0f);
	
	for (int j = 0; j < tDepthBuffer.mHeight; j++)
	{
		for (int i = 0; i < tDepthBuffer.mWidth; i++)
		{
			float lCurrentDepth = tDepthBuffer.mData[i + tDepthBuffer.mWidth * j];

			if (Math::near(lCurrentDepth, 0.0f, C_TOLL)) { continue; }
			
			// TODO: compute fragment position lPositionC
			Math::vec2 xp {tCamera.mWidth / tCamera.mWidthS * (static_cast<float>(i) - tCamera.mWidthS / 2.0f),
				      -tCamera.mWidth / tCamera.mWidthS * (static_cast<float>(j) - tCamera.mWidthS / (2.0f * tCamera.mAlpha))};

			Math::vec3 lPositionC {xp.x / tCamera.mNearPlane, xp.y / tCamera.mNearPlane, -1.0f};
			lPositionC /= lCurrentDepth;

			int lCounter = 0;
			for (int k = 0; k < tSettings.mSampleCount; k++)
			{
				Math::vec3 lPositionCp = lPositionC + Math::randf(0.0f, tSettings.mRadius) * Math::randNormal();

				Math::vec3 lPositionSp { -tCamera.mWidthS / tCamera.mWidth * lPositionCp.x / lPositionCp.z * tCamera.mNearPlane + tCamera.mWidthS / 2.0f,
							  tCamera.mWidthS / tCamera.mWidth * lPositionCp.y / lPositionCp.z * tCamera.mNearPlane + tCamera.mWidthS / (2.0f * tCamera.mAlpha),
							  -1.0f / lPositionCp.z};

				float lDepth = tDepthBuffer.mData[static_cast<int>(std::round(lPositionSp.x) + std::round(lPositionSp.y) * tCamera.mWidthS)];
				if (lDepth < lPositionSp.z)
				{
					lCounter += 1;
				}
			}

			tTarget.mData[i + j * tTarget.mWidth] = Math::clamp(2.0f * static_cast<float>(lCounter) / static_cast<float>(tSettings.mSampleCount), 0.0f, 1.0f);
		}
	}

};

void applySSAO(const Buffer<float>& tAO, Buffer<Math::vec3>& tFrameBuffer)
{
	for (int i = 0; i < tFrameBuffer.mWidth * tFrameBuffer.mHeight; i++)
	{
		if (not Math::near(tAO.mData[i], 0.0f, .00001f))
		{
			tFrameBuffer.mData[i] *= tAO.mData[i];
		}
	}
};



}

}
