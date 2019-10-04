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

	glGenBuffers(1, &m_uVIO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uVIO);

	// Set up Shader
	InitShader();

	//// Set up triangle here (stub)	// MOVED TO FACTORY METHOD BELOW
	//InitTriangle();

	glBindVertexArray(0);	//unbind vertex array, so it can be used in different places
}

void MyMesh::InitShader()
{
	m_uShaderProgramId = LoadShaders("Shaders//BasicColor.vs", "Shaders//BasicColor.fs");
	//m_uShaderProgramId = LoadShaders("Shaders//Basic.vs", "Shaders//BasicColor.fs");

	glUseProgram(m_uShaderProgramId);
}

void MyMesh::InitTriangle()
{
	glBindVertexArray(m_uVAO);	// bind VAO

	// Consists of 3 vertices
	float triSize = 0.75f;

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



	// Fill Index Buffer (VIO)
	m_indices.push_back((unsigned short)0);
	m_indices.push_back((unsigned short)1);
	m_indices.push_back((unsigned short)2);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);


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
	m_vertices.push_back(vertexTopRightPoint);

	// Vertex 2 (top left)
	vertex vertexTopLeftPoint;
	vertexTopLeftPoint.position = glm::vec3(-quadSize, quadSize, 0.0f);
	vertexTopLeftPoint.color = glm::vec3(0.0f, 1.0f, 0.0f);	// green
	m_vertices.push_back(vertexTopLeftPoint);

	// Vertex 3 (bottom left)
	vertex vertexBottomLeftPoint;
	vertexBottomLeftPoint.position = glm::vec3(-quadSize, -quadSize, 0.0f);
	vertexBottomLeftPoint.color = glm::vec3(0.0f, 0.0f, 1.0f);	// blue
	m_vertices.push_back(vertexBottomLeftPoint);

	// Vertex 4 (bottom right)
	vertex vertexBottomRightPoint;
	vertexBottomRightPoint.position = glm::vec3(quadSize, -quadSize, 0.0f);
	vertexBottomRightPoint.color = glm::vec3(1.0f, 1.0f, 0.0f);	// yellow
	m_vertices.push_back(vertexBottomRightPoint);

	/*
	// No longer relevant now that we've switched to referring to VIO for rendering (vs just the VBO)
	// First Triangle		// Remember, Triangles rendered COUNTER CLOCK-WISE
	m_vertices.push_back(vertexTopRightPoint);
	m_vertices.push_back(vertexTopLeftPoint);
	m_vertices.push_back(vertexBottomLeftPoint);

	// Second Triangle		// For second tri: (botL, botR, topR) OR (topR, botL, botR) OR (botR, topR, botL)
	m_vertices.push_back(vertexBottomLeftPoint);
	m_vertices.push_back(vertexBottomRightPoint);
	m_vertices.push_back(vertexTopRightPoint);
	*/

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);	// first three vertices, for testing triangle rendering


	// Fill Index Buffer (VIO)	//how does it know to refer to the indices of objects stored in the vertex vector?
	//First triangle
	m_indices.push_back((unsigned short)0);
	m_indices.push_back((unsigned short)1);
	m_indices.push_back((unsigned short)2);
	//Second triangle
	m_indices.push_back((unsigned short)0);
	m_indices.push_back((unsigned short)2);
	m_indices.push_back((unsigned short)3);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);


	// Count the attributes
	int attributeCount = 2;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(1 * sizeof(glm::vec3)));

	glBindVertexArray(0);	//unbind VAO
}

