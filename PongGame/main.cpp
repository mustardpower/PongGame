//Using SDL and standard IO
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "PongBall.h"
#include "Game.h"
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
Game game;

int main(int argc, char* args[])
{
	game.initialize();
	game.runGameLoop();

	SDL_Quit();
	TTF_Quit();

	return 0;
}