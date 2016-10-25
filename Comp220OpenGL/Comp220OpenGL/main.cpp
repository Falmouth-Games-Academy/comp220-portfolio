#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
int main(int arc, char ** argv)
{
	glewExperimental = GL_TRUE; 
	//glewInit();
	Display display(800, 600, "Comp220-1-OpenGL");
	
	Vertex vertices[] = {	
							Vertex(glm::vec3(-0.5,-0.5,0.0)	,glm::vec2(0.0,0.0)),
							Vertex(glm::vec3(0.0, 0.5, 0.0)	,glm::vec2(0.5,-1.0)),
							Vertex(glm::vec3(0.5,-0.5, 0.0)	,glm::vec2(1.0,0.0)),
						};

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	Shader shader(".\\res\\basicShader");
	Texture texture(".\\res\\croc.jpg");

	while(!display.IsClosed())
	{
		display.Clear(0.5f,0.15f, 0.5f, 1.0f);

		shader.Bind();
		texture.Bind(0);
		mesh.Draw();

		display.Update();
	}
	return 0;
}