#ifndef MY_CAMERA_H_
#define MY_CAMERA_H_

// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\quaternion.hpp>

typedef glm::vec3 vector3;
typedef glm::vec4 vector4;
typedef glm::mat3 matrix3;
typedef glm::mat4 matrix4;

#include "Simplex\Simplex.h";

class MyCamera 
{
public:
	MyCamera(Simplex::SystemSingleton* pSystem, vector3 ptOrigin, vector3 vEulerOrientation = vector3(0.0f));
	~MyCamera();

	matrix4 GetViewMatrix();
	matrix4 GetProjectionMatrix();
	void SetFOV(float fov);
	void RotateView(float deltaX, float deltaY);
	void MoveForward(float forward, float sideways);

private:
	vector3 m_ptOrigin;
	vector3 m_vEulerOrientation = vector3(0.0f, 0.0f, 0.0f);

	float m_FOV = glm::radians(60.0f);


	Simplex::SystemSingleton* m_pSystem = nullptr;
};

#endif
