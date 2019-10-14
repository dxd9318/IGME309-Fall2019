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

	static float theta = 0.0f;
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	auto mat4Model = glm::translate(IDENTITY_M4, vector3(2.5f, 0.0f, 0.0f));
	mat4Model = glm::rotate(mat4Model, theta, vector3(1.0f, 1.0f, 0.0f));
	theta += 0.05f;


	// zPos (below) helps demonstrate that ortho projections map the points of an object to the points of your screen. 
	// Changing this value does NOT change how the object looks in terms of size. 
	// However, can cause clipping if object doesn't sit within space of near and far planes, OR depending on placement of view as dependent on zPos.
	float zPos = -6.0f;	
	auto mat4View = glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, zPos));		//zPos is translated backwards so as to not be inside of the objects we're trying to look at
	/*
	// This was an accident, should've been rotating the model matrix (now above), but what it does is cool. 
	// When rotating the view matrix and using the perspective projection matrix (below), the cone spins around in a weird way.
	mat4View = glm::rotate(mat4View, theta, vector3(1.0f, 1.0f, 0.0f));
	theta += 0.01f;
	*/

	float width = (float)m_pSystem->GetWindowWidth();
	float height = (float)m_pSystem->GetWindowHeight();
	float ratio = width / height;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = glm::radians(60.0f);

	//auto mat4Projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, nearPlane, farPlane);
	auto mat4Projection = glm::perspective(fov, ratio, nearPlane, farPlane);


	m_pCone->Render(mat4Projection, mat4View, mat4Model);

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
	/*
	//release model
	SafeDelete(m_pModel);
	*/

	//release GUI
	ShutdownGUI();
}
