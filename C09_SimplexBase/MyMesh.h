#ifndef MY_MESH_H_
#define MY_MESH_H_

#include "Definitions.h"	// Needed for GLuint data type used below
#include <vector>
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\quaternion.hpp>

struct vertex 
{
	glm::vec3 position;
	glm::vec3 color;
};

class MyMesh 
{
public:
	typedef enum class TYPES
	{
		TRIANGLE,
		QUAD,
		CUBE,
		INVALID
	} type;

public:
	MyMesh();
	~MyMesh();

	void Init();
	void InitShader();
	void InitTriangle();
	void InitQuad();
	void InitCube();

	void Draw();
	void SetScale(float size);

	static MyMesh* Make(MyMesh::type meshType, float size);

private:
	static MyMesh* MakeTriangle(float size);
	static MyMesh* MakeQuad(float size);
	static MyMesh* MakeCube(float size);

private:
	std::vector<vertex> m_vertices;
	std::vector<unsigned short> m_indices;	//nvm the following	//vector of unsigned 16-bit integers	//glm has this bc generally you don't have more than 65000 vertices in an object (max value of 16 bit)
	GLuint m_uShaderProgramId = 0;
	GLuint m_uVAO = 0;
	GLuint m_uVBO = 0;
	GLuint m_uVIO = 0;	//index buffer

	float m_scale = 1.0f;
};

#endif // ! MY_MESH_H_
