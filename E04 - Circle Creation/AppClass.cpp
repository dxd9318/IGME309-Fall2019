#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	//Generate a circle

	//m_pMesh->GenerateCircle(5.0f, 3, C_RED);		//	TRIANGLE TEST
	//m_pMesh->GenerateCircle(5.0f, 4, C_RED);		//	QUAD TEST
	//m_pMesh->GenerateCircle(2.0f, 5, C_RED);		//	PENTAGON TEST (DEFAULT)

	//m_pMesh->GenerateCircle(2.0f, 12, C_RED);		//	12-Sided
	m_pMesh->GenerateCircle(2.0f, 360, C_RED);		//	360 Sides


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

	//Render the mesh
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
		
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
	//Safely release the memory
	SafeDelete(m_pMesh);
	//release GUI
	ShutdownGUI();
}