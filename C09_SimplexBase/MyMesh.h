#ifndef MY_MESH_H_
#define MY_MESH_H_

#include <vector>
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\quaternion.hpp>

#include <GL\glew.h>


class MyMesh {
public:
	MyMesh();
	~MyMesh();

	std::vector<glm::vec3> m_vertices;

	void InitShader();
	void InitTriangle();

	void Draw();

private:
	GLuint m_uShaderProgramId = 0;
	GLuint m_uVAO = 0;
	GLuint m_uVBO = 0;

};

#endif // ! MY_MESH_H_
