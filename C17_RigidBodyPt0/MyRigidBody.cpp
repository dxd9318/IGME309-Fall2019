#include "MyRigidBody.h"
using namespace Simplex;

//Accessors
bool MyRigidBody::GetVisible(void) { return m_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColor(void) { return m_v3Color; }
void MyRigidBody::SetColor(vector3 a_v3Color) { m_v3Color = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }

enum BOX_POINT 
{
	TLF,
	TRF,
	TLB,
	TRB,
	BLF,
	BRF,
	BLB,
	BRB,
	INVALID
};

void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix) 
{ 
	m_m4ToWorld = a_m4ModelMatrix; 
	m_v3CenterG = GetCenterGlobal();
	m_v3MinG = vector3(m_m4ToWorld * vector4(m_v3MinL, 1.0f));
	m_v3MaxG = vector3(m_m4ToWorld * vector4(m_v3MaxL, 1.0f));

	// We now have a half-vector, and model matrix. Use both to determine axis realigned bounding box
	//vector3 vHVOriented = matrix3(m_m4ToWorld) * m_v3HalfWidth;

	m_v3MaxLR = vector3(0.0f);
	m_v3MinLR = vector3(0.0f);

	for (BOX_POINT i = TLF; i < INVALID; i = (BOX_POINT)((int)(i) + 1)) 
	{
		vector3 ptBox = vector3(0.0f);
		
		switch (i) 
		{
			case TLF: ptBox += vector3(-m_v3HalfWidth.x, m_v3HalfWidth.y, -m_v3HalfWidth.z); break;
			case TRF: ptBox += vector3(m_v3HalfWidth.x, m_v3HalfWidth.y, -m_v3HalfWidth.z); break;
			case TLB: ptBox += vector3(-m_v3HalfWidth.x, m_v3HalfWidth.y, m_v3HalfWidth.z); break;
			case TRB: ptBox += vector3(m_v3HalfWidth.x, m_v3HalfWidth.y, m_v3HalfWidth.z); break;		//

			case BLF: ptBox += vector3(-m_v3HalfWidth.x, -m_v3HalfWidth.y, -m_v3HalfWidth.z); break;	//
			case BRF: ptBox += vector3(m_v3HalfWidth.x, -m_v3HalfWidth.y, -m_v3HalfWidth.z); break;
			case BLB: ptBox += vector3(-m_v3HalfWidth.x, -m_v3HalfWidth.y, m_v3HalfWidth.z); break;
			case BRB: ptBox += vector3(m_v3HalfWidth.x, -m_v3HalfWidth.y, m_v3HalfWidth.z); break;
		}

		ptBox = matrix3(m_m4ToWorld) * ptBox;
		
		if (ptBox.x < m_v3MinLR.x)
			m_v3MinLR.x = ptBox.x;
		if (ptBox.x > m_v3MaxLR.x)
			m_v3MaxLR.x = ptBox.x;

		if (ptBox.y < m_v3MinLR.y)
			m_v3MinLR.y = ptBox.y;
		if (ptBox.y > m_v3MaxLR.y)
			m_v3MaxLR.y = ptBox.y;

		if (ptBox.z < m_v3MinLR.z)
			m_v3MinLR.z = ptBox.z;
		if (ptBox.z > m_v3MaxLR.z)
			m_v3MaxLR.z = ptBox.z;
	}

	m_v3HalfWidthR = (m_v3MaxLR - m_v3MinLR) * 0.5;
	m_v3CenterR = m_v3CenterG;

	// Update the max/min with the global center
	m_v3MaxGR = m_v3MaxLR + m_v3CenterG;
	m_v3MinGR = m_v3MinLR + m_v3CenterG;


}

