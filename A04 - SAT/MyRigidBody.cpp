#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = true;	//at the moment shows colliding BSs count as true collision
	m_bVisibleOBB = true;
	m_bVisibleARBB = true;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	//vector3 v3Corner[8];	//moved to header to be easily accessible later
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		if(SAT(a_pOther) != eSATResults::SAT_NONE)	//if result finds a plane of separation
			bColliding = false;// reset to false
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	/*
	Your code goes here instead of this comment;

	For this method, if there is an axis that separates the two objects
	then the return will be different than 0; 1 for any separating axis
	is ok if you are not going for the extra credit, if you could not
	find a separating axis you need to return 0, there is an enum in
	Simplex that might help you [eSATResults] feel free to use it.
	(eSATResults::SAT_NONE has a value of 0)
	*/

#pragma region First_Round_Dot_Product

	// get the 8 points of both OBBs	// already done, just reference using v3Corner[#]
	/* // TAKEN FROM SetModelMatrix METHOD ABOVE
	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square	//in this project, Z increases towards the camera.
	v3Corner[0] = m_v3MinL;										//BLB
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);	//BRB
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);	//TLB
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);	//TRB

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);	//BLF
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);	//BRF
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);	//TLF
	v3Corner[7] = m_v3MaxL;										//TRF
	*/

	// 1ST ROUND - dot product checks
	// using the 8 points of an OBB, get the XYZ axes of each box
	// for x, y, and z, normalize the vector resulting from subtracting one corner from another
	vector3 xAxisA = glm::normalize(v3Corner[0] - v3Corner[1]); // BLB - BRB
	vector3 yAxisA = glm::normalize(v3Corner[0] - v3Corner[2]); // BLB - TLB
	vector3 zAxisA = glm::normalize(v3Corner[0] - v3Corner[4]); // BLB - BLF

	vector3 xAxisB = glm::normalize(a_pOther->v3Corner[0] - a_pOther->v3Corner[1]); // BLB - BRB
	vector3 yAxisB = glm::normalize(a_pOther->v3Corner[0] - a_pOther->v3Corner[2]); // BLB - TLB
	vector3 zAxisB = glm::normalize(a_pOther->v3Corner[0] - a_pOther->v3Corner[4]); // BLB - BLF

	// find min and max values for each axis of both boxes
	// since these values will vary depending on how a OBB is oriented, use dot product of found XYZ axes against each point of the OBB.
	float xMinA = 900.0f, yMinA = 900.0f, zMinA = 900.0f, xMinB = 900.0f, yMinB = 900.0f, zMinB = 900.0f;	// temp min values to be replaced through testing
	float xMaxA =900.0f, yMaxA = -900.0f, zMaxA = -900.0f, xMaxB = -900.0f, yMaxB = -900.0f, zMaxB = -900.0f; // temp max values to be replaced through testing
	float temp = 0; // temp value to test against mins and maxs
	float temp2 = 0; //RENAME THIS AND ABOVE TEMP

	// find mins and maxs of both OBBs against first OBB's axes
	for (int i = 0; i < 8; i++)
	{ 
		// check min and max values of first box against first box's X axis
		temp = glm::dot(xAxisA, v3Corner[i]);	
		if (temp < xMinA) xMinA = temp;	// to find point closest to axis
		if (temp > xMaxA) xMaxA = temp; // to find point farthest from axis

		// check min and max values of second box against first box's X axis
		temp2 = glm::dot(xAxisA, a_pOther->v3Corner[i]);	
		if (temp2 < xMinB) xMinB = temp2;	// to find point closest to axis
		if (temp2 > xMaxB) xMaxB = temp2; // to find point farthest from axis
	}
	for (int i = 0; i < 8; i++)
	{
		// check min and max values of first box against first box's Y axis
		temp = glm::dot(yAxisA, v3Corner[i]);
		if (temp < yMinA) yMinA = temp;
		if (temp > yMaxA) yMaxA = temp;

		// check min and max values of second box against first box's Y axis
		temp2 = glm::dot(yAxisA, a_pOther->v3Corner[i]);
		if (temp2 < yMinB) yMinB = temp2;
		if (temp2 > yMaxB) yMaxB = temp2;
	}
	for (int i = 0; i < 8; i++)
	{
		// check min and max values of first box against first box's Z axis
		temp = glm::dot(zAxisA, v3Corner[i]);
		if (temp < zMinA) zMinA = temp;
		if (temp > zMaxA) zMaxA = temp;

		// check min and max values of second box against first box's Z axis
		temp2 = glm::dot(zAxisA, a_pOther->v3Corner[i]);
		if (temp2 < zMinB) zMinB = temp2;
		if (temp2 > zMaxB) zMaxB = temp2;
	}

	// find mins and maxs of both OBBs against second OBB's axes
	for (int i = 0; i < 8; i++)
	{
		// check min and max values of second box against second box's X axis
		temp = glm::dot(xAxisB, a_pOther->v3Corner[i]);
		if (temp < xMinB) xMinB = temp;
		if (temp > xMaxB) xMaxB = temp;

		// check min and max values of first box against second box's X axis
		temp2 = glm::dot(xAxisB, v3Corner[i]);
		if (temp2 < xMinA) xMinA = temp2;
		if (temp2 > xMaxA) xMaxA = temp2;
	}
	for (int i = 0; i < 8; i++)
	{
		// check min and max values of second box against second box's Y axis
		temp = glm::dot(yAxisB, a_pOther->v3Corner[i]);
		if (temp < yMinB) yMinB = temp;
		if (temp > yMaxB) yMaxB = temp;

		// check min and max values of first box against second box's Y axis
		temp2 = glm::dot(yAxisB, v3Corner[i]);
		if (temp2 < yMinA) yMinA = temp2;
		if (temp2 > yMaxA) yMaxA = temp2;
	}
	for (int i = 0; i < 8; i++)
	{
		// check min and max values of second box against second box's Z axis
		temp = glm::dot(zAxisB, a_pOther->v3Corner[i]);
		if (temp < zMinB) zMinB = temp;
		if (temp > zMaxB) zMaxB = temp;

		// check min and max values of first box against second box's Z axis
		temp2 = glm::dot(zAxisB, v3Corner[i]);
		if (temp2 < zMinA) zMinA = temp2;
		if (temp2 > zMaxA) zMaxA = temp2;
	}

	// use min and max values to check for overlap
	// compare minx maxx of obb a to minx maxx of obb b
	// if no overlap -> early out

	// return value of 1 (or any not zero) means an axis of separation (ie. no collision) could be found.	//these might be backwards
	if (xMinA > xMaxB) return eSATResults::SAT_AX;
	if (xMaxA < xMinB) return eSATResults::SAT_BX;
	if (yMinA > yMaxB) return eSATResults::SAT_AY;
	if (yMaxA < yMinB) return eSATResults::SAT_BY;
	if (zMinA > zMaxB) return eSATResults::SAT_AZ;
	if (zMaxA < zMinB) return eSATResults::SAT_BZ;
