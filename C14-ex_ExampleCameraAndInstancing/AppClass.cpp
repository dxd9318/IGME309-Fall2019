#include "AppClass.h"
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	/*
	//initializing the model
	m_pModel = new BasicX::Model();

	//Load a model
	m_pModel->Load("Minecraft\\Steve.obj");
	*/

	m_pCone = new MyMesh();
	m_pCone->GenerateCone(1.0f, 2.0f, 10, Simplex::vector3(1.0f,0.0f, 0.0f));

	m_pCamera = new MyCamera(m_pSystem, vector3(0.0f, 0.0f, -10.0f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Get a timer
	static uint uClock = m_pSystem->GenClock();
	float fTimer = m_pSystem->GetTimeSinceStart(uClock);
	float fDeltaTime = m_pSystem->GetDeltaTime(uClock);
	
	/*
	//Attach the model matrix that takes me from the world coordinate system
	m_pModel->SetModelMatrix(m_m4Steve);

	//Send the model to render list
	m_pModel->AddToRenderList();
	*/
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//static float theta = 0.0f;
	
	// Draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	auto mat4Model = glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.0f));
	//mat4Model = glm::rotate(mat4Model, theta, vector3(1.0f, 1.0f, 0.0f));	//rotates the object
	//theta += 0.05f;

	//m_pCone->Render(mat4Projection, mat4View, mat4Model);
	m_pCone->Render(m_pCamera->GetProjectionMatrix(), m_pCamera->GetViewMatrix(), mat4Model);

	// Render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	// Clear the render list
	m_pMeshMngr->ClearRenderList();
	
	// Draw gui
	DrawGUI();
	
	// End the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	/*
	//release model
	SafeDelete(m_pModel);
	*/

	//release GUI
	ShutdownGUI();
}
