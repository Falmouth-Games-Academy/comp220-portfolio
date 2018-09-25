// Graphicsplosion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include <cassert>
#undef main

int SdlWrapper::numSdlUsers = 0;

int main(int argc, char* argv[]) {
	Window mainWindow("Tester", Vec2I(640, 480));

	while (!sdl.HasReceivedQuit()) {
		sdl.UpdateEvents();
	}

    return 0;
}

