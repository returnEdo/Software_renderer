#pragma once

#include <array>
#include <vector>
#include <cmath>

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Rotor.hpp"


/*
All the components of the renderer
are stored here
*/

namespace Renderer
{


struct Transform
{
	Math::vec3 	mPosition;
	Math::mat3 	mShear;
	Math::Rotor	mRotor;
};



struct Camera
{
	Math::vec3	mPosition;
	Math::Rotor	mRotor;

	float 	mWidth;
	float 	mAlpha;
	float 	mNearPlane;

	float	mWidthS;	// width in screen space
};


// Indices to vectors
struct Indices
{
	int mPosition;
	int mTextureUV;
	int mNormal;
};


// Data for single face
struct Face
{
	std::array<Indices, 3>  mIndices;
};


// Data contained in .obj file
struct Mesh
{
	std::vector<Math::vec3>	mPositions;
	std::vector<Math::vec3> mNormals;
	std::vector<Math::vec2>	mTextureUVs;

	std::vector<Face>	mFaces;
};


template <typename Type>
struct Buffer
{	
	std::vector<Type>	mData;
	
	int mWidth;
	int mHeight;
};

struct Buffers
{
	Buffer<Math::vec3>	mFrameBuffer;
	Buffer<float>		mDepthBuffer;
	Buffer<float>		mAlphaBuffer;
};


struct Texture
{
	Buffer<Math::vec3>	mVecBuffer;	
	Buffer<float>		mFloatBuffer;
};	


}
