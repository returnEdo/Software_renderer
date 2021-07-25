#pragma once

#include <array>

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Components.hpp"

#include "Unique.hpp"
#include "Macros.hpp"


namespace Renderer
{

struct IUniform
{

	Math::vec3 	mModelPosition;
	Math::mat3	mModelRotation;
	Math::mat3	mModelShear;
	
	Math::vec3	mCameraPosition;
	Math::mat3	mCameraRotation;
	
	Math::vec3	mLightPosition;
	Math::mat3	mLightRotation;

	float		mWidth;
	float		mWidthS;
	float		mAlpha;
	float		mNearPlane;
};


struct IVarying
{
	Math::vec3	mNormal;
	Math::vec2	mTextureUV;
	float 		mDepth;
};	


struct ISampler
{
	Buffer<Math::vec3>*	mColor;
};


struct IVertexOutput
{
	Math::vec3	mPosition;
	Math::vec3 	mPositionW;
	Math::vec3 	mNormal;
	Math::vec2	mTextureUV;
	float 		mFloat;
};


struct IVertexInput
{
	Math::vec3 	mPosition;
	Math::vec3	mNormal;
	Math::vec2	mTextureUV;
};

struct BBox
{
	Math::vec2	mSmallest;
	Math::vec2	mBiggest;
};

struct Fragment
{
	Math::vec3	mVec;
	float		mFloat;
};

class IProgram
{
	protected:
	
	Memory::Unique<IUniform>	mUniform;
	Memory::Unique<IVarying>	mVarying;
	Memory::Unique<ISampler>	mSampler;

	std::array<IVertexOutput, 3>	mVertexOutput;

	Math::vec3 			mFragmentPosition;		// World
	Math::vec3			mBarycentric;

	BBox updateBoundingBox(void);
	void updateBarycentric(int i, int j);
	void updateFragmentPosition(int i, int j);

	virtual void updateVarying(void) {}

	public:
	
	template <typename UniformType>
	void setUniform(UniformType tUniform) 	{ mUniform.copy<UniformType>(tUniform); }
	template <typename SamplerType>
	void setSampler(SamplerType tSampler)	{ mSampler.copy<SamplerType>(tSampler); }
	void setVarying(void)			{ mVarying.allocate(); }
	
	virtual void vertexShader(int i, IVertexInput tVertexInput) {}
	virtual Fragment fragmentShader(void) { return {}; }
	
	void render(Mesh& tMesh, Buffers& tBuffers);
};


}
