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
	vector3 m_vMin = vector3(0.0f);	// octant's min corner (BLB)
	vector3 m_vMax = vector3(0.0f);	// octant's max corner (TRF)
	vector3 m_vCenter = vector3(0.0f);	// center point of the octant
	vector3 m_vSize = vector3(0.0f); // size of the octant as a vector

	// Entity Management
	MeshManager* m_pMeshMngr = nullptr; // Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; // Entity Manager singleton
	std::vector<uint> m_EntityList;	// list of indices corresponding to entities currently located within this octant

};	//class end

}	//namespace Simplex end

#endif // __OCTANT_H_
