#include "CameraUtils.hpp"

#include "Rotor.hpp"
#include "mat3.hpp"
#include "Components.hpp"


namespace Renderer
{


void lookAt(Camera& tCamera, const Math::vec3& tFocus)
{
	
	Math::vec3 lUp {0.0f, 1.0f, 0.0f};

	Math::vec3 lxCam;
	Math::vec3 lyCam;
	Math::vec3 lzCam;

	lzCam = tCamera.mPosition - tFocus;
	lzCam.normalize();

	lxCam = lUp ^ lzCam;
	lxCam.normalize();

	lyCam = lzCam ^ lxCam;

	Math::mat3 lRotationMatrix = transpose(Math::mat3(lxCam, lyCam, lzCam));

	tCamera.mRotor = Math::Rotor(lRotationMatrix);
}


}
