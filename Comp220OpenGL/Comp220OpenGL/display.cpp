#include "display.h"
#include <GL/glew.h>
#include <iostream>


Display::Display(int width, int height, const std::string& title)
{
	//If I need SDL for other things will have to change this chunk
	SDL_Init(SDL_INIT_EVERYTHING);

	//allocating space
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		//Size = display this many bits of shades(8=256)
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);	//This is how much of data in bits which SDL will allocate for a single pixel
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//allocate space for 2 windows that wont be displayed
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);//depth buffer


	//Creates Window using SDL
	m_window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,height,SDL_WINDOW_OPENGL);
	m_glContext =SDL_GL_CreateContext(m_window);

	//init glew
	//glewExperimental = GL_TRUE;

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "Glew Initialize fail" << std::endl;	//Error report if Glew fails to initialize
	}

	m_isClosed = false; //set boolean of is closed to false 

	/*the z buffer, at ever pixel your going to store how far way it is from cammera,
	and what it does when it draws it compares with any close and if it sitn there then draw it*/
	glEnable(GL_DEPTH_TEST);

	/*
	Cull face makes the behind of the visble not rendered to
	reduce the number of polygons the hardware has to draw.
	backface culling works for convex items
	*/
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}


Display::~Display()
{
	//When deconstructing, deconstruct things you made last first
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	//deintiatilses everything
	SDL_Quit;
}
//screen color window function
void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
}
//returns an IsCLosed boolean.
bool Display::IsClosed() {
	return m_isClosed;
}
void  Display::Update()
{
	//this swaps the buffers---- allows the window to never show something that is being drawn but only when completed
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;
	//look for any operating system events
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			m_isClosed = true; //set is closed to true
	}
}
void Display::light()
{
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[] = { .5, .5, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}