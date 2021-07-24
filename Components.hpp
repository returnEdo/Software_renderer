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

using FrameBuffer = std::vector<Math::vec3>;
using DepthBuffer = std::vector<float>;

struct Buffers
{
	FrameBuffer	mFrameBuffer;
	DepthBuffer	mDepthBuffer;
	DepthBuffer	mAlphaBuffer;
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

struct Texture
{
	std::vector<float>	mAlpha;
	std::vector<Math::vec3>	mColors;

	int mWidth;
	int mHeight;
};


struct TextureSample
{	
	float 		mAlpha;
	Math::vec3	mColor;
};


}
