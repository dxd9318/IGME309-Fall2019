#include "Octant.h"
using namespace Simplex;

// Overall octree variables
uint Octant::m_uOctantCount = 0; // total number of octants created
uint Octant::m_uMaxLevel = 2; // max possible level of the octree (we don't want it to subdivide endlessly)
uint Octant::m_uIdealEntityCount = 5; // max number of entities each octant should contain // RENAME THIS VAR?


#pragma region The Big Three, and then some
// Constructor 1 (For creating Root)	////////////////// WITH ALBERTO'S EXAMPLE, THIS GET CALLED EVERY TIME WE ADD OR REMOVE A LEVEL USING KEY PRESSES (AppClassControls.cpp) // DELETE THE CURRENT TREE, CREATE A NEW ONE. WATCH FOR MEMORY LEAKS
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uOctantCount = 0;	//makes sure to set octant count (a static var) to 0 whenever new root is created
	Init();	// calls entitymngr and meshmngr singletons, inits values
	//m_uID = m_uOctantCount;	// starts indexing octants at 0 // done in Init
	m_uOctantCount++;// increment to account for this root octant

	m_pRoot = this;
	m_lChild.clear();
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
	//subdivision/constructtree would happen here
	//Subdivide();
}

// Constructor 2 (For creating all children octants)
Octant::Octant(vector3 a_v3Center, vector3 a_v3Size)
{
	Init();	// calls entitymngr and meshmngr singletons	//CAREFUL, SETS THINGS TO NULL
	//m_uID = m_uOctantCount; // done in Init
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

	//copying the following: root, level, parent, min, max, center, size, num of children, array of children, entity list, 
	m_pRoot = other.m_pRoot;
	m_uLevel = other.m_uLevel;
	//m_uChildren = other.m_uChildren;
	m_pParent = other.m_pParent;

	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_v3Center = other.m_v3Center;
	m_v3Size = other.m_v3Size;

	Release(); // should remove any children of the copy octant that might already exist, so that they can be replaced
	m_uChildren = other.m_uChildren;
	//copy children here, recursion
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}

	//copy over entity list
	for (int i = 0; i < other.m_pEntityMngr->GetEntityCount(); i++)
	{
		m_EntityList.push_back(other.m_EntityList[i]);
	}


}

// Destructor
Octant::~Octant(void) { Release(); }

// Swap
void Octant::Swap(Octant& other)
{
	std::swap(m_pRoot, other.m_pRoot);

	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	for (int i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
	std::swap(m_pParent, other.m_pParent);

	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Size, other.m_v3Size);

	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
}
#pragma endregion

#pragma region Octree Creation / Destruction
// creates 8 octant children for this octant	// SETS DIMENSIONALITY OF EACH ENTITIY USING ENTITY MANAGER, MAKE SURE TO CLEAR DIMENSIONS IN AppClassControls.cpp ON KEY PRESSES
void Octant::Subdivide(void) 
{
	if (m_uChildren > 0) return;	// Every time octant subdivides, number of children will increase. We don't want to subdivide an octant that's already subdivided
	if (m_uLevel >= m_uMaxLevel) return; // Cannot subdivide passed max level

	//create 8 octants, specify their relative positions and centers, calculated based on the position and center of this octant. Populate m_pchild with these new octants
	vector3 tempSize = m_v3Size / 4.0f;
	vector3 tempCenter = m_v3Center - tempSize;
	
	//BLB
	m_pChild[0] = new Octant(tempCenter, tempSize * 2.0f);

	//BRB
	m_pChild[1] = new Octant(vector3(tempCenter.x + (tempSize * 2.0f), tempCenter.y, tempCenter.z), tempSize * 2.0f);

	//BLF
	m_pChild[2] = new Octant(vector3(tempCenter.x, tempCenter.y, tempCenter.z + (tempSize * 2.0f)), tempSize * 2.0f);

	//BRF
	m_pChild[3] = new Octant(vector3(tempCenter.x + (tempSize * 2.0f), tempCenter.y, tempCenter.z + (tempSize * 2.0f)), tempSize * 2.0f);

	//TLB
	m_pChild[4] = new Octant(vector3(tempCenter.x, tempCenter.y + (tempSize * 2.0f), tempCenter.z), tempSize * 2.0f);

	//TRB
	m_pChild[5] = new Octant(vector3(tempCenter.x + (tempSize * 2.0f), tempCenter.y + (tempSize * 2.0f), tempCenter.z), tempSize * 2.0f);

	//TLF
	m_pChild[6] = new Octant(vector3(tempCenter.x, tempCenter.y + (tempSize * 2.0f), tempCenter.z + (tempSize * 2.0f)), tempSize * 2.0f);

	//TRF
	m_pChild[7] = new Octant(vector3(tempCenter.x + (tempSize * 2.0f), tempCenter.y + (tempSize * 2.0f), tempCenter.z + (tempSize * 2.0f)), tempSize * 2.0f);

	m_uChildren = 8;

	// Need to set some values that were reset/set to null when new children were initialized
	for (int i = 0; i < 8; i++) 
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		m_pChild[i]->m_pParent = this;

		// CHECK IF THIS CHILD CONTAINS ANY ENTITIES
		// UPDATE ENTITIY COUNT ACCORDINGLY

		// Subdivide again if necessary
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount)) m_pChild[i]->Subdivide();
	}
}

