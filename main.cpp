#include <string>
#include <cmath>
#include <vector>

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "Rotor.hpp"

#include "Macros.hpp"
#include "FileIO.hpp"
#include "Components.hpp"
#include "IProgram.hpp"
#include "Program.hpp"
#include "GlowProgram.hpp"

#include "CameraUtils.hpp"
#include "TextureUtils.hpp"

#include "PostProcessing.hpp"


std::vector<Math::vec3> blend(const std::vector<Math::vec3>& tBuffer1,
			      const std::vector<Math::vec3>& tBuffer2,
			      const std::vector<float>& tAlpha)
{
	std::vector<Math::vec3> lBlended;

	for (int i = 0; i < tBuffer1.size(); i++)
	{	
		lBlended.push_back(tBuffer1[i] * (1.0f - tAlpha[i]) + tBuffer2[i] * tAlpha[i]);
	}

	return lBlended;
}

using namespace Renderer;

int main()
{
	//--------SCENE
	std::string lColorAddress = "./resources/textures/diablo.png";
	std::string lNormalAddress = "./resources/textures/diabloNMtangent.png";
	std::string lGlowAddress = "./resources/textures/diabloGlow.png";

	Transform lTransform {Math::vec3(),
			      Math::mat3(Math::vec3(1.0f)),
			      Math::Rotor(0.0f * M_PI, Math::vec3(0.0f, 1.0f, 0.0f))};

	float FOV 		= M_PI / 2.0f;
	float nearPlane 	= 2.0f;
	float alpha 		= 1.4f;
	float pixelWidth 	= 1000.0f;

	Camera lCamera {Math::vec3(0.0f, 0.0f, 2.5f),
			Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
			2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};

	Camera lLight {Math::vec3(10.0f, 10.0f, 10.0f),
		       Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
		       2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};
	lookAt(lLight, lTransform.mPosition);

	Texture lTexture {};
	PRINT_TERNARY(IO::PNG::read(lColorAddress, lTexture),
		      "Color texture loaded!!",
		      "Could not load color texture");

	Texture lNormalTexture {};
	PRINT_TERNARY(IO::PNG::read(lNormalAddress, lNormalTexture),
		      "Normal map loaded!!",
		      "Could not load normal map");

	Texture lGlowTexture {};
	PRINT_TERNARY(IO::PNG::read(lGlowAddress, lGlowTexture),
		      "Glow map loaded!!",
		      "Could not load glow map");


	//--------MESH

	std::string lModelAddress = "./resources/models/diablo.obj";

	Mesh lMesh {};
	PRINT_TERNARY(IO::OBJ::read(lModelAddress, lMesh),
		      "Model loaded!!",
		      "Could not load model!!");

	//--------UNIFORM
	IUniform lUniform
	{	
		lTransform.mPosition, lTransform.mRotor.getMatrixForm(), lTransform.mShear,
		lCamera.mPosition, lCamera.mRotor.getMatrixForm(),
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lCamera.mWidth, lCamera.mWidthS, lCamera.mAlpha, lCamera.mNearPlane
	};

	NormalSampler lSampler
	{
		{&lTexture},
		&lNormalTexture
	};

	//--------BUFFERS
	
	Buffers lBuffers {};
	Buffers lGlowBuffers {};

	int lWidthS 	= static_cast<int>(lUniform.mWidthS);
	int lHeightS 	= static_cast<int>(lUniform.mWidthS / lUniform.mAlpha);

	for (int i = 0; i < lWidthS * lHeightS; i++)
	{
		lBuffers.mFrameBuffer.push_back(Math::vec3(40.0f));
		lBuffers.mDepthBuffer.push_back(0.0f);
		lBuffers.mAlphaBuffer.push_back(1.0f);
		lGlowBuffers.mFrameBuffer.push_back(Math::vec3(0.0f));
		lGlowBuffers.mDepthBuffer.push_back(0.0f);
		lGlowBuffers.mAlphaBuffer.push_back(0.0f);
	}

	//-------RENDERING
	Program program;		
	program.setUniform<IUniform>(lUniform);
	program.setSampler<NormalSampler>(lSampler);
	program.setVarying();
	program.render(lMesh, lBuffers);

	ISampler lSamplerGlow
	{
		&lGlowTexture
	};

	GlowProgram glowProgram;		
	glowProgram.setUniform<IUniform>(lUniform);
	glowProgram.setSampler<ISampler>(lSamplerGlow);
	glowProgram.setVarying();
	glowProgram.render(lMesh, lGlowBuffers);

	Texture lInputGlow
	{
		lGlowBuffers.mAlphaBuffer, lGlowBuffers.mFrameBuffer, 
		lWidthS, lHeightS
	};
	Texture lOutputGlow;
	filterGaussian(3.0f, lInputGlow, lOutputGlow);


	FrameBuffer lBlended = blend(lBuffers.mFrameBuffer, lGlowBuffers.mFrameBuffer, lGlowBuffers.mAlphaBuffer);

	//-------SAVING
	std::string lSaveAddress = "./softwareRenderer.ppm";

	if(IO::PPM::write(lSaveAddress, lGlowBuffers.mFrameBuffer, lWidthS, lHeightS))
	{
		PRINT("File saved!!");
	}

	IO::PPM::write("./glowBlurred.ppm", lBlended, lWidthS, lHeightS);

	return 0;
};
