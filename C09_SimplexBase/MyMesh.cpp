#include "MyMesh.h"

MyMesh::MyMesh() 
{
	
}

MyMesh::~MyMesh()
{
	glDeleteShader(m_uShaderProgramId);
	glDeleteBuffers(1, &m_uVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_uVAO);
}

void MyMesh::Init()
{
	glGenVertexArrays(1, &m_uVAO);
	glGenBuffers(1, &m_uVBO);

	glBindVertexArray(m_uVAO);				//This indicates that we're now using the VAO,
	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);	// and now using the VAO

	// Set up Shader
	InitShader();

	//// Set up triangle here (stub)	// MOVED TO FACTORY METHOD BELOW
	//InitTriangle();

	glBindVertexArray(0);	//unbind vertex array, so it can be used in different places
}

void MyMesh::InitShader()
{
	m_uShaderProgramId = LoadShaders("Shaders//BasicColor.vs", "Shaders//BasicColor.fs");
	//m_uShaderProgramId = LoadShaders("Shaders//BasicColor.vs", "Shaders//BasicColor.fs");

	glUseProgram(m_uShaderProgramId);
}

void MyMesh::InitTriangle()
{
	glBindVertexArray(m_uVAO);	// bind VAO

	// Consists of 3 vertices
	float triSize = 0.5f;

	// Vertex 1 (top point)
	vertex vertexTopPoint;
	vertexTopPoint.position = glm::vec3(0.0f, triSize, 0.0f);
	vertexTopPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);	// red
	m_vertices.push_back(vertexTopPoint);

	// Vertex 2 (left bottom)
	vertex vertexLeftBottomPoint;
	vertexLeftBottomPoint.position = glm::vec3(-triSize, -triSize, 0.0f);
	vertexLeftBottomPoint.color = glm::vec3(0.0f, 1.0f, 0.0f);	// green
	m_vertices.push_back(vertexLeftBottomPoint);

	// Vertex 3 (right bottom)
	vertex vertexRightBottomPoint;
	vertexRightBottomPoint.position = glm::vec3(triSize, -triSize, 0.0f);
	vertexRightBottomPoint.color = glm::vec3(0.0f, 0.0f, 1.0f);	// blue
	m_vertices.push_back(vertexRightBottomPoint);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

	// Count the attributes
	int attributeCount = 2;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(1* sizeof(glm::vec3)));

	glBindVertexArray(0);	//unbind VAO
}

void MyMesh::InitQuad()
{
	glBindVertexArray(m_uVAO);	// bind VAO

	// Consists of 4 vertices
	float quadSize = 0.5f;

	// Vertex 1 (top right)
	vertex vertexTopRightPoint;
	vertexTopRightPoint.position = glm::vec3(quadSize, quadSize, 0.0f);
	vertexTopRightPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);	// red

	// Vertex 2 (top left)
	vertex vertexTopLeftPoint;
	vertexTopLeftPoint.position = glm::vec3(-quadSize, quadSize, 0.0f);
	vertexTopLeftPoint.color = glm::vec3(0.0f, 1.0f, 0.0f);	// green

	// Vertex 3 (bottom left)
	vertex vertexBottomLeftPoint;
	vertexBottomLeftPoint.position = glm::vec3(-quadSize, -quadSize, 0.0f);
	vertexBottomLeftPoint.color = glm::vec3(0.0f, 0.0f, 1.0f);	// blue

	// Vertex 4 (bottom right)
	vertex vertexBottomRightPoint;
	vertexBottomRightPoint.position = glm::vec3(quadSize, -quadSize, 0.0f);
	vertexBottomRightPoint.color = glm::vec3(1.0f, 1.0f, 0.0f);	// yellow


	// First Triangle		// Remember, Triangles rendered COUNTER CLOCK-WISE
	m_vertices.push_back(vertexTopRightPoint);
	m_vertices.push_back(vertexTopLeftPoint);
	m_vertices.push_back(vertexBottomLeftPoint);

	// Second Triangle		// For second tri: (botL, botR, topR) OR (topR, botL, botR) OR (botR, topR, botL)
	m_vertices.push_back(vertexBottomLeftPoint);
	m_vertices.push_back(vertexBottomRightPoint);
	m_vertices.push_back(vertexTopRightPoint);


	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);	// first three vertices, for testing triangle rendering


	// Count the attributes
	int attributeCount = 2;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(1 * sizeof(glm::vec3)));

	glBindVertexArray(0);	//unbind VAO
}

void MyMesh::Draw()
{
	glBindVertexArray(m_uVAO);	//bind

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);		//unbind
}

void MyMesh::SetScale(float size)
{
	m_scale = size;
}

// Factory Methods
MyMesh* MyMesh::MakeTriangle(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up triangle here
	pMesh->InitTriangle();
	pMesh->SetScale(size);

	return pMesh;
}

MyMesh* MyMesh::MakeQuad(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up quad here
	pMesh->InitQuad();
	pMesh->SetScale(size);

	return pMesh;
}