void MyMesh::InitCube()
{
	glBindVertexArray(m_uVAO);	// bind VAO

	// Consists of 4 vertices
	float quadSize = 0.5f;
	unsigned short vertCount = 0;

	// FRONT VERTICES
	// Vertex 1 (top right front)
	vertex vertexTopRightFrontPoint;
	vertexTopRightFrontPoint.position = glm::vec3(quadSize, quadSize, quadSize);
	vertexTopRightFrontPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);	// red
	m_vertices.push_back(vertexTopRightFrontPoint);
	unsigned short vTRF = vertCount++;

	// Vertex 2 (top left front)
	vertex vertexTopLeftFrontPoint;
	vertexTopLeftFrontPoint.position = glm::vec3(-quadSize, quadSize, quadSize);
	vertexTopLeftFrontPoint.color = glm::vec3(0.0f, 1.0f, 0.0f);	// green
	m_vertices.push_back(vertexTopLeftFrontPoint);
	unsigned short vTLF = vertCount++;

	// Vertex 3 (bottom left front)
	vertex vertexBottomLeftFrontPoint;
	vertexBottomLeftFrontPoint.position = glm::vec3(-quadSize, -quadSize, quadSize);
	vertexBottomLeftFrontPoint.color = glm::vec3(0.0f, 0.0f, 1.0f);	// blue
	m_vertices.push_back(vertexBottomLeftFrontPoint);
	unsigned short vBLF = vertCount++;

	// Vertex 4 (bottom right front)
	vertex vertexBottomRightFrontPoint;
	vertexBottomRightFrontPoint.position = glm::vec3(quadSize, -quadSize, quadSize);
	vertexBottomRightFrontPoint.color = glm::vec3(1.0f, 1.0f, 0.0f);	// yellow
	m_vertices.push_back(vertexBottomRightFrontPoint);
	unsigned short vBRF = vertCount++;

	// BACK VERTICES
	// Vertex 5 (top right back)
	vertex vertexTopRightBackPoint;
	vertexTopRightBackPoint.position = glm::vec3(quadSize, quadSize, -quadSize);
	vertexTopRightBackPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);	// red
	m_vertices.push_back(vertexTopRightBackPoint);
	unsigned short vTRB = vertCount++;

	// Vertex 6 (top left back)
	vertex vertexTopLeftBackPoint;
	vertexTopLeftBackPoint.position = glm::vec3(-quadSize, quadSize, -quadSize);
	vertexTopLeftBackPoint.color = glm::vec3(0.0f, 1.0f, 0.0f);	// green
	m_vertices.push_back(vertexTopLeftBackPoint);
	unsigned short vTLB = vertCount++;

	// Vertex 7 (bottom left back)
	vertex vertexBottomLeftBackPoint;
	vertexBottomLeftBackPoint.position = glm::vec3(-quadSize, -quadSize, -quadSize);
	vertexBottomLeftBackPoint.color = glm::vec3(0.0f, 0.0f, 1.0f);	// blue
	m_vertices.push_back(vertexBottomLeftBackPoint);
	unsigned short vBLB = vertCount++;

	// Vertex 8 (bottom right back)
	vertex vertexBottomRightBackPoint;
	vertexBottomRightBackPoint.position = glm::vec3(quadSize, -quadSize, -quadSize);
	vertexBottomRightBackPoint.color = glm::vec3(1.0f, 1.0f, 0.0f);	// yellow
	m_vertices.push_back(vertexBottomRightBackPoint);
	unsigned short vBRB = vertCount++;

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);	//?? doesn't work w/o this line, but shouldnt it?

	// Fill Index Buffer (VIO)
	// 1 - FRONT FACE
	{
		//First triangle		// Remember, Triangles rendered COUNTER CLOCK-WISE
		m_indices.push_back(vTRF);
		m_indices.push_back(vTLF);
		m_indices.push_back(vBLF);
		//Second triangle
		m_indices.push_back(vBLF);
		m_indices.push_back(vBRF);
		m_indices.push_back(vTRF);

		/*
		// First Triangle		// Remember, Triangles rendered COUNTER CLOCK-WISE
		m_vertices.push_back(vertexTopRightFrontPoint);
		m_vertices.push_back(vertexTopLeftFrontPoint);
		m_vertices.push_back(vertexBottomLeftFrontPoint);
		// Second Triangle
		m_vertices.push_back(vertexBottomLeftFrontPoint);
		m_vertices.push_back(vertexBottomRightFrontPoint);
		m_vertices.push_back(vertexTopRightFrontPoint);
		*/
	}

	// 2 - BACK FACE
	{
		//First triangle
		m_indices.push_back(vTLB);
		m_indices.push_back(vTRB);
		m_indices.push_back(vBLB);
		//Second triangle
		m_indices.push_back(vBLB);
		m_indices.push_back(vTRB);
		m_indices.push_back(vBRB);

		/*
		// First Triangle
		m_vertices.push_back(vertexTopLeftBackPoint);
		m_vertices.push_back(vertexTopRightBackPoint);
		m_vertices.push_back(vertexBottomLeftBackPoint);
		// Second Triangle
		m_vertices.push_back(vertexBottomLeftBackPoint);
		m_vertices.push_back(vertexTopRightBackPoint);
		m_vertices.push_back(vertexBottomRightBackPoint);
		*/
	}
	
	// 3 - LEFT FACE
	{
		//First triangle
		m_indices.push_back(vTLF);
		m_indices.push_back(vTLB);
		m_indices.push_back(vBLB);
		//Second triangle
		m_indices.push_back(vBLB);
		m_indices.push_back(vBLF);
		m_indices.push_back(vTLF);

		/*
		// First Triangle
		m_vertices.push_back(vertexTopLeftFrontPoint);
		m_vertices.push_back(vertexTopLeftBackPoint);
		m_vertices.push_back(vertexBottomLeftBackPoint);
		// Second Triangle
		m_vertices.push_back(vertexBottomLeftBackPoint);
		m_vertices.push_back(vertexBottomLeftFrontPoint);
		m_vertices.push_back(vertexTopLeftFrontPoint);
		*/
	}

	// 4 - RIGHT FACE
	{
		//First triangle
		m_indices.push_back(vTRF);
		m_indices.push_back(vBRF);
		m_indices.push_back(vTRB);
		//Second triangle
		m_indices.push_back(vTRB);
		m_indices.push_back(vBRF);
		m_indices.push_back(vBRB);

		/*
		// First Triangle
		m_vertices.push_back(vertexTopRightFrontPoint);
		m_vertices.push_back(vertexBottomRightFrontPoint);
		m_vertices.push_back(vertexTopRightBackPoint);

		// Second Triangle
		m_vertices.push_back(vertexTopRightBackPoint);
		m_vertices.push_back(vertexBottomRightFrontPoint);
		m_vertices.push_back(vertexBottomRightBackPoint);
		*/
	}

	// 5 - TOP FACE
	{
		//First triangle
		m_indices.push_back(vTRB);
		m_indices.push_back(vTLB);
		m_indices.push_back(vTLF);
		//Second triangle
		m_indices.push_back(vTRB);
		m_indices.push_back(vTLF);
		m_indices.push_back(vTRF);

		/*
		// First Triangle
		m_vertices.push_back(vertexTopRightBackPoint);
		m_vertices.push_back(vertexTopLeftBackPoint);
		m_vertices.push_back(vertexTopLeftFrontPoint);
		// Second Triangle
		m_vertices.push_back(vertexTopRightBackPoint);
		m_vertices.push_back(vertexTopLeftFrontPoint);
		m_vertices.push_back(vertexTopRightFrontPoint);
		*/
	}
	
	// 6 - BOTTOM FACE
	{
		//First triangle
		m_indices.push_back(vBRF);
		m_indices.push_back(vBLF);
		m_indices.push_back(vBLB);
		//Second triangle
		m_indices.push_back(vBRF);
		m_indices.push_back(vBLB);
		m_indices.push_back(vBRB);

		/*
		// First Triangle
		m_vertices.push_back(vertexBottomRightFrontPoint);
		m_vertices.push_back(vertexBottomLeftFrontPoint);
		m_vertices.push_back(vertexBottomLeftBackPoint);
		// Second Triangle
		m_vertices.push_back(vertexBottomRightFrontPoint);
		m_vertices.push_back(vertexBottomLeftBackPoint);
		m_vertices.push_back(vertexBottomRightBackPoint);
		*/
	}
	
	////glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);	// first three vertices, for testing triangle rendering
	//glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);	//moved up, before filling VIO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);

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

	// View and Projection matrices	
	auto pCameraManager = CameraManager::GetInstance();
	glm::mat4 m4Projection = pCameraManager->GetProjectionMatrix();		// allows model to be sized based on window size, using lines above and below
	glm::mat4 m4View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 m4Model = glm::mat4(1.0f);

	/*
		Alternatively, could use the following for projection matrix

		float aspect = 1.8f; // place your screen aspect ratio here
		glm::mat4 m3Projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
		// OR
		glm::mat4 m3Projection = glm::perspectiveFOV(45.0f, 1920.0f, 1080.0f, 0.1f, 1000.0f);

	*/

	static float theta = 0.0f;
	theta += 0.01f;
	//m4Model = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 1.0f, 0.0f));
	m4Model = glm::rotate(m4Model, theta /*+ 0.51f * ((float)rand() / (float)RAND_MAX)*/, glm::vec3(0.0f, 1.0f, 0.0f));
	m4Model = glm::rotate(m4Model, theta /*+ 0.01f * ((float)rand() / (float)RAND_MAX)*/, glm::vec3(1.0f, 0.0f, 0.0f));

	// Read uniforms and send values
	GLuint MVP = glGetUniformLocation(m_uShaderProgramId, "transform");	// Model View Projection
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4Projection * m4View * m4Model));
	
	// Draw
	//glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());	//using plain VBO
	
	/*
	if (m_indices.size() > 0) 
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)(0));	//draw using VIO	//more efficient way of rendering
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());	//using plain VBO
	}
	*/

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)(0));	//draw using VIO	//more efficient way of rendering

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

MyMesh* MyMesh::MakeCube(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up cube here
	pMesh->InitCube();
	pMesh->SetScale(size);

	return pMesh;
}

MyMesh* MyMesh::Make(MyMesh::type meshType, float size)
{
	MyMesh* pMesh = nullptr;

	switch (meshType)
	{
		case MyMesh::type::TRIANGLE:
		{
			pMesh = MyMesh::MakeTriangle(size);
		} break;
		case MyMesh::type::QUAD:
		{
			pMesh = MyMesh::MakeQuad(size);

		} break;
		case MyMesh::type::CUBE:
		{
			pMesh = MyMesh::MakeCube(size);

		} break;
		case MyMesh::type::INVALID:
		default:
		{
			pMesh = nullptr;
			// Error!
		} break;
	}

	return pMesh;
}