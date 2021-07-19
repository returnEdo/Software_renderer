#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#include "Macros.hpp"
#include "Unique.hpp"

#include "Components.hpp"
#include "FileIO.hpp"

#include "MathUtils.hpp"
#include "CameraUtils.hpp"
#include "EntityComponentSystem.hpp"
#include "IProgram.hpp"
#include "Program.hpp"



using namespace Renderer; 
using Memory::Unique;

ecs::Manager gManager;


int main()
{
	gManager.init();

	// Addresses
	std::string lModelAddress 	= "./resources/models/diablo.obj";
	std::string lTextureAddress	= "./resources/textures/diablo.png";

	// Create entities
	ecs::Entity lModelId 		= gManager.createEntity();
	ecs::Entity lCameraId		= gManager.createEntity();
	ecs::Entity lBuffersId		= gManager.createEntity();
	ecs::Entity lLightId		= gManager.createEntity();
	
	// Buffers init
	Buffers& lBuffers = gManager.addComponent<Buffers>(lBuffersId, {});

	// Model loading
	Mesh& lMesh = gManager.addComponent<Mesh>(lModelId, {});	
	IO::OBJ::read(lModelAddress, lModelId);

	// Model transform setting
	Transform& lTransform = gManager.addComponent<Transform>(lModelId,
								 {
									Math::vec3(0.0f, 0.0f, 0.0f),
									Math::mat3(Math::vec3(1.0f, 1.0f, 1.0f)),
									Math::Rotor(0.0f*M_PI, Math::vec3(0.0f, 1.0f, 0.0f))
								 });
	
	// Camera setting
	float lNearPlane = 3.0f;
	float lFov = M_PI / 3.0f;
	float lAlpha = 1.3f;

	Camera& lCamera = gManager.addComponent<Camera>(lCameraId,
							{
							Math::vec3(.0f, .0f, 4.0f),
							Math::Rotor(0.0f, Math::vec3(0.0, 0.0f, 1.0f)),
							(2.0f * lNearPlane * std::tan(lFov / 2.0f)),
							lAlpha, lNearPlane, 2000.0f
							});
	lookAt(lCameraId, lTransform.mPosition);

	// Light setting
	Camera& lLight = gManager.addComponent<Camera>(lLightId,
							{
							Math::vec3(10.0f, 10.0f, 10.0f),
							Math::Rotor(0.0f, Math::vec3(0.0, 0.0f, 1.0f)),
							(2.0f * lNearPlane * std::tan(lFov / 2.0f)),
							lAlpha, lNearPlane, 2000.0f
							});
	lookAt(lLightId, lTransform.mPosition);

	// Texture loading
	Texture& lTexture = gManager.addComponent<Texture>(lModelId, {});
	PRINT(IO::PNG::read(lTextureAddress, lModelId)? "Texture loaded!": "Cannot load texture!!");

	Uniform lUniform
	{
	       {lCamera.mPosition,
		transpose(lCamera.mRotor.getMatrixForm()),
		lCamera.mWidthS,
		lCamera.mWidth,
		lCamera.mAlpha,
		lCamera.mNearPlane,
		lTransform.mPosition,
		lTransform.mRotor.getMatrixForm(),
		lTransform.mShear},
		lLight.mPosition,
		transpose(lLight.mRotor.getMatrixForm())
	};
	
	ISampler lSampler
	{
		&lTexture
	};

	int lWidth = static_cast<int>(lUniform.mWidthS);
	int lHeight = static_cast<int>(lUniform.mWidthS / lUniform.mAlpha);
	
	for (int i = 0; i < lWidth * lHeight; i++)
	{
		lBuffers.mFrameBuffer.push_back(Math::vec3(40.0f));
		lBuffers.mDepthBuffer.push_back(0.0f);
	}
	
	// Rendering
	Program program;

	program.setUniform(&lUniform);
	program.setSampler(&lSampler);
	program.render(lMesh, lBuffers);


	// Saving
	std::string lAddress = "softwareRenderer.ppm";
	if(IO::PPM::write(lAddress, lBuffers.mFrameBuffer,
	  	          static_cast<int>(lUniform.mWidthS),
		          static_cast<int>(lUniform.mWidthS / lUniform.mAlpha)))
	{
		PRINT(".ppm saved!!");
	};

	// Clean up
	gManager.destroy();


	return 0;
};
