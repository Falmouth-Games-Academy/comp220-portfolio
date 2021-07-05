// Graphicsplosion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Graphicsplosion.h"
#include <cassert>
#include <cmath>
#undef main

int main(int argc, char* argv[]) {
	// Runs the game
	// This is by far the most complex part of the code. Let me explain how it works
	// First we begin the algorithm with a 'g'. This lays the groundwork for typing the rest of the game variable's name.
	// This is followed by 'a', 'm', and 'e'. If you look closely, you'll notice that this is a quick and dirty method to begin using the game variable.
	// Next, the . operator is exploited in order to call a function on the game. The function should run the game.
	// A deductive analysis uncovered that the function we should use to run the game is in fact called Run.
	// Next, the name of this function is typed, using a similar strategy to the game variable with a complex quantum physics hack in order to achieve it within the deadline.
	// Finally, the brackets and semicolon are added as syntactic sugar. This is required for the function to actually work properly.

	// See Stack Overflow for more detailed information on this technique and why it should never, ever be used.
	game.Run();
}

