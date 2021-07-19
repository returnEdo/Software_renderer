#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Components.hpp"

namespace Renderer
{

using VertexInput = VertexOutput;

// Uniforms are constant along the pipeline
struct IUniform
{
	// Camera
	Math::vec3	mCameraPosition;
	Math::mat3	mCameraRotationT;

	float 		mWidthS;
	float 		mWidth;
	float 		mAlpha;
	float 		mNearPlane;

	// Model
	Math::vec3	mModelPosition;
	Math::mat3 	mModelRotation;
	Math::mat3	mModelShear;
};


// Varying are interpolated
struct IVarying
{
	Math::vec3 	mNormal;
	Math::vec2 	mTextureUV;

	float		mDepth;
};


// Samplers are to be sampled :P
struct ISampler
{
	Texture*	mColor; 
};

// 2D Triangle BBox
struct BBox
{	
	Math::vec2	mSmallest;
	Math::vec2	mBiggest;
};


// The program operates the vertex and fragment shader
// rendering a scene
class IProgram
{
	protected:

	IUniform*	mUniform;
	IVarying*	mVarying;
	ISampler*	mSampler;

	VertexOutput	mVertexOutput[3];

	Math::vec3 	mFragmentPosition;
	Math::vec3 	mBarycentric;

	BBox getBoundingBox(void) const;
	void updateBarycentric(int i, int j);
	/*IMPLEMENT*/ virtual void updateVarying(void) {} 
	void updateFragmentPosition(int i, int j);

	public:

	inline void setUniform(IUniform* tUniform)	{ mUniform = tUniform; }
	inline void setSampler(ISampler* tSampler)	{ mSampler = tSampler; }

	/*IMPLEMENT*/virtual void vertex(int i, const VertexInput& tVertexInput) {}
	/*IMPLEMENT*/virtual Math::vec3 fragment(void) { return Math::vec3(); }
	void render(const Mesh& tMesh, Buffers& tBuffers);
};

}
