#include "MyCamera.h"

MyCamera::MyCamera(Simplex::SystemSingleton* pSystem, vector3 ptOrigin, vector3 vEulerOrientation) : 
	m_pSystem(pSystem), 
	m_ptOrigin(ptOrigin), 
	m_vEulerOrientation(vEulerOrientation)
{
	//
}

MyCamera::~MyCamera()
{
	//
}

matrix4 MyCamera::GetViewMatrix()
{
	//vector3 cameraPosition = vector3(2.5f, 0.0f, -10.0f);	//because this gets inverted later on, this is placed at z = -10
	matrix4 mat4View = glm::translate(IDENTITY_M4, m_ptOrigin);	//m_ptOrigin replaces cameraPosition;

	//static vector3 eulerAngles = vector3(0.0f, 0.0f, 0.0f);		//moved to MyCamera.h and renamed
	glm::quat qCameraOrientation = glm::quat(m_vEulerOrientation);
	mat4View = mat4View * glm::toMat4(qCameraOrientation);	//camera looking up, which means object appears to go down

	//eulerAngles.y += 0.05f;

	/*
	// zPos (below) helps demonstrate that ortho projections map the points of an object to the points of your screen.
	// Changing this value does NOT change how the object looks in terms of size.
	// However, can cause clipping if object doesn't sit within space of near and far planes, OR depending on placement of view as dependent on zPos.
	//float zPos = -6.0f;	//zPos is translated backwards so as to not be inside of the objects we're trying to look at

	//if the camera gets translated, that effect needs to be inverted in order to be reflected by the view matrix
	//vector3 cameraPosition = vector3(2.5f, 0.0f, 10.0f);
	//mat4View = glm::translate(mat4View, cameraPosition); // cameraPosition replaced vector3(0.0f, 0.0f, zPos)
	*/

	mat4View = glm::inverse(mat4View);
	/*
	// Because the following observation holds true, and SHOULD be held true, 
	// we realized we have to invert whatever is fed into the view matrix (above) in order to get the desired true effect.
	//cameraPosition.x += 0.01f;	//causes object to appear to move to the right, but in reality the camera is moving to the left
	*/

	/*
	// The following was an accident, should've been rotating the model matrix (now above), but what it does is cool.
	// When rotating the view matrix and using the perspective projection matrix (below), the cone spins around in a weird way.
	mat4View = glm::rotate(mat4View, theta, vector3(1.0f, 1.0f, 0.0f));
	theta += 0.01f;
	*/


	return mat4View;
}

matrix4 MyCamera::GetProjectionMatrix()
{
	float width = (float)m_pSystem->GetWindowWidth();
	float height = (float)m_pSystem->GetWindowHeight();
	float ratio = width / height;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	//float fov = glm::radians(60.0f); // moved to MyCamera.h and renamed

	//auto mat4Projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, nearPlane, farPlane);
	auto mat4Projection = glm::perspective(m_FOV, ratio, nearPlane, farPlane);


	return mat4Projection;
}

void MyCamera::SetFOV(float fov) 
{
	fov = glm::radians(fov);

	if (fov > 170.0f)
		fov = 170.0f;

	else if (fov < 10.0f)
		fov = 10.0f;

	m_FOV = fov;
}

void MyCamera::RotateView(float deltaX, float deltaY)
{
	m_vEulerOrientation.x += deltaX;
	m_vEulerOrientation.y += deltaY;
}

void MyCamera::MoveForward(float forward, float sideways) 
{

}

