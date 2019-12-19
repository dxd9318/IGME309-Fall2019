#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 5.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)


	/*
		NOTES:

		- Can't figure out how to add a wireframe cube as an entity. I tried creating a new set of entitymngr functions, and entity functions, but it was erroring out.
		- Even if I did managed to do this, I'd have to add an alternate set of physics solver code. I'd have to invert collision detection for the cube. With the 
			way things are now, any spheres spawning within the cube would automatically get pushed out of it. Would need to change it so any sphere would bounce if its position
			becomes less than the left/bottom/front face, and greater than the right/top/back face.
		- Depending on the density of the particles, octree would be a good spatial optimization implementation to include here, with subdivisions scaling along with particle density.
		- I reduced the gravity force in MySolver to give the particles a more floaty feeling upon spawning, instead of having them immediately drop to the ground.
	*/

	// wireframe box	
	//m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_1");
	//m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(0.0f)) * glm::scale(IDENTITY_M4, vector3(10.0f)), C_BLUE); //NEEDS TO BE AN ENTITY TO REMAIN UPDATED, CURRENTLY DISAPPEARING AFTER FIRST FRAME
	//m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f, -5.0f, 0.0f)) * glm::scale(vector3(10.0f)));
	//m_pEntityMngr->UsePhysicsSolver();

	// spawn spheres
	for (int i = 0; i < 100; i++)
	{
		m_pEntityMngr->AddEntity("Planets\\03A_Moon.obj", "Sphere_" + std::to_string(i));
		vector3 v3Position = vector3(glm::sphericalRand(12.0f));
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(0.25f)));	//make them tiny
		m_pEntityMngr->UsePhysicsSolver();
		m_pEntityMngr->SetMass(0.01f * i);	// for applying super pseudo 'random' mass to each sphere
		m_pEntityMngr->ApplyForce(vector3(glm::sphericalRand(1.05f)));	// each sphere will have unique random starting acceleration vectors
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(0.0f)) * glm::scale(IDENTITY_M4, vector3(10.0f)), C_BLUE);

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Set the model matrix for the main object
	//m_pEntityMngr->SetModelMatrix(m_m4Steve, "Steve");

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}