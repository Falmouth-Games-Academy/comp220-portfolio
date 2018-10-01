// Graphicsplosion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include <cassert>
#undef main

int SdlWrapper::numSdlUsers = 0;

int main(int argc, char* argv[]) {
	Window mainWindow("Tester", Vec2I(640, 480));

	Renderer test;
	
	test.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	test.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	while (!sdl.HasReceivedQuit()) {
		sdl.UpdateEvents();

		mainWindow.Render();
	}

    return 0;
}

