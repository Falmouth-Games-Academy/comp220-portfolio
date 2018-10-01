// Graphicsplosion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include <cassert>
#undef main

int SdlWrapper::numSdlUsers = 0;

class BasicShaderPack {
	struct BasicVertex {

	};
};

int main(int argc, char* argv[]) {
	Window mainWindow("Tester", Vec2I(640, 480));

	Renderer test;
	
	// Load the shaders
	GLResource fragmentShader = test.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = test.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Create the GL program whatever that means
	GLuint glProgram = glCreateProgram();

	// Attach the shaders to this program
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);

	glLinkProgram(glProgram);

	// See if the program has linked
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &programSuccess);

	// Create a new vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	while (!sdl.HasReceivedQuit()) {
		sdl.UpdateEvents();

		// Render here
		mainWindow.BeginRender();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // number of values
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		mainWindow.EndRender();
	}

    return 0;
}