// Creates a tree through subdivision, with up to the max number of levels
void Octant::ConstructTree(uint a_nMaxLevel = 3) {}

// Returns the total number of octants in the world
uint Octant::GetOctantCount(void) { return m_uOctantCount; }

// returns the child octant specified by index
Octant* Octant::GetChild(uint a_nChild)
{
	if (m_uChildren == 0)
	{
		return nullptr;
	}
	return m_pChild[a_nChild];
}

// returns the parent of this octant
Octant* Octant::GetParent(void) { return m_pParent; }

// clears the entity list for each octant (will do so recursively)
void Octant::ClearEntityList(void)
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[0]->ClearEntityList();
	}
	m_EntityList.clear();
}

// Deletes all children and any further descendants
void Octant::KillBranches(void)
{
	if (IsLeaf()) return;

	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		SafeDelete(m_pChild[i]);
		m_pChild[i] = nullptr;
	}

	m_uChildren = 0;
}
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
// Display helper function, displays an octant
void Octant::Display(vector3 a_v3Color = C_YELLOW)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(IDENTITY_M4, m_v3Size), a_v3Color);
}

// Displays the octant specified by index
void Octant::DisplaySpecified(uint a_nIndex, vector3 a_v3Color = C_YELLOW)
{
	//if the specified index doesn't exist, default to DisplayWholeTree
	if (a_nIndex >= m_uID) DisplayWholeTree();
	else
		m_lChild[a_nIndex]->Display(a_v3Color);
}

// Displays all octants in the tree recursively, from the root to all the leaves	//WATCH OUT, only octants with any children that contain entities should be displayed
void Octant::DisplayWholeTree(vector3 a_v3Color = C_YELLOW) 
{
	// Calls DisplayOne on leaves as base case
	if (IsLeaf()) Display(a_v3Color);
	else 
	{
		for (int i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->DisplayWholeTree(a_v3Color);
		}
	}
}
#pragma endregion

#pragma region Helper Functions
// Checks if the specified entity is contained by this octant
bool Octant::IsContained(uint a_uRBIndex) 
{
	//ARBB
}

// Checks if this octant has more than the specified number of entities
bool Octant::ContainsMoreThan(uint a_nEntities)
{
	// USES IsContained(uint)
	//for all entities in the entitymngr, is its contained by this octant increase a temp count. If this temp count is greater than a_nEntities, return true, else false.
}

// Checks if this octant contains no child octants
bool Octant::IsLeaf(void)
{
	if (m_uChildren == 0)
		return true;
	return false;
}

// Assigns IDs to entities contained by leaf nodes
void Octant::AssignIDtoEntity(void) {}
#pragma endregion

// Deallocates member data // Calls KillBranches (recursive), and so should only really deal with de-allocating root
void Octant::Release(void)
{
	if (m_pRoot == this) KillBranches();

	m_lChild.clear();
	m_uChildren = 0;
	m_v3Size = vector3(0.0f);
	m_EntityList.clear();
}

// Allocates member data
void Octant::Init(void)
{
	m_pRoot = nullptr;

	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_uChildren = 0;
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
	m_pParent = nullptr;

	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3Center = vector3(0.0f);
	m_v3Size = vector3(0.0f);

	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
}

// Creates list of all leaf octants that contain entities
void Octant::ListOccupiedLeaves(void) {}