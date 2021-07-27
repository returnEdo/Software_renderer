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
#include "SSAO.hpp"



class RemapDepth: public Renderer::IFunctor
{
	public:

	float mAlpha 		= 2.0f;

	virtual float evaluate(float tX) override
	{
		return std::pow(std::max(tX, 0.0f) / (1.0f + tX), 1.0f / mAlpha);
	}
};


class Identity: public Renderer::IFunctor
{	
	public:

	virtual float evaluate(float tX) override
	{
		return tX;
	}

};



using namespace Renderer;

int main()
{
	// Scene setup
	Transform lTransform {Math::vec3(0.0f, -1.5f, -3.0f),
			      Math::mat3(Math::vec3(1.0f / 50.0f)),
			      Math::Rotor(1.1f * M_PI, Math::vec3(0.0f, 1.0f, 0.0f))};

	float FOV 		= M_PI / 2.0f;
	float nearPlane 	= 2.0f;
	float alpha 		= 1.4f;
	float pixelWidth 	= 2000.0f;

	Camera lCamera {Math::vec3(0.0f, 0.0f, 2.5f),
			Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
			2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};

	Camera lLight {Math::vec3(0.5f, -2.5f, -1.0f),
		       Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f)),
		       2.0f * nearPlane * std::tan(FOV / 2.0f), alpha, nearPlane, pixelWidth};
	lookAt(lLight, lTransform.mPosition);


	// Texture loading
	Texture lColorTexture;
	Texture lNormalTexture;
	Texture lSpecularTexture;
	PRINT_TERNARY(IO::PNG::read("./resources/textures/drone_diffuse.png", lColorTexture.mVecBuffer, lColorTexture.mFloatBuffer), "y", "n");
	PRINT_TERNARY(IO::PNG::read("./resources/textures/drone_normal.png", lNormalTexture.mVecBuffer, lNormalTexture.mFloatBuffer), "y", "n");
	//IO::PNG::read("./resources/textures/diablo.png", lColorTexture.mVecBuffer, lColorTexture.mFloatBuffer);
	//IO::PNG::read("./resources/textures/diabloNMtangent.png", lNormalTexture.mVecBuffer, lNormalTexture.mFloatBuffer);
	
	// Mesh loading
	Mesh lMesh {};
	PRINT_TERNARY(IO::OBJ::read("./resources/models/drone.obj", lMesh), "y", "n");
//	IO::OBJ::read("./resources/models/diablo.obj", lMesh);

	int lWidthS 	= static_cast<int>(lCamera.mWidthS);
	int lHeightS 	= static_cast<int>(lCamera.mWidthS / lCamera.mAlpha);

	// SHADOWS
	// Shadow map construction. Will render to a Buffer<float> the depth
	// as seen from the light source
	IUniform lUniform
	{	
		lTransform.mPosition, lTransform.mRotor.getMatrixForm(), lTransform.mShear,
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lLight.mPosition, lLight.mRotor.getMatrixForm(),
		lCamera.mWidth, lCamera.mWidthS, lCamera.mAlpha, lCamera.mNearPlane
	};

	ISampler lShadowSampler { nullptr };

	Buffers lShadowBuffers;
	initBuffer<float>(lShadowBuffers.mDepthBuffer, lWidthS, lHeightS, 0.0f);
	initBuffer<Math::vec3>(lShadowBuffers.mFrameBuffer, lWidthS, lHeightS, Math::vec3());

	DepthProgram lDepthProgram;
	lDepthProgram.setUniform<IUniform>(lUniform);
	lDepthProgram.setSampler<ISampler>(lShadowSampler);
	lDepthProgram.setVarying();
	lDepthProgram.render(lMesh, lShadowBuffers);
	

	// AMBIENT OCCLUSION
	// SSAO map. We need to render the scene from the camera pov. Then postprocess
	// the depth field.
	lUniform.mCameraPosition = lCamera.mPosition;
	lUniform.mCameraRotation = lCamera.mRotor.getMatrixForm();
	
	Buffers lAObuffers;
	initBuffer<float>(lAObuffers.mDepthBuffer, lWidthS, lHeightS, 0.0f);
	initBuffer<Math::vec3>(lAObuffers.mFrameBuffer, lWidthS, lHeightS, Math::vec3());

	// We can use the same depth renderer as above
	lDepthProgram.setUniform<IUniform>(lUniform);	
	lDepthProgram.render(lMesh, lAObuffers);

	// No we need to post process the texture just created
	Buffer<float> lAmbientOccluded;
	AfterEffects::AOsettings lSettings { 800, 0.05f };
	AfterEffects::SSAO(lAObuffers.mDepthBuffer, lAmbientOccluded, lCamera, lSettings);


	// FINAL RENDER
	// Set the textures (shadow, ao, ...)
	FullSampler lSampler
	{
		{&lColorTexture.mVecBuffer},
		&lNormalTexture.mVecBuffer,
		&lAmbientOccluded,
		&lShadowBuffers.mDepthBuffer
	};

	Buffers lRenderBuffers {};

	initBuffer<Math::vec3>(lRenderBuffers.mFrameBuffer, lWidthS, lHeightS, Math::vec3(40.0f));
	initBuffer<float>(lRenderBuffers.mDepthBuffer, lWidthS, lHeightS, 0.0f);

	FullProgram program;		
	program.setUniform<IUniform>(lUniform);
	program.setSampler<FullSampler>(lSampler);
	program.setVarying();
	program.render(lMesh, lRenderBuffers);

	IO::PPM::write("./renderPreSSAO.ppm", lRenderBuffers.mFrameBuffer);
	AfterEffects::applySSAO(lAmbientOccluded, lRenderBuffers.mFrameBuffer);
	
	//RemapDepth lFunctor;
	//IO::PPM::write("./render.ppm", lShadowBuffers.mDepthBuffer, &lFunctor);
	IO::PPM::write("./render.ppm", lRenderBuffers.mFrameBuffer);

	return 0;
};
