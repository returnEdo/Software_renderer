#include "IProgram.hpp"

#include "PNGio.hpp"
#include "MathUtils.hpp"
#include "Macros.hpp"


namespace Renderer
{


// Evaluate bbox of triangle to be rasterized in screen space
BBox IProgram::getBoundingBox(void) const
{
	Math::vec3 u = mVertexOutput[0].mPosition;
	Math::vec3 v = mVertexOutput[1].mPosition;
	Math::vec3 w = mVertexOutput[2].mPosition;

	return {
		 {std::min(std::min(u.x, v.x), w.x), std::min(std::min(u.y, v.y), w.y)},
 		 {std::max(std::max(u.x, v.x), w.x), std::max(std::max(u.y, v.y), w.y)} 
	       };
}

// ...
void IProgram::updateBarycentric(int i, int j)
{
	Math::vec3 lFragmentPosition {static_cast<float>(i), static_cast<float>(j), 0.0f};
	
	mBarycentric = Math::findBarycentric(lFragmentPosition, mVertexOutput[0].mPosition,
								mVertexOutput[1].mPosition,
								mVertexOutput[2].mPosition);
}

// Evaluates world frame position of the current fragment
void IProgram::updateFragmentPosition(int i, int j)
{
	Math::vec2 xp {mUniform -> mWidth / mUniform -> mWidthS * (static_cast<float>(i) - mUniform -> mWidthS / 2.0f),
		       -mUniform -> mWidth / mUniform -> mWidthS * (static_cast<float>(j) - mUniform -> mWidthS / (2.0f * mUniform -> mAlpha))};

	Math::vec3 xc {xp.x / mUniform -> mNearPlane, xp.y / mUniform -> mNearPlane, -1.0f};
	xc /= mVarying -> mDepth;

	mFragmentPosition = mUniform -> mCameraPosition + transpose(mUniform -> mCameraRotationT) * xc;
}

// Full mesh rendering
void IProgram::render(const Mesh& tMesh, Buffers& tBuffers)
{
	int lWidthS 	= static_cast<int>(mUniform->mWidthS);
	int lHeightS	= static_cast<int>(mUniform->mWidthS / mUniform->mAlpha);

	int m = 0, n = 0;
	for (Face lFace: tMesh.mFaces)
	{
		LOOP(i, 0, 3, 1)
		{
			vertex(i, {tMesh.mPositions[lFace.mIndices[i].mPosition],
			           tMesh.mNormals[lFace.mIndices[i].mNormal],
			           tMesh.mTextureUVs[lFace.mIndices[i].mTextureUV]});
		}

		BBox lBBox = getBoundingBox();
		LOOP(j, std::max(static_cast<int>(lBBox.mSmallest.y - 1), 0), std::min(static_cast<int>(lBBox.mBiggest.y + 1), lHeightS), 1) {
		LOOP(i, std::max(static_cast<int>(lBBox.mSmallest.x - 1), 0), std::min(static_cast<int>(lBBox.mBiggest.x + 1), lWidthS), 1) {

		updateBarycentric(i, j);
		if (Math::isInsideBarycentric(mBarycentric))
		{
			updateVarying();
			if ((mVarying->mDepth) > tBuffers.mDepthBuffer[i + j * lWidthS])
			{
				updateFragmentPosition(i, j);
				tBuffers.mFrameBuffer[i + j * lWidthS] 	= fragment();
				tBuffers.mDepthBuffer[i + j * lWidthS] 	= mVarying->mDepth;
			}
		}
	
		}
		}
		
	}
};

}
