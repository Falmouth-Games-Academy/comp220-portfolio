// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "comp220_portfolio.h"

void showErrorMessage(const char* message, const char* title)
{
	// Note: this is specific to Windows, and would need redefining to work on Mac or Linux
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

bool compileShader(GLuint shader_id, const std::string& shader_file_name)
{
	// Read the source code from the file
	std::string shader_location;
	std::ifstream source_stream(shader_file_name, std::ios::in);
	if (source_stream.is_open())
	{
		std::stringstream buffer;
		buffer << source_stream.rdbuf();
		shader_location = buffer.str();
		source_stream.close();
	}
	else
	{
		showErrorMessage(shader_file_name.c_str(), "File not found");
		return false;
	}

	// Compile the shader
	const char* source_pointer = shader_location.c_str();
	glShaderSource(shader_id, 1, &source_pointer, NULL);
	glCompileShader(shader_id);

	// Check the results of compilation
	GLint result = GL_FALSE;
	int info_log_length = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1)
	{
		// Display the compilation log
		std::vector<char> error_message(info_log_length + 1);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, error_message.data());
		showErrorMessage(error_message.data(), shader_file_name.c_str());
	}

	return (result != GL_FALSE);
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

	// Create the shaders
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertex_shader_id, vertex_file_path);

	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragment_shader_id, fragment_file_path);

	// Link the program
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	GLint result = GL_FALSE;
	int info_log_length = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		std::vector<char> error_message(info_log_length + 1);
		glGetProgramInfoLog(program_id, info_log_length, NULL, error_message.data());
		showErrorMessage(error_message.data(), "glLinkProgram error");
	}

	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		showErrorMessage("SDL_Init failed", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		showErrorMessage("SDL_CreateWindow failed", SDL_GetError());
		return 1;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	if (gl_context == nullptr)
	{
		showErrorMessage("SDL_GL_CreateContext failed", SDL_GetError());
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		showErrorMessage("glewInit failed", ":(");
	}

	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	static const GLfloat g_colour_buffer_data[] = {
		0.8f, 0.2f, 0.2f,
		0.2f, 0.8f, 0.2f,
		0.2f, 0.2f, 0.8f,
	};

	// This will identify our vertex buffer
	GLuint vertex_buffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertex_buffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_colour_buffer_data), g_colour_buffer_data, GL_STATIC_DRAW);

	GLuint program_id = loadShaders("vertex.glsl", "fragment.glsl");

	GLuint mvp_location = glGetUniformLocation(program_id, "mvp");

	bool running = true;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}

		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program_id);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		glm::mat4 view = glm::lookAt(glm::vec3(2, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f,0.1f,100.0f );


		// Draw the triangle !
		glm::mat4 transform;
		glm::mat4 mvp(projection * view * transform);
		
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
