#include "DepthProgram.hpp"

#include "MathUtils.hpp"
#include "TextureUtils.hpp"

namespace Renderer
{

void DepthProgram::updateVarying(void)
{
	mVarying->mDepth 	= Math::interpolateBarycentric(mBarycentric, mVertexOutput[0].mPosition.z,
									     mVertexOutput[1].mPosition.z,
								 	     mVertexOutput[2].mPosition.z);
	(mVarying->mNormal).normalize();
};

bool DepthProgram::vertexShader(int i, IVertexInput tVertexInput)
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

Fragment DepthProgram::fragmentShader(void)
{
	return {Math::vec3(), 1.0f};
};


}
