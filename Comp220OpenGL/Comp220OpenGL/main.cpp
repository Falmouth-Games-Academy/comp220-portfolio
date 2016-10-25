#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

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
	Transform transform;

	float counter = 0.0f;

	while(!display.IsClosed())
	{
		display.Clear(0.5f,0.15f, 0.5f, 1.0f);

		float sinCounter = sinf(counter);
		float cosCounter = cosf(counter);

		transform.GetPos().x = sinf(counter);//makes it go left to right
		transform.GetRot().z = counter ;	//makes it rotate
		transform.SetScale(glm::vec3(cosCounter, cosCounter, cosCounter)); //makes it look like its going away and coming back

		shader.Bind();
		texture.Bind(0);
		shader.Update(transform);
		mesh.Draw();

		display.Update();
		counter += 0.02;

	}
	return 0;
}