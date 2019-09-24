#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);


	//DRAW SPACE INVADER HERE 
	// create separate function to make code clearer to read
		// draw row by row (8 rows total, widest is 11 across)
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

	/*	
	//Taken from C11 Translation and Scale demo
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	// SCALE
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
	static float value = 0.0f;	// Counter, static to increment per display iteration
	// TRANSLATION
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;				// Will only increment after mesh has been translated in this instance

	//matrix4 m4Model = m4Translate * m4Scale;	//backwards multiplication **********DO NOT USE***********
	matrix4 m4Model = m4Scale * m4Translate;

	m_pMesh->Render(m4Projection, m4View, m4Model);

	//whether using the above or below rendering method, final rendering will always follow this paradigm: mesh->Render(Projection matrix, View matrix, Model matrix)
	*/

	//m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 0.0f, 0.0f)));

	//FOR LEFT-RIGHT SPACE INVADER MOTION
	// keep a "direction" variable, set equal to 1
	// multiply translation value by "direction" variable
	// whenever counter hits absVal of 10 (or some other value), multiply "direction" var by -1 (flip translation direction)
	// MUST APPLY THIS LOGIC TO EVERY CUBE? I think that should be fine (ie, not too costly)

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
void Application::Release(void)
{

	// CREATE FUNCTION, DELETE ALL CUBES HERE

	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}