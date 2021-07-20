#include "Program.hpp"

#include "MathUtils.hpp"
#include "TextureUtils.hpp"

namespace Renderer
{

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
	(mVarying->mNormal).normalize();
};

void Program::vertexShader(int i, IVertexInput tVertexInput)
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
};

Math::vec3 Program::fragmentShader(void)
{
	NormalSampler& lSampler = mSampler.get<NormalSampler>();

	Math::vec3 L = mUniform->mLightPosition - mFragmentPosition;
	L.normalize();

	// Retrieve normal

	Math::vec3 v_u = mVertexOutput[1].mPositionW - mVertexOutput[0].mPositionW;
	Math::vec3 w_u = mVertexOutput[2].mPositionW - mVertexOutput[0].mPositionW;

	Math::vec3 p {mVertexOutput[1].mTextureUV.x - mVertexOutput[0].mTextureUV.x,
		      mVertexOutput[2].mTextureUV.x - mVertexOutput[0].mTextureUV.x, 0.0f};
	Math::mat3 Q {v_u, w_u, mVarying->mNormal};
	// TODO: check singular
	Math::vec3 grad_u = inv(Q) * p;
	grad_u.normalize();
	
	Math::vec3 v_hat = (mVarying->mNormal) ^ grad_u;
	Math::mat3 M_ts_W = transpose(Math::mat3(grad_u, v_hat, mVarying->mNormal));
	

	TextureSample lNormalSample = sampleTexture(mVarying->mTextureUV, *lSampler.mNormal);
	
	Math::vec3 sampledNormal = lNormalSample.mColor - Math::vec3(255.0f / 2.0f);
	sampledNormal.normalize();
	
	Math::vec3 lNormal = M_ts_W * sampledNormal; 

	float lDiffuse = std::max(L * mVarying->mNormal, 0.0f);	

	TextureSample lColorSample = sampleTexture(mVarying->mTextureUV, *lSampler.mNormal);

	return (lDiffuse * Math::vec3(250.0f));
};


}
