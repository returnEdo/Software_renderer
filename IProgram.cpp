#include "IProgram.hpp"

#include "MathUtils.hpp"


namespace Renderer
{


BBox IProgram::updateBoundingBox(void)
{
	Math::vec3 u = mVertexOutput[0].mPosition;
	Math::vec3 v = mVertexOutput[1].mPosition;
	Math::vec3 w = mVertexOutput[2].mPosition;
	
	return {
		{std::min(std::min(u.x, v.x), w.x), std::min(std::min(u.y, v.y), w.y)},
		{std::max(std::max(u.x, v.x), w.x), std::max(std::max(u.y, v.y), w.y)}
		};
};


void IProgram::updateBarycentric(int i, int j)
{
	Math::vec3 x {static_cast<float>(i), static_cast<float>(j), 0.0f};

	mBarycentric = Math::findBarycentric(x, mVertexOutput[0].mPosition,
						mVertexOutput[1].mPosition,
						mVertexOutput[2].mPosition);
};


void IProgram::updateFragmentPosition(int i, int j)
{
	Math::vec2 xp {mUniform->mWidth / mUniform->mWidthS * (static_cast<float>(i) - mUniform->mWidthS / 2.0f),
		      -mUniform->mWidth / mUniform->mWidthS * (static_cast<float>(j) - mUniform->mWidthS / (2.0f * mUniform->mAlpha))};

	Math::vec3 xc {xp.x / mUniform->mNearPlane, xp.y / mUniform->mNearPlane, -1.0f};
	xc /= mVarying->mDepth;

	mFragmentPosition = mUniform->mCameraPosition + mUniform->mCameraRotation * xc;
};


void IProgram::render(Mesh& tMesh, Buffers& tBuffers)
{
	int lWidthS 	= static_cast<int>(mUniform->mWidthS);
	int lHeightS 	= static_cast<int>(mUniform->mWidthS / mUniform->mAlpha);
	
	for (const Face& lFace: tMesh.mFaces)
	{
		for (int i = 0; i < 3; i++)
		{
			vertexShader(i, {tMesh.mPositions[lFace.mIndices[i].mPosition],
				      	 tMesh.mNormals[lFace.mIndices[i].mNormal],
				      	 tMesh.mTextureUVs[lFace.mIndices[i].mTextureUV]});
		}

		BBox lBBox = updateBoundingBox();
		
		for (int j = std::max(static_cast<int>(lBBox.mSmallest.y) - 1, 0);
		         j < std::min(static_cast<int>(lBBox.mBiggest.y) + 1, lHeightS); j++)
		{
			for (int i = std::max(static_cast<int>(lBBox.mSmallest.x) - 1, 0);
				 i < std::min(static_cast<int>(lBBox.mBiggest.x) + 1, lWidthS); i++)
			{
				updateBarycentric(i, j);
				if (Math::isInsideBarycentric(mBarycentric))
				{
					updateVarying();
					if (mVarying->mDepth > tBuffers.mDepthBuffer[i + j * lWidthS])
					{
						updateFragmentPosition(i, j);
						tBuffers.mDepthBuffer[i + j * lWidthS] = mVarying->mDepth;
						tBuffers.mFrameBuffer[i + j * lWidthS] = fragmentShader();
					}
				}
			}
		}
	}
};



}
