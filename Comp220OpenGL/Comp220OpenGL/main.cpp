#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "objLoader.h"
#define WIDTH 800
#define HEIGHT 600



int main(int arc, char ** argv)
{
	glewExperimental = GL_TRUE; 
	//glewInit();

	//better way is to go into display class and have a get width/height function
	Display display(WIDTH, HEIGHT, "Comp220-1-OpenGL");
	
	Vertex vertices[] = {	
							Vertex(glm::vec3(-0.5,-0.5,0.0)	,glm::vec2(0.0,0.0)),
							Vertex(glm::vec3(0.0, 0.5, 0.0)	,glm::vec2(0.5,-1.0)),
							Vertex(glm::vec3(0.5,-0.5, 0.0)	,glm::vec2(1.0,0.0)),
						};

	unsigned int indices[] = { 0,1,2 };

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]),indices, sizeof(indices)/ sizeof(indices)[0]);
	Mesh mesh2(".\\res\\croco.obj");		//http://tf3dm.com/download-page.php?url=crocodile-27619 croco .obj 
	Shader shader(".\\res\\basicShader");
	Texture texture(".\\res\\green.jpg");
	Camera camera(glm::vec3(0,0,-4),70.0f,(float)WIDTH/(float)HEIGHT,0.01f,1000.0f);
	Transform transform;

	float counter = 0.0f;
	glm::vec3 eyePosition(0, 0, 10);
	
	while(!display.IsClosed())
	{
		display.Clear(0.5f,0.15f, 0.5f, 1.0f);
		
		float sinCounter = sinf(counter);
		float cosCounter = cosf(counter);

		//movement controlls
		//transform.GetPos().x = sinCounter;		//makes it go left to right
		//transform.GetPos().z = cosCounter ;			//makes it rotate
		transform.GetRot().x = counter ;
		//transform.GetRot().y = counter ;
		//transform.GetRot().z = counter ;



		//transform.SetScale(glm::vec3(cosCounter, cosCounter, cosCounter)); //makes it look like its going away and coming back

		shader.Bind();
		texture.Bind(0);


		shader.Update(transform,camera);


		mesh2.Draw();
		
		
		display.Update();
		

		counter += 0.001;

	}
	return 0;
}