//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisible = true;

	m_fRadius = 0.0f;

	m_v3Color = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& other)
{
	std::swap(m_pMeshMngr , other.m_pMeshMngr);
	std::swap(m_bVisible , other.m_bVisible);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_v3Color , other.m_v3Color);

	std::swap(m_v3Center , other.m_v3Center);
	std::swap(m_v3MinL , other.m_v3MinL);
	std::swap(m_v3MaxL , other.m_v3MaxL);

	std::swap(m_v3MinG , other.m_v3MinG);
	std::swap(m_v3MaxG , other.m_v3MaxG);

	std::swap(m_v3HalfWidth , other.m_v3HalfWidth);

	std::swap(m_m4ToWorld , other.m_m4ToWorld);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();

	m_v3MinL = m_v3MaxL = a_pointList[0];

	// find rigid body's max and min points
	for (const vector3& pt : a_pointList) 
	{
		if (pt.x < m_v3MinL.x)
			m_v3MinL.x = pt.x;
		if (pt.x > m_v3MaxL.x)
			m_v3MaxL.x = pt.x;
		
		if (pt.y < m_v3MinL.y)
			m_v3MinL.y = pt.y;
		if (pt.y > m_v3MaxL.y)
			m_v3MaxL.y = pt.y;
		
		if (pt.z < m_v3MinL.z)
			m_v3MinL.z = pt.z;
		if (pt.z > m_v3MaxL.z)
			m_v3MaxL.z = pt.z;
	}

	// calculate rb's center, and rb sphere's radius using that center
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) * 0.5;	// distance(and direction) btwn two points, divided in half
	m_v3Center = m_v3HalfWidth + m_v3MinL;			// add this distance(and direction) back to the min point to find center point

	m_fRadius = 0.0f;
	m_fRadius2 = 0.0f;

	// Find the radius of bounding sphere (find distance btwn center and pt farthest from center)
	/* //Method 1
	for (const vector3& pt : a_pointList) 
	{
		float temp;
		if ((temp = glm::distance(pt, m_v3Center)) > m_fRadius)		//costly operation on cpu because uses sqroot
			m_fRadius = temp;
	}
	*/

	/*	//Method 2
	for (const vector3& pt : a_pointList)
	{
		float temp;
		if ((temp = glm::distance2(pt, m_v3Center)) > m_fRadius2)		//distance2 uses squared values to save cpu calculations
			m_fRadius2 = temp;
	}

	m_fRadius = sqrt(m_fRadius2);
	*/

	// Method 3 (just using halfwidths)
	m_fRadius = glm::length(m_v3HalfWidth);
	m_fRadius2 = m_fRadius * m_fRadius;

	int a = 6;
}
MyRigidBody::MyRigidBody(MyRigidBody const& other)
{
	m_pMeshMngr = m_pMeshMngr;
	m_bVisible = m_bVisible;

	m_fRadius = m_fRadius;

	m_v3Color = m_v3Color;

	m_v3Center = m_v3Center;
	m_v3MinL = m_v3MinL;
	m_v3MaxL = m_v3MaxL;

	m_v3MinG = m_v3MinG;
	m_v3MaxG = m_v3MaxG;

	m_v3HalfWidth = m_v3HalfWidth;

	m_m4ToWorld = m_m4ToWorld;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyRigidBody temp(other);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody(){Release();};

//--- Non Standard Singleton Methods
void MyRigidBody::AddToRenderList(void)
{
	if (!m_bVisible)
		return;

	m_pMeshMngr->AddWireSphereToRenderList(glm::scale(glm::translate(m_m4ToWorld, m_v3Center), vector3(m_fRadius)), m_v3Color, RENDER_WIRE);

	// AABB
	m_pMeshMngr->AddWireCubeToRenderList(glm::scale(glm::translate(m_m4ToWorld, m_v3Center), m_v3HalfWidth * 2.0f), C_YELLOW, RENDER_SOLID);

	// ARBB
	m_pMeshMngr->AddWireCubeToRenderList(glm::scale(glm::translate(IDENTITY_M4, m_v3CenterR), m_v3HalfWidthR * 2.0f), C_ORANGE, RENDER_SOLID);

}
bool MyRigidBody::IsColliding(MyRigidBody* const other)
{
	//This method is called every frame, so it's important to consider ways to optimize it.

	/*	//This method doesn't work because you're trying to compare local centers, instead of these models' respective centers across global space.
	if (glm::distance(m_v3Center, other->m_v3Center) < (m_fRadius + other->m_fRadius))
		return true;
	*/

	/*	//gets center using global coordinates, compares distance using converted center data.
	if (glm::distance(GetCenterGlobal(), other->GetCenterGlobal()) < (m_fRadius + other->m_fRadius))
		return true;

	return false;
	*/

	if (glm::distance(m_v3CenterG, other->m_v3CenterG) < (m_fRadius + other->m_fRadius)) 
	{
		// check bounding boxes

		// check the bounds of the AABB
		/*if (m_v3MaxG.x < other->m_v3MinG.x)
			return false;
		else if (m_v3MinG.x > other->m_v3MaxG.x)
			return false;

		if (m_v3MaxG.y < other->m_v3MinG.y)
			return false;
		else if (m_v3MinG.y > other->m_v3MaxG.y)
			return false;

		if (m_v3MaxG.z < other->m_v3MinG.z)
			return false;
		else if (m_v3MinG.z > other->m_v3MaxG.z)
			return false;*/

		// check the bounds of the ARBB
		if (m_v3MaxGR.x < other->m_v3MinGR.x)
			return false;
		else if (m_v3MinGR.x > other->m_v3MaxGR.x)
			return false;

		if (m_v3MaxGR.y < other->m_v3MinGR.y)
			return false;
		else if (m_v3MinGR.y > other->m_v3MaxGR.y)
			return false;

		if (m_v3MaxGR.z < other->m_v3MinGR.z)
			return false;
		else if (m_v3MinGR.z > other->m_v3MaxGR.z)
			return false;

		return true;
	}

	return false;
}