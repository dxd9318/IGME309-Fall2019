#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Diana Diaz - dxd9318@g.rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		
		// Initialize stops list	//might require another for loop to generate stops dynamically
		// As the orbits are created, create the list of stops for each orbit as well
		// Angle between stops of the orbit is (360deg / number of stops), where number of stops is same as number of sides of that orbit level.
		// # of stops = (orbits list index val + 2)		//ie. 1st ring = 3 stops;	2nd ring = 4 stops;		3rd ring = 5 stops;
		
		float foundAngle = 360.0f / i;
		float foundAngleRads = glm::radians(foundAngle);		//points will be (cos(this angle, incremented) * radius, sin(this angle, incremented) * radius, 0);

		for (uint j = 0; j < i; j++) 
		{
			m_stopsList.push_back(vector3(cos(foundAngleRads * j) * fSize, sin(foundAngleRads * j) * fSize, 0));
		}

		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	// Initialize timer
	static float fTimer = 0;	// Store the new timer
	static uint uClock = m_pSystem->GenClock(); // Generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); // Get the delta time for that timer

	// Draw all shapes
	for (uint i = 0; i < m_uOrbits; ++i)	// We're lerping multiple objects, so loop application of lerp behavior
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		// Calculate the current position
		vector3 v3CurrentPos = ZERO_V3;

		// -------------------------
		
		// Set current sprint number, start, and end points		//THIS NEEDS TO BE MODIFIED SO EACH SPHERE FOLLOWS ONLY ITS OWN ORBIT
		static uint sprintCounter = 0;	// Keeps track of number of the current "sprint", ie. the path between the current and next stops.
		vector3 v3SprintStart = m_stopsList[sprintCounter];	// Sets starting point for the current sprint, using sprintCounter as an index for the stops vector.
		vector3 v3SprintEnd = m_stopsList[(sprintCounter + 1) % (m_stopsList.size() - ((i+1) + 2)) /*m_stopsList.size()*/];	/*	Sets the next stop as the end of the current sprint.
																						% used to loop to first stop if at end of stops vector. */
		
		// Calculate percentage to LERP by
		float fTimeBtwnStops = 1.0f;
		float fLerpPercentage = MapValue(fTimer, 0.0f, fTimeBtwnStops, 0.0f, 1.0f);		/*	Converts the ratio of [current elapsed time(fTimer) : fTimeBtwnStops]
																							into a percentage value, scaled between 0.0f and 1.0f. */
		
		// Set current position to lerp'd value
		v3CurrentPos = glm::lerp(v3SprintStart, v3SprintEnd, fLerpPercentage);

		// If percentage complete, reset lerp calculation variables
		if (fLerpPercentage >= 1.0f)
		{
			sprintCounter++;							// increment sprintCounter to set start of next sprint
			sprintCounter %= m_stopsList.size();		// if this was the last sprint, reset the sprint number to allow looping
			fTimer = m_pSystem->GetDeltaTime(uClock);	// reset timer to allow LERPing for next sprint
		}

		// -------------------------

		// Translate current indexed sphere by current pos
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		// Draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}