#pragma endregion

#pragma region Second_Round_Cross_Product
	// 2ND ROUND - cross product checks
	// use it to find a plane (represented as a vector) between the x axis of boxA and each axis of box B. // repeat with y and z axes of A against all axes of B
	vector3 xAxBPlane = glm::cross(xAxisA, xAxisB);
	vector3 xAyBPlane = glm::cross(xAxisA, yAxisB);
	vector3 xAzBPlane = glm::cross(xAxisA, zAxisB);	

	vector3 yAxBPlane = glm::cross(yAxisA, xAxisB);
	vector3 yAyBPlane = glm::cross(yAxisA, yAxisB);
	vector3 yAzBPlane = glm::cross(yAxisA, zAxisB);

	vector3 zAxBPlane = glm::cross(zAxisA, xAxisB);
	vector3 zAyBPlane = glm::cross(zAxisA, yAxisB);
	vector3 zAzBPlane = glm::cross(zAxisA, zAxisB);

	// store these plane results in a vector
	std::vector<vector3> crossPlanes;

	crossPlanes.push_back(xAxBPlane);
	crossPlanes.push_back(xAyBPlane);
	crossPlanes.push_back(xAzBPlane);

	crossPlanes.push_back(yAxBPlane);
	crossPlanes.push_back(yAyBPlane);
	crossPlanes.push_back(yAzBPlane);

	crossPlanes.push_back(zAxBPlane);
	crossPlanes.push_back(zAyBPlane);
	crossPlanes.push_back(zAzBPlane);

	// dot product on corners vs found crossPlanes
	for (int i = 0; i < 9; i++) //for all 9 crossPlanes pls help me
	{
		// re-using min and max variables from round 1 dot product results above
		xMinA = 900.0f, yMinA = 900.0f, zMinA = 900.0f, xMinB = 900.0f, yMinB = 900.0f, zMinB = 900.0f;	// reset temp min values to be replaced through testing
		xMaxA = -900.0f, yMaxA = -900.0f, zMaxA = -900.0f, xMaxB = -900.0f, yMaxB = -900.0f, zMaxB = -900.0f; // reset temp max values to be replaced through testing

		// find mins and maxs of both OBBs against first OBB's axes
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of first box against first box's X axis
			temp = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp < xMinA) xMinA = temp;	// to find point closest to axis
			if (temp > xMaxA) xMaxA = temp; // to find point farthest from axis

			// check min and max values of second box against first box's X axis
			temp2 = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp2 < xMinB) xMinB = temp2;	// to find point closest to axis
			if (temp2 > xMaxB) xMaxB = temp2; // to find point farthest from axis
		}
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of first box against first box's Y axis
			temp = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp < yMinA) yMinA = temp;
			if (temp > yMaxA) yMaxA = temp;

			// check min and max values of second box against first box's Y axis
			temp2 = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp2 < yMinB) yMinB = temp2;
			if (temp2 > yMaxB) yMaxB = temp2;
		}
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of first box against first box's Z axis
			temp = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp < zMinA) zMinA = temp;
			if (temp > zMaxA) zMaxA = temp;

			// check min and max values of second box against first box's Z axis
			temp2 = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp2 < zMinB) zMinB = temp2;
			if (temp2 > zMaxB) zMaxB = temp2;
		}

		// find mins and maxs of both OBBs against second OBB's axes
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of second box against second box's X axis
			temp = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp < xMinB) xMinB = temp;
			if (temp > xMaxB) xMaxB = temp;

			// check min and max values of first box against second box's X axis
			temp2 = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp2 < xMinA) xMinA = temp2;
			if (temp2 > xMaxA) xMaxA = temp2;
		}
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of second box against second box's Y axis
			temp = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp < yMinB) yMinB = temp;
			if (temp > yMaxB) yMaxB = temp;

			// check min and max values of first box against second box's Y axis
			temp2 = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp2 < yMinA) yMinA = temp2;
			if (temp2 > yMaxA) yMaxA = temp2;
		}
		for (int j = 0; j < 8; j++)
		{
			// check min and max values of second box against second box's Z axis
			temp = glm::dot(crossPlanes[i], a_pOther->v3Corner[j]);
			if (temp < zMinB) zMinB = temp;
			if (temp > zMaxB) zMaxB = temp;

			// check min and max values of first box against second box's Z axis
			temp2 = glm::dot(crossPlanes[i], v3Corner[j]);
			if (temp2 < zMinA) zMinA = temp2;
			if (temp2 > zMaxA) zMaxA = temp2;
		}

		//// BOX A
		//for (vector3 ptA : v3Corner) 
		//{
		//	temp = glm::dot(crossPlanes[i], ptA);
		//	if (temp < xMinA) xMinA = temp;
		//	if (temp > xMaxA) xMaxA = temp;
		//}
		//for (vector3 ptA : v3Corner)
		//{
		//	temp = glm::dot(crossPlanes[i], ptA);
		//	if (temp < yMinA) yMinA = temp;
		//	if (temp > yMaxA) yMaxA = temp;
		//}
		//for (vector3 ptA : v3Corner)
		//{
		//	temp = glm::dot(crossPlanes[i], ptA);
		//	if (temp < zMinA) zMinA = temp;
		//	if (temp > zMaxA) zMaxA = temp;
		//}

		//// BOX B
		//for (vector3 ptB : a_pOther->v3Corner)
		//{
		//	temp = glm::dot(crossPlanes[i], ptB);
		//	if (temp < xMinB) xMinB = temp;
		//	if (temp > xMaxB) xMaxB = temp;
		//}
		//for (vector3 ptB : a_pOther->v3Corner)
		//{
		//	temp = glm::dot(crossPlanes[i], ptB);
		//	if (temp < yMinB) yMinB = temp;
		//	if (temp > yMaxB) yMaxB = temp;
		//}
		//for (vector3 ptB : a_pOther->v3Corner)
		//{
		//	temp = glm::dot(crossPlanes[i], ptB);
		//	if (temp < zMinB) zMinB = temp;
		//	if (temp > zMaxB) zMaxB = temp;
		//}

		// return value of 1 (or any not zero) means an axis of separation (ie. no collision) could be found.	//these might be backwards
		if (xMinA > xMaxB) return 1;
		if (xMaxA < xMinB) return 1;
		if (yMinA > yMaxB) return 1;
		if (yMaxA < yMinB) return 1;
		if (zMinA > zMaxB) return 1;
		if (zMaxA < zMinB) return 1;
	}
#pragma endregion

	// there is no axis test that separates these two objects //ie. they're colliding
	return eSATResults::SAT_NONE;
}