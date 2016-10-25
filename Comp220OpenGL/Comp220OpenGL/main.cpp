#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
int main(int arc, char ** argv)
{
	glewExperimental = GL_TRUE; 
	//glewInit();
	Display display(800, 600, "Comp220-1-OpenGL");
	Shader shader(".\\res\\basicShader");
	Vertex vertices[] = {	
							Vertex(glm::vec3(-0.5,-0.5,0)),
							Vertex(glm::vec3(0, 0.5, 0)),
							Vertex(glm::vec3(0.5, -0.5, 0)),
						};


	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));



	while(!display.IsClosed())
	{
		display.Clear(0.5f,0.15f, 0.5f, 1.0f);
		shader.Bind();
		mesh.Draw();

		display.Update();
	}
	return 0;
}