#include "Octant.h"
using namespace Simplex;

// Overall octree variables
uint Octant::m_uOctantCount = 0; // total number of octants created
uint Octant::m_uMaxLevel; // max possible level of the octree (we don't want it to subdivide endlessly)
uint Octant::m_uIdealEntityCount; // max number of entities each octant should contain // RENAME THIS VAR?


#pragma region The Big Three, and then some
// Constructor 1 (For creating Root)
Octant::Octant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5) 
{
	Init();	// calls entitymngr and meshmngr singletons
	m_uID = m_uOctantCount;	// starts indexing octants at 0
	m_uOctantCount++;		// actual count of octants

	m_pRoot = this;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	//for size: get center, min and max values
	m_v3Min = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	m_v3Max = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	vector3 tempMin = vector3(0.0f);
	vector3 tempMax = vector3(0.0f);

	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++) 
	{
		//similar to ARBB/SAT, we're getting the smallest possible min and largest possible max across all entities to construct corners of octant.
		tempMin = m_pEntityMngr->GetRigidBody(i)->GetMinGlobal();
		tempMax = m_pEntityMngr->GetRigidBody(i)->GetMaxGlobal();

		if (tempMin.x < m_v3Min.x) m_v3Min.x = tempMin.x;
		if (tempMax.x > m_v3Max.x) m_v3Max.x = tempMax.x;

		if (tempMin.y < m_v3Min.y) m_v3Min.y = tempMin.y;
		if (tempMax.y > m_v3Max.y) m_v3Max.y = tempMax.y;

		if (tempMin.z < m_v3Min.z) m_v3Min.z = tempMin.z;
		if (tempMax.z > m_v3Max.z) m_v3Max.z = tempMax.z;
	}

	m_v3Center = (m_v3Min + m_v3Min) / 2.0f;	// center point of the octant
	m_v3Size = (m_v3Min - m_v3Min); // size of the octant as a vector
	
	//add entities to octant
	//subdivision would happen here
}

// Constructor 2 (For creating all children octants)
Octant::Octant(vector3 a_v3Center, vector3 a_v3Size) 
{
	Init();	// calls entitymngr and meshmngr singletons
	m_uID = m_uOctantCount;
	m_uOctantCount++;

	m_v3Center = a_v3Center;
	m_v3Size = a_v3Size;

	m_v3Max = (m_v3Size / 2.0f) + m_v3Center;
	m_v3Min = (m_v3Size / 2.0f) - m_v3Center;
}

// Copy Assignment Operator
Octant& Octant::operator=(Octant const& other) 
{
	Init();

	//copying the following: level, num of children, parent, min, max, center, size, array of children, entity list, 

	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	m_pParent = other.m_pParent;

	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_v3Center = other.m_v3Center;
	m_v3Size = other.m_v3Size;


}

// Destructor
Octant::~Octant(void) { Release(); }

// Swap
void Octant::Swap(Octant& other) {}
#pragma endregion

#pragma region Octree Creation / Destruction
// creates 8 octant children for this octant
void Octant::Subdivide(void) {}

// Creates a tree through subdivision, with up to the max number of levels
void Octant::ConstructTree(uint a_nMaxLevel = 3) {}

// Returns the total number of octants in the world
uint Octant::GetOctantCount(void) { return m_uOctantCount; }

// returns the child octant specified by index
Octant* Octant::GetChild(uint a_nChild) { return m_pChild[a_nChild]; }

// returns the parent of this octant
Octant* Octant::GetParent(void) { return m_pParent; }

// clears the entity list for each octant (will do so recursively)
void Octant::ClearEntityList(void) {}

// Deletes all children and any further descendants
void Octant::KillBranches(void) {}
#pragma endregion

#pragma region Octant Shape Creation
// Returns this octant's size vector
vector3 Octant::GetSize(void) { return m_v3Size; }

// Returns this octant's center vector in global space
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }

// Returns this octant's min vector (BLB) in global space
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }

// Returns this octant's max vector (TRF) in global space
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
#pragma endregion

#pragma region Display Functions
// Displays the specified octant as well as all its children octants //( AM I DISPLAYING ONLY IMMEDIATE CHILDREN, OR ALL DESCENDANTS?)
void Octant::DisplayDescendants(uint a_nIndex, vector3 a_v3Color = C_YELLOW) {}

// Displays only the specified octant
void Octant::Display(vector3 a_v3Color = C_YELLOW) {}

// Displays any leaf octants that currently contain entities //(use if you want to cycle through just this type of octant)
void Octant::DisplayOccupiedLeaves(vector3 a_v3Color = C_YELLOW) {}
#pragma endregion

#pragma region Helper Functions
// Checks if the specified entity is contained by this octant
bool Octant::IsContained(uint a_uRBIndex) {}

// Checks if this octant contains no child octants
bool Octant::IsLeaf(void) 
{
	if (m_uChildren == 0)
		return true;
	return false;
}

// Checks if this octant has more than the specified number of entities
bool Octant::ContainsMoreThan(uint a_nEntities) {}

// Assigns IDs to entities contained by leaf nodes
void Octant::AssignIDtoEntity(void) {}
#pragma endregion

// Deallocates member data
void Octant::Release(void) {}

// Allocates member data
void Octant::Init(void) 
{
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
}

// Creates list of all leaf octants that contain entities
void Octant::ListOccupiedLeaves(void) {}