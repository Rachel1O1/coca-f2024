//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "SDL2/SDL.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
void EmMainLoop(void* arg)
{
	Game* game = reinterpret_cast<Game*>(arg);
	game->EmRunIteration();
}
#endif

int main(int argc, char** argv)
{
	Game myGame;
	bool initResult = myGame.Initialize();
	if (initResult)
	{
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(EmMainLoop, &myGame, 0, true);
#else
		myGame.RunLoop();
#endif
	}
	myGame.Shutdown();
	return 0;
}