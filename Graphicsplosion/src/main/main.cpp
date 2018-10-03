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

class Vec4 {
public:
	Vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};

public:
	float x, y, z, w;
};

class Matrix {
public:
	Matrix() = default;
	Matrix(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w) :
		m11(x.x), m21(x.y), m31(x.z), m41(x.w), m12(y.x), m22(y.y), m32(y.z), m42(y.w), m13(z.x), m23(z.y), m33(z.z), m43(z.w),
		m14(w.x), m24(w.y), m34(w.z), m44(w.w) {};

	// Identity constructor
	static Matrix Matrix::FromIdentity() {
		return Matrix(Vec4(1.0f, 0.0f, 0.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 1.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

public:
	union {
		struct {
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};

		float m[4][4];
	};

public:
	// Matrix multiplication operator
	void operator*=(const Matrix& other) {
		Matrix temp{};
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				for (int s = 0; s < 4; s++) {
					temp.m[r][c] += m[0][s] * other.m[s][0];
				}
			}
		}

		*this = temp;
	}
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // number of values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Set the world transform
	Matrix matWorld(Vec4(1.0f, 0.0f, 0.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 1.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	int uniMatWorld = glGetUniformLocation(shaderProgram.GetGlProgram(), "matWorld");

	float angle = 0.0f;

	while (!sdl.HasReceivedQuit()) {
		sdl.UpdateEvents();

		// Render here
		mainWindow.BeginRender();

		// Use our test shader
		test.UseShaderProgram(shaderProgram);

		// Test rotate the triangle
		angle += 0.1f;

		matWorld.m11 = cos(angle);
		matWorld.m21 = -sin(angle);
		matWorld.m12 = sin(angle);
		matWorld.m22 = cos(angle);

		/*

		*/

		Matrix test = Matrix::FromIdentity();

		test.m11 = cos(angle);
		test.m31 = -sin(angle);
		test.m13 = sin(angle);
		test.m33 = cos(angle);

		matWorld *= test;

		// Upload the world transform
		glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, (GLfloat*)&matWorld);

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

