// Graphicsplosion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include <cassert>
#include <cmath>
#undef main

int SdlWrapper::numSdlUsers = 0;

class BasicShaderProgram {
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
	ShaderProgram shaderProgram(test, vertexShader, fragmentShader);
	bool shaderProgramWorked = shaderProgram.IsLoaded();
	
	// Create a new vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // number of values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	while (!sdl.HasReceivedQuit()) {
		sdl.UpdateEvents();

		// Rotate the triangle lol because we can
		for (int i = 0; i < sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]); i += 3) {
			float x = g_vertex_buffer_data[i + 0], y = g_vertex_buffer_data[i + 1];

			g_vertex_buffer_data[i + 0] = x * cos(0.1f) - y * sin(0.1f);
			g_vertex_buffer_data[i + 1] = x * sin(0.1f) + y * cos(0.1f);
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

		// Render here
		mainWindow.BeginRender();

		// Use our test shader
		test.UseShaderProgram(shaderProgram);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		// Done!
		mainWindow.EndRender();
	}

	glDeleteBuffers(1, &vertexbuffer);


    return 0;
}

