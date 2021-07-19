#include "CameraUtils.hpp"

#include "Rotor.hpp"
#include "mat3.hpp"
#include "Components.hpp"
#include "Manager.hpp"

extern ecs::Manager gManager;

namespace Renderer
{


void lookAt(ecs::Entity tCameraId, const Math::vec3& tFocus)
{

	Camera& lCamera = gManager.getComponent<Camera>(tCameraId);
	
	Math::vec3 lUp {0.0f, 1.0f, 0.0f};

	Math::vec3 lxCam;
	Math::vec3 lyCam;
	Math::vec3 lzCam;

	lzCam = lCamera.mPosition - tFocus;
	lzCam.normalize();

	lxCam = lUp ^ lzCam;
	lxCam.normalize();

	lyCam = lzCam ^ lxCam;

	Math::mat3 lRotationMatrix = transpose(Math::mat3(lxCam, lyCam, lzCam));

	lCamera.mRotor = Math::Rotor(lRotationMatrix);
}


}
