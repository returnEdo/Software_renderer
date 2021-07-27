#include "FullProgram.hpp"

#include "MathUtils.hpp"
#include "TextureUtils.hpp"

namespace Renderer
{

void FullProgram::updateVarying(void)
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
	(mVarying->mNormal).normalize();
};

bool FullProgram::vertexShader(int i, IVertexInput tVertexInput)
{
	mVertexOutput[i].mPositionW = (mUniform->mModelRotation * mUniform->mModelShear * tVertexInput.mPosition +
				       mUniform->mModelPosition - mUniform->mCameraPosition); 

	Math::vec3 lPositionC = transpose(mUniform->mCameraRotation) * mVertexOutput[i].mPositionW; 

	mVertexOutput[i].mPosition = Math::vec3(-mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.x / lPositionC.z + mUniform->mWidthS / 2.0f,
					         mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * lPositionC.y / lPositionC.z + mUniform->mWidthS / (2.0f * mUniform->mAlpha),
					         -1.0f / lPositionC.z);

	mVertexOutput[i].mNormal = mUniform->mModelRotation * diagonalInv(mUniform->mModelShear) * tVertexInput.mNormal;
	mVertexOutput[i].mNormal.normalize();
	
	mVertexOutput[i].mTextureUV = tVertexInput.mTextureUV;

	return (((mVertexOutput[i].mPositionW - mUniform->mCameraPosition) * mVertexOutput[i].mNormal) < 0.0f);
};

Fragment FullProgram::fragmentShader(void)
{
	// Sampling textures
	FullSampler& lSampler = mSampler.get<FullSampler>();
	Math::vec3 lNormalSample = sampleTexture<Math::vec3>(mVarying->mTextureUV, *lSampler.mNormal);
	Math::vec3 lColorSample  = sampleTexture<Math::vec3>(mVarying->mTextureUV, *lSampler.mColor);

	// Some useful unit vectors
	Math::vec3 L = mUniform->mLightPosition - mFragmentPosition;
	Math::vec3 lView = mUniform->mCameraPosition - mFragmentPosition;
	L.normalize();
	lView.normalize();

	Math::vec3 rL = transpose(mUniform->mLightRotation) * (mFragmentPosition - mUniform->mLightPosition);

	// project in light frame
	Math::vec2 rLS {-mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * rL.x / rL.z + mUniform->mWidthS / 2.0f,
			 mUniform->mWidthS / mUniform->mWidth * mUniform->mNearPlane * rL.y / rL.z + mUniform->mWidthS / (2.0f * mUniform->mAlpha)};
	
	int lIndex = static_cast<int>(std::round(rLS.x) + std::round(rLS.y) * mUniform->mWidthS);

	float lFragmentDepth = -1.0f / rL.z;
	float lLightDepth = lSampler.mShadow->mData[std::max(std::min(lIndex, static_cast<int>(lSampler.mShadow -> mData.size())), 0)];

	if (not Math::near(lLightDepth, lFragmentDepth, 0.001f))
	{
	//	return {(0.2f * lColorSample), 1.0f};
		return {Math::vec3(100.0f, 200.0f, 100.0f), 1.0f};
	}

	// Retrieve normal
	Math::vec3 v_u = mVertexOutput[1].mPositionW - mVertexOutput[0].mPositionW;
	Math::vec3 w_u = mVertexOutput[2].mPositionW - mVertexOutput[0].mPositionW;

	Math::vec3 p {mVertexOutput[1].mTextureUV.x - mVertexOutput[0].mTextureUV.x,
		      mVertexOutput[2].mTextureUV.x - mVertexOutput[0].mTextureUV.x, 0.0f};
	Math::mat3 Q {v_u, w_u, mVarying->mNormal};
	Math::vec3 grad_u = inv(Q) * p;
	grad_u.normalize();
	
	Math::vec3 v_hat = (mVarying->mNormal) ^ grad_u;
	Math::mat3 M_ts_W = transpose(Math::mat3(grad_u, v_hat, mVarying->mNormal));
	
	Math::vec3 sampledNormal = lNormalSample - Math::vec3(255.0f / 2.0f);
	sampledNormal.normalize();
	Math::vec3 lNormal = M_ts_W * sampledNormal; 

	float lDiffuse = std::max(L * lNormal, 0.0f);	

	// Specular
	float lSpecular = 0.0f;
	if (lDiffuse >= 0.000001f)
	{
		Math::vec3 lReflected = Math::reflect(L, lNormal);
		lSpecular = std::pow(std::max(lReflected*lView, 0.0f), 100.0f); 
	}

	// AO
//	float lAmbient = sampleTexture<float>(mVarying->mTextureUV, *lSampler.mSpecular);

	return {Math::vec3(100.0f, 100.0f, 200.0f), 1.0f};
	//return {((.2f + .4f*lSpecular + lDiffuse)* lColorSample), 1.0f};
};


}
