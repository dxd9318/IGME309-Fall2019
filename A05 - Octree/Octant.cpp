#include "Octant.h"
using namespace Simplex;





#pragma region The Big Three, and then some
// Constructor 1 (Root)
Octant::Octant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5) {}

// Constructor 2 (All children octants)
Octant::Octant(vector3 a_v3Center, vector3 a_v3Size) {}

// Copy Assignment Operator
Octant& Octant::operator=(Octant const& other) {}

// Destructor
Octant::~Octant(void) {}

// Swap
void Octant::Swap(Octant& other) {}
#pragma endregion

#pragma region Octree Creation / Destruction
// creates 8 octant children for this octant
void Octant::Subdivide(void) {}

// Creates a tree through subdivision, with up to the max number of levels
void Octant::ConstructTree(uint a_nMaxLevel = 3) {}

// Returns the total number of octants in the world
uint Octant::GetOctantCount(void) {}

// returns the child octant specified by index
Octant* Octant::GetChild(uint a_nChild) {}

// returns the parent of this octant
Octant* Octant::GetParent(void) {}

// clears the entity list for each octant (will do so recursively)
void Octant::ClearEntityList(void) {}

// Deletes all children and any further descendants
void Octant::KillBranches(void) {}
#pragma endregion

#pragma region Octant Shape Creation
// Returns this octant's size vector
vector3 Octant::GetSize(void) {}

// Returns this octant's center vector in global space
vector3 Octant::GetCenterGlobal(void) {}

// Returns this octant's min vector (BLB) in global space
vector3 Octant::GetMinGlobal(void) {}

// Returns this octant's max vector (TRF) in global space
vector3 Octant::GetMaxGlobal(void) {}
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
bool Octant::IsLeaf(void) {}

// Checks if this octant has more than the specified number of entities
bool Octant::ContainsMoreThan(uint a_nEntities) {}

// Assigns IDs to entities contained by leaf nodes
void Octant::AssignIDtoEntity(void) {}
#pragma endregion

// Deallocates member data
void Octant::Release(void) {}

// Allocates member data
void Octant::Init(void) {}

// Creates list of all leaf octants that contain entities
void Octant::ListOccupiedLeaves(void) {}