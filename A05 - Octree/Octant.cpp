#include "Octant.h"
using namespace Simplex;





#pragma region The Big Three, and then some
// Constructor 1 (For creating Root)
Octant::Octant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5) 
{
	m_pRoot = this;

	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
}

// Constructor 2 (For creating all children octants)
Octant::Octant(vector3 a_v3Center, vector3 a_v3Size) 
{
	m_v3Center = a_v3Center;
	m_v3Size = a_v3Size;
}

// Copy Assignment Operator
Octant& Octant::operator=(Octant const& other) {}

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