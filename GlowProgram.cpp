#include "GlowProgram.hpp"

#include "MathUtils.hpp"
#include "TextureUtils.hpp"

namespace Renderer
{

void GlowProgram::updateVarying(void)
{
	mVarying->mDepth 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mPosition.z,
									     mVertexOutput[1].mPosition.z,
								 	     mVertexOutput[2].mPosition.z);
	mVarying->mTextureUV 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mTextureUV,
								 	     mVertexOutput[1].mTextureUV,
								 	     mVertexOutput[2].mTextureUV);
};

void GlowProgram::vertexShader(int i, IVertexInput tVertexInput)
{
	mVertexOutput[i].mPositionW = (mUniform->mModelRotation * mUniform->mModelShear * tVertexInput.mPosition +
				       mUniform->mModelPosition - mUniform->mCameraPosition); 

	Math::vec3 lPositionC = transpose(mUniform->mCameraRotation) * mVertexOutput[i].mPositionW; 

	mVertexOutput[i].mPosition = Math::vec3(-mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.x / lPositionC.z + mUniform->mWidthS / 2.0f,
					         mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.y / lPositionC.z + mUniform->mWidthS / (2.0f * mUniform->mAlpha),
					         -1.0f / lPositionC.z);

	mVertexOutput[i].mTextureUV = tVertexInput.mTextureUV;
};

Fragment GlowProgram::fragmentShader(void)
{
	TextureSample lColorSample = sampleTexture(mVarying->mTextureUV, *mSampler->mColor);

	float lAlpha = (lColorSample.mColor.x + 
			lColorSample.mColor.y + 
			lColorSample.mColor.z) / (255.0f * 3.0f);

	return {lColorSample.mColor, lAlpha};
};


}
