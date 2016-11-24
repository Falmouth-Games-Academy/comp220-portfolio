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
	//some functions are unable to work without glew experimental on.
	glewExperimental = GL_TRUE; 
	//glewInit();

	//better way is to go into display class and have a get width/height function
	Display display(WIDTH, HEIGHT, "Comp220-1-OpenGL");
	//drawing the points for the vertices.
	Vertex vertices[] = {	
							Vertex(glm::vec3(-0.5,-0.5,0.0)	,glm::vec2(0.0,0.0)),
							Vertex(glm::vec3(0.0, 0.5, 0.0)	,glm::vec2(0.5,-1.0)),
							Vertex(glm::vec3(0.5,-0.5, 0.0)	,glm::vec2(1.0,0.0)),
						};
	unsigned int indices[] = { 0,1,2 };

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]),indices, sizeof(indices)/ sizeof(indices)[0]);
	Mesh mesh2(".\\res\\croco.obj");		//http://tf3dm.com/download-page.php?url=crocodile-27619 //load mesh obj of crocodile
	Shader shader(".\\res\\basicShader");	//Load the basic shader for the lighting.
	Texture texture(".\\res\\green.jpg");	//Load the texture for the croc obj
	Camera camera(glm::vec3(0,0,-4),70.0f,(float)WIDTH/(float)HEIGHT,0.01f,1000.0f); //set camera viewport
	Transform transform;					

	float mover = 0.0f;//used for the transform.
	glm::vec3 eyePosition(0, 0, 10);

	while(!display.IsClosed())
	{
		display.Clear(0.5f,0.15f, 0.5f, 1.0f);//set the color of the window.
		
		float sinemover = sinf(mover);
		float cosmover = cosf(mover);

		//movement controlls-Change for different transforms after execution
		transform.GetPos().x = sinemover;		//makes it go left to right
		//transform.GetPos().z = cosmover ;		//makes it rotate
		transform.GetRot().x = mover;			//makes it roll forwards
		transform.GetRot().y = mover ;		//makes it spin 
		//transform.GetRot().z = mover;			//makes it rotate to the right

		//uncomment above for different transforms

		//transform.SetScale(glm::vec3(cosmover, cosmover, cosmover)); //makes it look like its going away and coming back

		shader.Bind();		//bind the shader
		texture.Bind(0);	//bind the texture


		shader.Update(transform,camera);


		mesh2.Draw();
		
		
		
		display.Update();				


		mover += 0.005; //change this variable for the speed of transform.

	}
	return 0;
}