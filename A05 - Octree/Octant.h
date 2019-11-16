#pragma once
#ifndef __OCTANT_H_
#define __OCTANT_H_

#include "MyEntityManager.h"

namespace Simplex 
{
	
class Octant // created based off of the Octant.h file used in the C21_SpatialOptimizationPt2 class example
{
	// Overall octree variables
	static uint m_uOctantCount; // total number of octants created
	static uint m_uMaxLevel; // max possible level of the octree (we don't want it to subdivide endlessly)
	static uint m_uIdealEntityCount; // max number of entities each octant should contain // RENAME THIS VAR?

	// Root of octree
	Octant* m_pRoot = nullptr; // root octant
	std::vector<Octant*>m_lChild; // list of octants that have active child octants

	// Identification info for this octant
	uint m_uID = 0; // this octant's ID
	uint m_uLevel = 0; // the level of this octant in the octree
	uint m_uChildren = 0; // number of children this octant has, which will be either 0 or 8 (none or all)
	Octant* m_pChild[8]; // array of this octant's children
	Octant* m_pParent = nullptr; // this octant's parent octant

	// Shape info for constructing this octant
	vector3 m_v3Min = vector3(0.0f);	// octant's min corner (BLB)
	vector3 m_v3Max = vector3(0.0f);	// octant's max corner (TRF)
	vector3 m_v3Center = vector3(0.0f);	// center point of the octant
	vector3 m_v3Size = vector3(0.0f); // size of the octant as a vector

	// Entity Management
	MeshManager* m_pMeshMngr = nullptr; // Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; // Entity Manager singleton
	std::vector<uint> m_EntityList;	// list of indices corresponding to entities currently located within this octant

public:
#pragma region The Big Three, and then some
	// Constructor 1 (Root)
	Octant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);

	// Constructor 2 (All children octants)
	Octant(vector3 a_v3Center, vector3 a_v3Size);

	// Copy Assignment Operator
	Octant& operator=(Octant const& other);

	// Destructor
	~Octant(void);

	// Swap
	void Swap(Octant& other);
#pragma endregion

#pragma region Octree Creation / Destruction
	// creates 8 octant children for this octant
	void Subdivide(void);

	// Creates a tree through subdivision, with up to the max number of levels
	void ConstructTree(uint a_nMaxLevel = 3);

	// Returns the total number of octants in the world
	uint GetOctantCount(void);

	// returns the child octant specified by index
	Octant* GetChild(uint a_nChild);

	// returns the parent of this octant
	Octant* GetParent(void);

	// clears the entity list for each octant (will do so recursively)
	void ClearEntityList(void);

	// Deletes all children and any further descendants
	void KillBranches(void);
#pragma endregion

#pragma region Octant Shape Creation
	// Returns this octant's size vector
	vector3 GetSize(void);

	// Returns this octant's center vector in global space
	vector3 GetCenterGlobal(void);

	// Returns this octant's min vector (BLB) in global space
	vector3 GetMinGlobal(void);

	// Returns this octant's max vector (TRF) in global space
	vector3 GetMaxGlobal(void);
#pragma endregion

#pragma region Display Functions
	// Displays the specified octant as well as all its children octants //( AM I DISPLAYING ONLY IMMEDIATE CHILDREN, OR ALL DESCENDANTS?)
	void DisplayDescendants(uint a_nIndex, vector3 a_v3Color = C_YELLOW);

	// Displays only the specified octant
	void Display(vector3 a_v3Color = C_YELLOW);

	// Displays any leaf octants that currently contain entities //(use if you want to cycle through just this type of octant)
	void DisplayOccupiedLeaves(vector3 a_v3Color = C_YELLOW);
#pragma endregion

#pragma region Helper Functions
	// Checks if the specified entity is contained by this octant
	bool IsContained(uint a_uRBIndex);

	// Checks if this octant contains no child octants
	bool IsLeaf(void);

	// Checks if this octant has more than the specified number of entities
	bool ContainsMoreThan(uint a_nEntities);

	// Assigns IDs to entities contained by leaf nodes
	void AssignIDtoEntity(void);
#pragma endregion

private:
	// Deallocates member data
	void Release(void);

	// Allocates member data
	void Init(void);

	// Creates list of all leaf octants that contain entities
	void ListOccupiedLeaves(void);
};	// class end

}	// namespace Simplex end

#endif // __OCTANT_H_
