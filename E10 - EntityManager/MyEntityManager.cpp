#include "MyEntityManager.h"
using namespace Simplex;

// MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	// start with a clean slate
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	// end with a clean slate
	for (int i = 0; i < m_uEntityCount; i++) 
	{
		//make sure to deallocate any and all stored entities
		MyEntity* pEntity = m_entityList[i];
		SafeDelete(pEntity);
	}
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	// this is important given that the MyEntityManager object should be a singleton. There should only ever be one instance of the entity manager.
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr) 
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//find the index of the specified entity within the entity manager's list/array
	for (int i = 0; i < m_uEntityCount; i++) 
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
			return i;
	}
	// returns -1 if index wasn't found. Indicates invalid UniqueID
	return -1;
}

// Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	// we'll have to make sure the UniqueID is valid (ie. can actually find a returnable value) first
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	if (temp)
		return temp->GetModel();

	// Invalid UniqueID, can't find a respective entity
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	// we'll have to make sure the UniqueID is valid (ie. can actually find a returnable value) first
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	if (temp)
		return temp->GetRigidBody();

	// Invalid UniqueID, can't find a respective entity
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return IDENTITY_M4;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	// we'll have to make sure the UniqueID is valid (ie. can actually find a returnable value) first
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	if (temp)
		return temp->GetModelMatrix();

	// Invalid UniqueID, can't find a respective entity
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	// we'll have to make sure the UniqueID is valid (ie. can actually find a returnable value) first
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	if (temp)
		return temp->SetModelMatrix(a_m4ToWorld);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}

// The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};

// Other methods
void Simplex::MyEntityManager::Update(void)
{
	//simple collision detection
	for (int i = 0; i < m_uEntityCount - 1; i++) 
	{
		for (int j = i + 1; j < m_uEntityCount; j++) 
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//create a temp entity using these values, then add to entity list if initialization successful
	MyEntity* temp = new MyEntity(a_sFileName, a_sUniqueID);
	if (temp->IsInitialized()) 
	{
		m_entityList.push_back(temp);
		m_uEntityCount = m_entityList.size();	//increases the entity count precisely by using the entity list size instead of just adding one, since multiple entities could be added at once
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	// if the entity we want isn't the last one in the entity manager's list, swap it with the last one then pop it.
	// this is more efficient than removing the entity we want and then having to shift every following entity over by one.
	if (a_uIndex != m_uEntityCount - 1)
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);

	// De-allocate the entity, pop it from the entity manager's list, and decrement the entity count
	MyEntity* temp = m_entityList[m_uEntityCount - 1];
	SafeDelete(temp);
	m_entityList.pop_back();
	--m_uEntityCount;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	// we can use other already established methods here to re-use code
	int foundIndex = GetEntityIndex(a_sUniqueID);	//find the index of the entity with the given UniqueID
	RemoveEntity(foundIndex);	//will pop and deallocate the entity accordingly if index was found.
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return "";

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	// if entity manger list doesn't contain any entities, return nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	// if index is greater than number of entities in entity manager list, use last indexed entity
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	// if the index is out of bounds, we want all entities rendered, probably to visibly depict an error that needs to be checked
	if (a_uIndex >= m_uEntityCount)
	{
		for (int i = 0; i < m_uEntityCount; i++)
		{
			m_entityList[i]->AddToRenderList(a_bRigidBody);
		}
	}
	else // just add the specified entity to the render list
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//create a temp entity using these values, then add to entity list if initialization successful
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);

	if (temp)
		temp->AddToRenderList(a_bRigidBody);
}