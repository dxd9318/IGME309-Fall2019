#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Diana Diaz - dxd9318@g.rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));
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

	// Draw the model
	m_pModel->PlaySequence();

	// Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//-------------------
	// Set current sprint number, start, and end points
	static uint sprintCounter = 0;	// Keeps track of number of the current "sprint", ie. the path between the current and next stops.
	vector3 v3SprintStart = m_stopsList[sprintCounter];	// Sets starting point for the current sprint, using sprintCounter as an index for the stops vector.
	vector3 v3SprintEnd = m_stopsList[(sprintCounter + 1) % m_stopsList.size()];	/*	Sets the next stop as the end of the current sprint. 
																						% used to loop to first stop if at end of stops vector. */
	// Calculate percentage to LERP by
	float fTimeBtwnStops = 1.0f;
	float fLerpPercentage = MapValue(fTimer, 0.0f, fTimeBtwnStops, 0.0f, 1.0f);		/*	Converts the ratio of [current elapsed time(fTimer) : fTimeBtwnStops]
																						into a percentage value, scaled between 0.0f and 1.0f. */
	// Use LERP percentage to calculate the current position
	vector3 v3CurrentPos = glm::lerp(v3SprintStart, v3SprintEnd, fLerpPercentage);
	
	// Reset timer and percentage when sprint ends (percentage == 100%)
	if (fLerpPercentage >= 1.0f)
	{
		sprintCounter++;							// increment sprintCounter to set start of next sprint
		sprintCounter %= m_stopsList.size();		// if this was the last sprint, reset the sprint number to allow looping
		fTimer = m_pSystem->GetDeltaTime(uClock);	// reset timer to allow LERPing for next sprint
	}
	//-------------------

	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
	}
	
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
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}