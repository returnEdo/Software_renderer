#include <string>
#include <cmath>
#include <vector>

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Rotor.hpp"
#include "MathUtils.hpp"

#include "Macros.hpp"
#include "FileIO.hpp"
#include "Components.hpp"
#include "IProgram.hpp"
#include "Programs.hpp"

#include "CameraUtils.hpp"
#include "TextureUtils.hpp"
#include "Functor.hpp"



class RemapDepth: public Renderer::IFunctor
{
	public:

	float mAlpha 		= 2.0f;
	float mMinimumDistance	= 0.0f;

	virtual float evaluate(float tX) override
	{
	//	return (Math::near(tX, 0.0f, .00001f)? 0.0f: 1.0f);
		//PRINT(std::pow(std::max(tX - mMinimumDistance, 0.0f) / (1.0f - mMinimumDistance + tX), 1.0f / mAlpha));
		return std::pow(std::max(tX + 1.0f/mMinimumDistance, 0.0f) / (1.0f + 1.0f/mMinimumDistance + tX), 1.0f / mAlpha);
	}
};



using namespace Renderer;

int main()
{
	//--------SCENE
	std::string lColorAddress = "./resources/textures/diablo.png";
	std::string lNormalAddress = "./resources/textures/diabloNMtangent.png";

	Transform lTransform {Math::vec3(),
			      Math::mat3(Math::vec3(1.0f)),
			      Math::Rotor(0.0f * M_PI, Math::vec3(0.0f, 1.0f, 0.0f))};

	float FOV 		= M_PI / 2.0f;
	float nearPlane 	= 2.0f;
	float alpha 		= 1.4f;
	float pixelWidth 	= 4000.0f;

	Camera lCamera {Math::vec3(0.0f, 0.0f, 2.5f),
			Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
			2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};

	Camera lLight {Math::vec3(2.0f, 0.0f, 0.0f),
		       Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
		       2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};
	lookAt(lLight, lTransform.mPosition);

	Texture lTexture {};
	PRINT_TERNARY(IO::PNG::read(lColorAddress, lTexture.mVecBuffer, lTexture.mFloatBuffer),
		      "Color texture loaded!!",
		      "Could not load color texture");

	Texture lNormalTexture {};
	PRINT_TERNARY(IO::PNG::read(lNormalAddress, lNormalTexture.mVecBuffer, lTexture.mFloatBuffer),
		      "Normal map loaded!!",
		      "Could not load normal map");

	//--------MESH

	std::string lModelAddress = "./resources/models/diablo.obj";

	Mesh lMesh {};
	PRINT_TERNARY(IO::OBJ::read(lModelAddress, lMesh),
		      "Model loaded!!",
		      "Could not load model!!");

	int lWidthS 	= static_cast<int>(lCamera.mWidthS);
	int lHeightS 	= static_cast<int>(lCamera.mWidthS / lCamera.mAlpha);


	//--------SHADOW TEXTURE
	IUniform lDepthUniform
	{	
		lTransform.mPosition, lTransform.mRotor.getMatrixForm(), lTransform.mShear,
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lLight.mWidth, lLight.mWidthS, lLight.mAlpha, lLight.mNearPlane
	};

	ISampler lDepthSampler
	{
		&lTexture.mVecBuffer
	};

	Buffers lDepthBuffers;
	initBuffer<Math::vec3>(lDepthBuffers.mFrameBuffer, lWidthS, lHeightS, Math::vec3(40.0f));
	initBuffer<float>(lDepthBuffers.mDepthBuffer, lWidthS, lHeightS, 0.0f);

	DepthProgram depthProgram;
	depthProgram.setUniform<IUniform>(lDepthUniform);
	depthProgram.setSampler<ISampler>(lDepthSampler);
	depthProgram.setVarying();
	depthProgram.render(lMesh, lDepthBuffers);

//	for (int i = 0; i < lDepthBuffers.mDepthBuffer.mData.size(); i++)
//	{
//		if (not Math::near(lDepthBuffers.mDepthBuffer.mData[i], 0.0f, 0.0001f))
//		{
//			PRINT(i);
//		}
//	}

//	RemapDepth lRemapFunctor;
//	lRemapFunctor.mAlpha = 3.0f;
//	lRemapFunctor.mMinimumDistance = -19.0f;
//	PRINT_TERNARY(IO::PPM::write("./depthRender.ppm", lDepthBuffers.mDepthBuffer, &lRemapFunctor), "Depth saved!!", "Depth not saved!!");

	//--------UNIFORM
	IUniform lUniform
	{	
		lTransform.mPosition, lTransform.mRotor.getMatrixForm(), lTransform.mShear,
		lCamera.mPosition, lCamera.mRotor.getMatrixForm(),
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lCamera.mWidth, lCamera.mWidthS, lCamera.mAlpha, lCamera.mNearPlane
	};

	ShadowSampler lSampler
	{
		{&lTexture.mVecBuffer},		// texture
		&lDepthBuffers.mDepthBuffer	// depth for light pov
	};
	

	//--------BUFFERS
	Buffers lBuffers {};

	initBuffer<Math::vec3>(lBuffers.mFrameBuffer, lWidthS, lHeightS, Math::vec3(40.0f));
	initBuffer<float>(lBuffers.mDepthBuffer, lWidthS, lHeightS, 0.0f);

	//-------RENDERING
	ShadowProgram program;		
	program.setUniform<IUniform>(lUniform);
	program.setSampler<ShadowSampler>(lSampler);
	program.setVarying();
	program.render(lMesh, lBuffers);

	//-------SAVING
	std::string lSaveAddress = "./softwareRenderer.ppm";

	PRINT_TERNARY(IO::PPM::write(lSaveAddress, lBuffers.mFrameBuffer), "Render saved!!", "Render not saved!!");



	return 0;
};
