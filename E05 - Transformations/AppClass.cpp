#include "AppClass.h"
void Application::InitVariables(void)
{
	// Make MyMesh object		// According to the C11 Translation and Scale demo, I can use the same MyMesh instance to generate multiple shapes, instead of calling a new MyMesh every time.
	m_pMesh = new MyMesh();
	
	// Generates all 46 cubes needed to construct the Space Invader shape
	for (int i = 0; i < 46; i++) 
	{
		m_pMesh->GenerateCube(2.0f, C_BROWN);
		spaceInvaderCubes.push_back(m_pMesh);
	}

	// APPLY POSITIONS FOR EACH CUBE
	GenerateSpaceInvader();
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

	// Taken from the C11 Translation and Scale demo
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	// SCALE						// When I change the scale, both the cubes and the spaces between them scale. This tells me the coordinate system for each indivual cube is also getting scaled. Not what I want.
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));	// Will be the same for all cubes

	// TRANSLATION VARIABLES
	static float value = 0.0f;		// Frame animation counter, static to increment per display iteration
	static float direction = 0.1f;	// Directional variable, will handle flipping the space invader's movement direction

	if (glm::abs(value) > 10.0f)	// From glm documentation
	{
		direction *= -1.0f;			// When value hits absVal(10), direction flips.
	}
	value += direction;				// The actual math behind the position animation

	// Renders all 46 cubes. Cube placement determined from meshPositions vector, which is populated in GenerateSpaceInvader()
	for (int i = 0; i < 46; i++)
	{
		// TRANSLATION APPLICATION
		matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(meshPositions[i].x + value, meshPositions[i].y, meshPositions[i].z));		// x is altered each frame, and is (x + value).
		
		matrix4 m4Model = m4Scale * m4Translate;
		spaceInvaderCubes[i]->Render(m4Projection, m4View, m4Model);
	}

	// Draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	// Render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	// Clear the render list
	m_pMeshMngr->ClearRenderList();
	
	// Draw gui
	DrawGUI();
	
	// End the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::GenerateSpaceInvader(void) 
{
	int spread = 2.0f;	// A factor by which to multiply the x and y position values of each cube. This shows me the cubes were placed correctly relatively to each other,
						// so the rendering issue was due to cube scale + finding the right spread factor.

	// 8 ROWS, 11 MAX COLUMNS

	// Row 0	//Top
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, 4.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, 3.0f, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(2.0f, 3.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, 4.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));
	
	// Row 1
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-3.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f * spread, 3.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f * spread, 3.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(3.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 2
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-2.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-1.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(0.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(1.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(2.0f * spread, 2.0f * spread, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, 2.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(4.0f, 0.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 3	// "Eyes"
	//meshPositions.push_back(vector3(-5.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-4.0f * spread, 1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, 1.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f * spread, 1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(0.0f * spread, 1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(1.0f * spread, 1.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(2.0f, 0.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, 1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(4.0f * spread, 1.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(5.0f, 0.0f, 0.0f));

	// Row 4	// "Center"
	meshPositions.push_back(vector3(-5.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-4.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(-1.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(0.0f,0.0f,0.0f));	// "dead center"
	meshPositions.push_back(vector3(1.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(2.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(4.0f * spread, 0.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f * spread, 0.0f, 0.0f));

	// Row 5
	meshPositions.push_back(vector3(-5.0f * spread, -1.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-2.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-1.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(0.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(1.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(2.0f * spread, -1.0f * spread, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, -1.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f * spread, -1.0f * spread, 0.0f));

	// Row 6
	meshPositions.push_back(vector3(-5.0f * spread, -2.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(-3.0f * spread, -2.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(-2.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(-1.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(0.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(1.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(2.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(3.0f * spread, -2.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(5.0f * spread, -2.0f * spread, 0.0f));

	// Row 7	//Bottom
	//meshPositions.push_back(vector3(-5.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(-4.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(-3.0f, -2.0f, 0.0f));
	meshPositions.push_back(vector3(-2.0f * spread, -3.0f * spread, 0.0f));
	meshPositions.push_back(vector3(-1.0f * spread, -3.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(0.0f, -1.0f, 0.0f));
	meshPositions.push_back(vector3(1.0f * spread, -3.0f * spread, 0.0f));
	meshPositions.push_back(vector3(2.0f * spread, -3.0f * spread, 0.0f));
	//meshPositions.push_back(vector3(3.0f, -2.0f, 0.0f));
	//meshPositions.push_back(vector3(4.0f, -1.0f, 0.0f));
	//meshPositions.push_back(vector3(5.0f, -2.0f, 0.0f));
}
void Application::Release(void)
{
	// DELETE ALL CUBES HERE
	// in theory, go through each member of mesh pointer vector,
		// de-ref the pointer, and deallocate the 'pointee'
		// set pointer to nullptr to avoid dangling

	//std::for_each(spaceInvaderCubes.begin(), spaceInvaderCubes.end(), delete);	//can't do this

	/*for (int i = 0; i < 46; i++)				// breaks after first loop.	// I only called 'new' once so this isn't necessary.
	{
		if (spaceInvaderCubes[i] != nullptr)
		{
			delete spaceInvaderCubes[i];
			spaceInvaderCubes[i] = nullptr;
		}
	}*/

	spaceInvaderCubes.clear();			// clear all 46 cubes        //does this eliminate just the stored pointers, or the pointers and their objects?
	meshPositions.clear();				// clear the 46 positions for the cubes
	SafeDelete(m_pMesh);				// deallocate the mesh rendering object
	
	// Release GUI
	ShutdownGUI();
}