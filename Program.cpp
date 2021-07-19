#include "Program.hpp"

#include "PNGio.hpp"
#include "MathUtils.hpp"
#include "Macros.hpp"


namespace Renderer
{

// Varying are interpolated from vertex values using barycentric coordinates
void Program::updateVarying(void)
{
	mVarying->mDepth 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mPosition.z,
									     mVertexOutput[1].mPosition.z,
									     mVertexOutput[2].mPosition.z);
	mVarying->mNormal 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mNormal,
									     mVertexOutput[1].mNormal,
									     mVertexOutput[2].mNormal);
	mVarying->mTextureUV 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mTextureUV,
									     mVertexOutput[1].mTextureUV,
									     mVertexOutput[2].mTextureUV);
}

// Basic vertex shader implementation
void Program::vertex(int i, const VertexInput& tVertexInput)
{
	Math::vec3 lPositionC = mUniform->mCameraRotationT * (mUniform->mModelRotation * mUniform->mModelShear * tVertexInput.mPosition +
					      		       mUniform->mModelPosition - mUniform->mCameraPosition); 

	mVertexOutput[i].mPosition = Math::vec3(-mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.x / lPositionC.z + mUniform->mWidthS / 2.0f,
					      mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.y / lPositionC.z + mUniform->mWidthS / (2.0f * mUniform->mAlpha),
					     -1.0f / lPositionC.z);

	mVertexOutput[i].mNormal = mUniform->mModelRotation * diagonalInv(mUniform->mModelShear) * tVertexInput.mNormal;
	mVertexOutput[i].mNormal.normalize();
	
	mVertexOutput[i].mTextureUV = tVertexInput.mTextureUV;
};

// Basic fragment shader implementation
Math::vec3 Program::fragment(void)
{
	Uniform* lUniform = static_cast<Uniform*>(mUniform);

	Math::vec3 L = lUniform->mLightPosition - mFragmentPosition;
	L.normalize();

	float lDiffuse = std::max(L * mVarying->mNormal, 0.0f);

	TextureSample lColorSample = sampleTexture(mVarying->mTextureUV, *mSampler->mColor);

//	return (lDiffuse * Math::vec3(200.0f));
	return (lDiffuse * lColorSample.mColor);
};


}
