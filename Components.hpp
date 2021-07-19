#pragma once

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
};


// Indices to vectors
struct Indices
{
	unsigned int mPosition;
	unsigned int mTextureUV;
	unsigned int mNormal;
};


// Data for single face
struct Face
{
	Indices mIndices[3];
};


// Data contained in .obj file
struct Mesh
{
	std::vector<Math::vec3>	mPositions;
	std::vector<Math::vec3> mNormals;
	std::vector<Math::vec2>	mTextureUVs;

	std::vector<Face>	mFaces;
};

struct VertexOutput
{
	Math::vec3 mPosition;
	Math::vec3 mNormal;		// in world space
	Math::vec2 mTextureUV;
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
