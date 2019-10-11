#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// -------------------------------
	vector3 v3Center = vector3(0.0f, 0.0f, 0.0f);
	float foundAngle = 360.0f / a_nSubdivisions;
	float foundAngleRads = glm::radians(foundAngle);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Circle Base		// Careful, circle is only rendered on one side!
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z - (a_fHeight / 2.0f)),	// point A (center of circle)	// height modified to center primitive around it center
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, -(a_fHeight / 2.0f)),	// point C
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, -(a_fHeight / 2.0f)));	// point B	// Switched points B and C so this circle renders on outside of primitive

		// Cone Tip
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z + (a_fHeight / 2.0f)),	// point D (tip)
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, -(a_fHeight / 2.0f)),	// point E
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, -(a_fHeight / 2.0f)));	// point F
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// -------------------------------
	// Circle Base		// careful, circle is only rendered on one side!
	vector3 v3Center = vector3(0.0f, 0.0f, 0.0f);
	float foundAngle = 360.0f / a_nSubdivisions;
	float foundAngleRads = glm::radians(foundAngle);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Circle bottom
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z - (a_fHeight / 2.0f)),	//point A (center of circle)
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, -(a_fHeight / 2.0f)),	//point C
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, -(a_fHeight / 2.0f)));	//point B

		// Circle top
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z + (a_fHeight / 2.0f)),	//point D (center of circle)
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, (a_fHeight / 2.0f)),	//point E
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, (a_fHeight / 2.0f)));	//point F

		// Side panels
		AddQuad(
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, -(a_fHeight / 2.0f)),				// B		// First Tri B->C->E
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, -(a_fHeight / 2.0f)),	// C
			vector3(cos(foundAngleRads * i) * a_fRadius, sin(foundAngleRads * i) * a_fRadius, (a_fHeight / 2.0f)),				// E		// Second Tri E->C->F
			vector3(cos(foundAngleRads * (i + 1)) * a_fRadius, sin(foundAngleRads * (i + 1)) * a_fRadius, (a_fHeight / 2.0f)));	// F
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// -------------------------------
	// Circle Base		// careful, circle is only rendered on one side!
	vector3 v3Center = vector3(0.0f, 0.0f, 0.0f);
	float foundAngle = 360.0f / a_nSubdivisions;
	float foundAngleRads = glm::radians(foundAngle);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Ring bottom		// Inner side then outer side
		AddQuad(
			vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, -(a_fHeight / 2.0f)),				// B1		// First Tri B1->C1->B2
			vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, -(a_fHeight / 2.0f)),	// C1
			vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, -(a_fHeight / 2.0f)),				// B2		// First Tri B2->C1->C2
			vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, -(a_fHeight / 2.0f)));	// C2

		// Ring top			// Outer side then inner side
		AddQuad(
			vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, (a_fHeight / 2.0f)),				// B3		// First Tri B3->C3->B4
			vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, (a_fHeight / 2.0f)),	// C3
			vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, (a_fHeight / 2.0f)),				// B4		// First Tri B4->C3->C4
			vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, (a_fHeight / 2.0f)));	// C4

		// Outside panels	// Bottom to top
		AddQuad(
			vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, -(a_fHeight / 2.0f)),				// B		// First Tri B->C->E
			vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, -(a_fHeight / 2.0f)),	// C
			vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, (a_fHeight / 2.0f)),				// E		// Second Tri E->C->F
			vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, (a_fHeight / 2.0f)));	// F

		// Inside panels	// Bottom to top	// ORDER OF POINTS INVERTED FROM OUTSIDE
		AddQuad(
			vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, -(a_fHeight / 2.0f)),	// C
			vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, -(a_fHeight / 2.0f)),				// B		// First Tri C->B->F
			vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, (a_fHeight / 2.0f)),	// F
			vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, (a_fHeight / 2.0f)));				// E		// Second Tri F->B->E
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);

	/*
	vector3 v3Center = vector3(0.0f, 0.0f, 0.0f);
	float foundAngle = 360.0f / a_nSubdivisionsA;
	float foundAngleRads = glm::radians(foundAngle);
	for (int j = 0; j < a_nSubdivisionsB; j++)
	{
		for (int i = 0; i < a_nSubdivisionsA; i++)
		{
			//a_nSubdivisionsA determines number of segments in a ring
			//a_nSubdivisionsB determines number of rings
			// Ring bottom		// Inner side then outer side
			AddQuad(
				vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, -(a_fHeight / 2.0f)),				// B1		// First Tri B1->C1->B2
				vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, -(a_fHeight / 2.0f)),	// C1
				vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, -(a_fHeight / 2.0f)),				// B2		// First Tri B2->C1->C2
				vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, -(a_fHeight / 2.0f)));	// C2
			// Ring top			// Outer side then inner side
			AddQuad(
				vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, (a_fHeight / 2.0f)),				// B3		// First Tri B3->C3->B4
				vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, (a_fHeight / 2.0f)),	// C3
				vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, (a_fHeight / 2.0f)),				// B4		// First Tri B4->C3->C4
				vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, (a_fHeight / 2.0f)));	// C4
			// Outside panels	// Bottom to top
			AddQuad(
				vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, -(a_fHeight / 2.0f)),				// B		// First Tri B->C->E
				vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, -(a_fHeight / 2.0f)),	// C
				vector3(cos(foundAngleRads * i) * a_fOuterRadius, sin(foundAngleRads * i) * a_fOuterRadius, (a_fHeight / 2.0f)),				// E		// Second Tri E->C->F
				vector3(cos(foundAngleRads * (i + 1)) * a_fOuterRadius, sin(foundAngleRads * (i + 1)) * a_fOuterRadius, (a_fHeight / 2.0f)));	// F
			// Inside panels	// Bottom to top	// ORDER OF POINTS INVERTED FROM OUTSIDE
			AddQuad(
				vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, -(a_fHeight / 2.0f)),	// C
				vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, -(a_fHeight / 2.0f)),				// B		// First Tri C->B->F
				vector3(cos(foundAngleRads * (i + 1)) * a_fInnerRadius, sin(foundAngleRads * (i + 1)) * a_fInnerRadius, (a_fHeight / 2.0f)),	// F
				vector3(cos(foundAngleRads * i) * a_fInnerRadius, sin(foundAngleRads * i) * a_fInnerRadius, (a_fHeight / 2.0f)));				// E		// Second Tri F->B->E
		}
	}*/

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// -------------------------------
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);

	vector3 v3Center = vector3(0.0f, 0.0f, 0.0f);
	float foundTheta = 180.0f / a_nSubdivisions;		//determine vertical angle, which should only swing between up and down, not all the way around, hence 180 degrees.
	float foundThetaRads = glm::radians(foundTheta);
	float foundPhi = 360.0f / a_nSubdivisions;			//determines horizontal angle
	float foundPhiRads = glm::radians(foundPhi);

	/*
	// Issue with this method is the top and bottom circles will be flat, not what I want.
	// Need to find a way to get the angle for x, y, and z coordinates. How to use trig to represent a point on a sphere?
		// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/spherical-coordinates-and-trigonometric-functions
		// Turns out I need to calculate not one but two angles, one for the vertical plane and one for the horizontal plane.
		// theta = vertical angle
		// phi = horizontal angle
		// x = r * cos(phi)sin(theta)//z		// y = r * sin(phi)sin(theta)//x		// z = r * cos(theta)//y		
			//example's coord system doesn't match the one we're using for opengl
	*/

	//handles incrementing across "latitudes" of sphere, ie.left-right rendering
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		/*
		// Originally commented out the top and bottom circles code I had for this function so I can see what was going on with the rings,
		// but then I realized the rings code is creating top and bottom points as well, so I don't actually need these circles.
		// Top circle
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z + a_fRadius),	// point A (center of circle)	//
			vector3(cos(foundPhiRads * i) * a_fRadius, sin(foundPhiRads * i) * a_fRadius, a_fRadius),	// point B	// Switched points B and C so this circle renders on outside of primitive
			vector3(cos(foundPhiRads * (i + 1)) * a_fRadius, sin(foundPhiRads * (i + 1)) * a_fRadius, a_fRadius));	// point C
		*/

		// Multiple rings, as many as there are a_nSubdivisions. Created one column per loop at a time, connecting the triangles of top and bottom circles
		// Similar to cylinder, where I made a side panel that connects the triangles of the top and bottom circles
				// except this side "panel" is in segments (from top to bottom), and each segment is angled slightly differently	
				// this angling is by theta, so (theta * j)
					//but the trouble here is there isn't just one coordinate that changes every time.

		//handles incrementing down "longitudes" of sphere, ie. up-down rendering	//needs to be done one hemisphere at a time or it'll invert (render the inside), not what I want
		for (int j = 0; j < (a_nSubdivisions / 2); j++)	//(int j = (a_nSubdivisions / 2); j > 0; j--)	
		{
			 //Currently BADC
				//CD
				//AB

			AddQuad(
				

				vector3(cos(foundPhiRads * (i + 1)) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundPhiRads * (i + 1)) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundThetaRads * j) * a_fRadius),	//quad ptB

				vector3(cos(foundPhiRads * i) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundPhiRads * i) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundThetaRads * j) * a_fRadius),	//quad ptA

				vector3(cos(foundPhiRads * (i + 1)) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundPhiRads * (i + 1)) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundThetaRads * (j - 1)) * a_fRadius),		//quad ptD

				vector3(cos(foundPhiRads * i) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundPhiRads * i) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundThetaRads * (j - 1)) * a_fRadius)		//quad ptC

			);
		}


		//handles incrementing down "longitudes" of sphere, ie. up-down rendering	//needs to be done one hemisphere at a time or it'll invert (render the inside), not what I want
		for (int j = (a_nSubdivisions / 2); j < (a_nSubdivisions); j++)	//(int j = (a_nSubdivisions / 2); j > 0; j--)	
		{
			//Currently BADC
			   //CD
			   //AB

			AddQuad(


				vector3(cos(foundPhiRads * (i + 1)) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundPhiRads * (i + 1)) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundThetaRads * j) * a_fRadius),	//quad ptB

				vector3(cos(foundPhiRads * i) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundPhiRads * i) * sin(foundThetaRads * j) * a_fRadius,
					sin(foundThetaRads * j) * a_fRadius),	//quad ptA

				vector3(cos(foundPhiRads * (i + 1)) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundPhiRads * (i + 1)) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundThetaRads * (j - 1)) * a_fRadius),		//quad ptD

				vector3(cos(foundPhiRads * i) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundPhiRads * i) * sin(foundThetaRads * (j + 1)) * a_fRadius,
					sin(foundThetaRads * (j - 1)) * a_fRadius)		//quad ptC

			);
		}

		/*
		// Originally commented out the top and bottom circles code I had for this function so I can see what was going on with the rings,
		// but then I realized the rings code is creating top and bottom points as well, so I don't actually need these circles.
		// Bottom circle
		AddTri(
			vector3(v3Center.x, v3Center.y, v3Center.z - a_fRadius),	// point D (center of circle)	//
			vector3(cos(foundPhiRads * (i + 1)) * a_fRadius, sin(foundPhiRads * (i + 1)) * a_fRadius, -a_fRadius),	// point E
			vector3(cos(foundPhiRads * i) * a_fRadius, sin(foundPhiRads * i) * a_fRadius, -a_fRadius));	// point F
		*/
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}