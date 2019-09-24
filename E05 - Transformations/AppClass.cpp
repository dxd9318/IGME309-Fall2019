#include "AppClass.h"
void Application::InitVariables(void)
{
	// Make MyMesh object
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(2.0f, C_BROWN);
	//m_pMesh1 = new MyMesh();
	//m_pMesh1->GenerateCube(1.0f, C_WHITE);
	
	// According to C11 Translation and Scale demo, can use the same MyMesh instance to generate multiple shapes, 
	// instead of calling a new MyMesh every time as above.

	// GENERATE ALL 46 CUBES FOR THE SPACE INVADER HERE
	for (int i = 0; i < 46; i++) {
		m_pMesh->GenerateCube(2.0f, C_BROWN);
		spaceInvaderCubes.push_back(m_pMesh);
	}

	// APPLY POSITIONS FOR EACH CUBE HERE, BEFORE TRANSLATING THEM??
	//GenerateSpaceInvader();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Taken from C11 Translation and Scale demo
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	// SCALE
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));	// Will be the same for all cubes
	//static float value = 0.0f;	// Counter, static to increment per display iteration

	//////////// APPLY POSITIONS FOR EACH CUBE HERE, BEFORE TRANSLATING THEM	// NOT SURE IF DOING THIS HERE IS WHY CONSIDERING THIS SHOULD MEAN IT RESETS POSITION EVERY FRAME DESPITE TRANSLATION
	GenerateSpaceInvader();

	/*
	// TRANSLATION
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;				// Will only increment after mesh has been translated in this instance	//add to x position

	//FOR LEFT-RIGHT SPACE INVADER MOTION
	// keep a "direction" variable, set equal to 1
	// multiply translation value by "direction" variable
	// whenever counter hits absVal of 10 (or some other value), multiply "direction" var by -1 (flip translation direction)
	// MUST APPLY THIS LOGIC TO EVERY CUBE? I think that should be fine (ie, not too costly)



	matrix4 m4Model = m4Scale * m4Translate;

	m_pMesh->Render(m4Projection, m4View, m4Model);	// APPLY TO EACH CUBE INSTEAD
	*/
	for (int i = 0; i < 46; i++)
	{
		matrix4 m4Translate = glm::translate(IDENTITY_M4, (meshPositions[i]));
		matrix4 m4Model = m4Scale * m4Translate;

		spaceInvaderCubes[i]->Render(m4Projection, m4View, m4Model);

	}
	/*	//Rendering individual meshes
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));
	*/


	

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::GenerateSpaceInvader(void) 
{
	// 8 ROWS, 11 MAX COLUMNS

	// Row 0	//Top
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, 4.0f, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, 3.0f, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(2.0f, 3.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, 4.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 1
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-3.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f, 3.0f, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f, 3.0f, 0.0f));
	//meshPositions.push_back(vector3(3.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 2
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(0.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(1.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f, 2.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, 2.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 3	// "Eyes"
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-4.0f, 1.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, 1.0f, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f, 1.0f, 0.0f));
	meshPositions.push_back(vector3(0.0f, 1.0f, 0.0f));
	meshPositions.push_back(vector3(1.0f, 1.0f, 0.0f));
	//meshPositions.push_back(vector3(2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, 1.0f, 0.0f));
	meshPositions.push_back(vector3(4.0f, 1.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 4	// "Center"
	meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f, 0.0f, 0.0f));

	meshPositions.push_back(vector3(0.0f,0.0f,0.0f));	// "dead center"

	meshPositions.push_back(vector3(1.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 5
	meshPositions.push_back(vector3(-5.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(0.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(1.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f, -1.0f, 0.0f));

	// Row 6
	meshPositions.push_back(vector3(-5.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(2.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f, -2.0f, 0.0f));

	// Row 7	//Bottom
	//meshPositions.push_back(vector3(-5.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(-3.0f, -2.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f, -3.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f, -3.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(1.0f, -3.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f, -3.0f, 0.0f));
	//meshPositions.push_back(vector3(3.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, -2.0f, 0.0f));
}
void Application::Release(void)
{
	//IM GETTING MEMORY LEAKS
	//I DONT UNDERSTAND HOW TO DEALLOCATE POINTERS FROM A VECTOR

	// DELETE ALL CUBES HERE
	/*for (int i = 0; i < 46; i++) 
	{
		if (spaceInvaderCubes[i] != nullptr)
		{
			delete spaceInvaderCubes[i];
			spaceInvaderCubes[i] = nullptr;
		}
	}
	spaceInvaderCubes.clear();
	*/

	//SafeDelete(m_pMesh1);
	// Release GUI
	ShutdownGUI();
}