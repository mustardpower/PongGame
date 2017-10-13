#include "Game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <algorithm>

const int Game::WINDOW_WIDTH = 640;
const int Game::WINDOW_HEIGHT = 480;

Game::Game()
{
	players[0].reset(new Player());
	players[1].reset(new AIPlayer());
	selectedMenuItemIndex = 0;
	gameState = Menu;
}

Game::~Game()
{
}

void Game::checkScore()
{
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);

	//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "Checking score");
	if (ball.getCenterX() < 0)
	{
		ball.resetPosition(renderer);
		players[1]->incrementScore();
		ball.resetSpeed();
	}
	else if (ball.getCenterX() > darea.w)
	{
		ball.resetPosition(renderer);
		players[0]->incrementScore();
		ball.resetSpeed();
	}

	if (players[0]->getScore() >= WINNING_SCORE)
	{
		gameState = EndOfGame;
	}
	else if (players[1]->getScore() >= WINNING_SCORE)
	{
		gameState = EndOfGame;
	}
}

const int Game::getWindowHeight()
{
	return WINDOW_HEIGHT;
}

const int Game::getWindowWidth()
{
	return WINDOW_WIDTH;
}

void Game::handleEvents()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			gameState = Quiting;
			return;
		}

		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_ESCAPE)) {
			gameState = Quiting;
			return;
		}

		switch (gameState)
		{
			case Menu:
			{
				handleMenuEvent(e);
			}
			break;
			case Playing:
			{
				handleGameEvent(e);
			}
			break;
			case EndOfGame:
			{
				if (e.type == SDL_KEYDOWN)
				{
					if (e.key.keysym.sym == SDLK_SPACE)
					{
						gameState = Menu;
					}
				}
			}
		}
	}
}

void Game::handleGameEvent(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_DOWN)
		{
			players[0]->moveDown();
		}
		else if (e.key.keysym.sym == SDLK_UP)
		{
			players[0]->moveUp();
		}
	}
}

void Game::handleMenuEvent(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_DOWN)
		{
			selectedMenuItemIndex = (selectedMenuItemIndex + 1) % NUMBER_OF_MENU_CHOICES;
		}
		else if (e.key.keysym.sym == SDLK_UP)
		{
			selectedMenuItemIndex = (selectedMenuItemIndex - 1) % NUMBER_OF_MENU_CHOICES;
			if (selectedMenuItemIndex < 0)
			{
				selectedMenuItemIndex = NUMBER_OF_MENU_CHOICES - 1;
			}
		}
		else if (e.key.keysym.sym == SDLK_RETURN)
		{
			switch (selectedMenuItemIndex)
			{
			case 0:
			{
				gameState = Playing;
				resetGame();
			}
			break;
			case 1:
			{
				gameState = Quiting;
			}
			break;
			}
		}
	}
}

void Game::initialize()
{
	SDL_Surface *surface;

	/* Enable standard application logging */
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
		return;
	}

	/* Create window and renderer for given surface */
	window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n", SDL_GetError());
		return;
	}
	surface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateSoftwareRenderer(surface);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n", SDL_GetError());
		return;
	}

	/* Clear the rendering surface with the specified color */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	TTF_Init();

	resetGame();
}

bool Game::isColliding(Player player)
{
	/* THIS CODE CHECKS FOR A COLLISION BETWEEN THE CIRCLE (ball)
	AND THE SURFACE OF THE PLAYER'S BAT!
	ASSUMES THE SURFACE OF THE BAT IS VERTICAL */

	/* TODO:
		IGNORE COLLISIONS WHEN BALL IS BEHIND BAT
	*/

	if (ball.getRadius() <= 0.0f) { return false; }

	float lX, eX;
	float epsilon = ball.getRadius() / 2.0f;
	SDL_Rect rect = player.getRect(renderer);
	if (player.onLeftOfBall(renderer, ball))
	{
		lX = (float)(rect.x + rect.w);
		eX = (float)(rect.x + rect.w);
		auto collisionDepth = ball.getCenterX() - ball.getRadius() - lX;
		if (collisionDepth > epsilon)
		{
			return false;
		}
		if (ball.getSpeedX() > 0.0f)
		{
			return false;
		}
	}
	else
	{
		lX = (float)rect.x;
		eX = (float)rect.x;
		auto collisionDepth = ball.getCenterX() + ball.getRadius() - lX;
		if (collisionDepth > epsilon)
		{
			return false;
		}
		if (ball.getSpeedX() < 0.0f)
		{
			return false;
		}
	}

	float eY = (float)(rect.y + rect.h);
	float a = (rect.y - eY) * (rect.y - eY);
	float b = 2 * ((eY - ball.getCenterY()) * (rect.y - eY));
	float c = (((eX - ball.getCenterX()) * (eX - ball.getCenterX())) + ((eY - ball.getCenterY()) * (eY - ball.getCenterY()))) - (ball.getRadius() * ball.getRadius());

	float discriminant = b*b - 4 * a*c;
	if (discriminant >= 0)
	{
		// ray didn't totally miss sphere,
		// so there is a solution to
		// the equation.
		discriminant = sqrt(discriminant);

		// either solution may be on or off the ray so need to test both
		// t1 is always the smaller value, because BOTH discriminant and
		// a are nonnegative.
		float t1 = (-b - discriminant) / (2 * a);
		float t2 = (-b + discriminant) / (2 * a);

		// 3x HIT cases:
		//          -o->             --|-->  |            |  --|->
		// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

		// 3x MISS cases:
		//       ->  o                     o ->              | -> |
		// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

		if (t1 >= 0 && t1 <= 1)
		{
			// t1 is the intersection, and it's closer than t2
			// (since t1 uses -b - discriminant)
			// Impale, Poke
			return true;
		}

		// here t1 didn't intersect so we are either started
		// inside the sphere or completely past it
		if (t2 >= 0 && t2 <= 1)
		{
			// ExitWound
			return true;
		}
	}
	return false;
}

void Game::render()
{
	switch (gameState)
	{
		case Menu:
		{
			renderMenu();
		}
		break;
		case Playing:
		{
			renderGame();
		}
		break;
		case EndOfGame:
		{
			renderEndOfGame();
		}
		break;
	}
}

void Game::renderEndOfGame()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	SDL_Rect textLocationTitle = { (int)(darea.w * 0.45), (int)(darea.h * 0.05), 0, 0 };
	renderText("PONG!", textLocationTitle, { 0, 255, 0 });

	SDL_Rect textLocationReturnToMenu = { (int)(darea.w * 0.08), (int)(darea.h * 0.5), 0, 0 };
	renderText("PRESS SPACE TO RETURN TO MENU!", textLocationReturnToMenu, { 0, 255, 0 });

	renderScores();

	SDL_UpdateWindowSurface(window);
}

void Game::renderGame()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	SDL_Rect textLocationTitle = { (int)(darea.w * 0.45), (int)(darea.h * 0.05), 0, 0 };
	renderText("PONG!", textLocationTitle, { 0, 255, 0 });

	renderScores();
	renderPlayers();

	ball.draw(renderer);

	SDL_UpdateWindowSurface(window);
}

void Game::renderMenu()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	SDL_Rect darea;
	SDL_Color menuItemColor;
	SDL_RenderGetViewport(renderer, &darea);

	SDL_Rect textLocationPlay = { (int)(darea.w * 0.45), (int)(darea.h * 0.4), 0, 0 };
	menuItemColor = selectedMenuItemIndex == 0 ? SDL_Color{ 255, 0, 0 } : SDL_Color{ 0, 255, 0 };
	renderText("PLAY", textLocationPlay, menuItemColor);

	SDL_Rect textLocationQuit = { (int)(darea.w * 0.45), (int)(darea.h * 0.6), 0, 0 };
	menuItemColor = selectedMenuItemIndex == 1 ? SDL_Color{ 255, 0, 0 } : SDL_Color{ 0, 255, 0 };
	renderText("QUIT", textLocationQuit, menuItemColor);

	SDL_UpdateWindowSurface(window);
}

void Game::renderPlayers()
{
	players[0]->draw(renderer);
	players[1]->draw(renderer);
}

void Game::renderScores()
{
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	SDL_Rect textLocationPlayer0 = { (int)(darea.w * 0.1), (int)(darea.h * 0.05), 0, 0 };
	renderText(std::to_string(players[0]->getScore()), textLocationPlayer0, { 0, 255, 0 });

	SDL_Rect textLocationPlayer1 = { (int)(darea.w * 0.9), (int)(darea.h * 0.05), 0, 0 };
	renderText(std::to_string(players[1]->getScore()), textLocationPlayer1, { 0, 255, 0 });

	SDL_Rect winningScorePromptTextLocation = { (int)(darea.w * 0.35), (int)(darea.h * 0.4), 0, 0 };
	if (players[0]->getScore() >= WINNING_SCORE)
	{
		renderText("PLAYER 1 WINS!", winningScorePromptTextLocation, { 0, 255, 0 });
	}
	else if (players[1]->getScore() >= WINNING_SCORE)
	{
		renderText("PLAYER 2 WINS!", winningScorePromptTextLocation, { 0, 255, 0 });
	}
}

void Game::renderText(string text, SDL_Rect textLocation, SDL_Color foregroundColor)
{
	const int fontSize = 30;
	TTF_Font* font = TTF_OpenFont("ARIAL.TTF", fontSize);
	if (!font)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Font loading failed with error : %s\n", SDL_GetError());
	}
	else
	{
		SDL_Color backgroundColor = { 0, 0, 0 };
		SDL_Surface* textSurface = TTF_RenderText_Shaded(font, text.c_str(), foregroundColor, backgroundColor);
		
		// Pass zero for width and height to draw the whole surface
		SDL_Surface* screen = SDL_GetWindowSurface(window);
		SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
		
		SDL_FreeSurface(textSurface);
		TTF_CloseFont(font);
	}
}

void Game::resetGame()
{
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	players[0]->resetScore();
	players[1]->resetScore();
	players[0]->setPosition(darea.w / 8, darea.h / 2);
	players[1]->setPosition((7 * darea.w) / 8, darea.h / 2);
	ball.resetPosition(renderer);
	ball.resetSpeed();
}

void Game::runGameLoop()
{
	while (gameState != Quiting)
	{
		update();
		render();
	}
}

void Game::update()
{
	handleEvents();

	if (gameState == Playing)
	{
		updateGame();
	}
}

void Game::updateBall()
{
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	const int WINDOW_HEIGHT = darea.h;

	if (isColliding(*players[0]))
	{
		isColliding(*players[0]);
		ball.flipX();
	}
	else if (isColliding(*players[1]))
	{
		ball.flipX();
	}
	else if (ball.getCenterY() + ball.getRadius() > WINDOW_HEIGHT)
	{
		ball.flipY();
	}
	else if (ball.getCenterY() - ball.getRadius() < 0)
	{
		ball.flipY();
	}

	ball.updatePosition();
}

void Game::updateGame()
{
	updateBall();
	updatePlayers();
	checkScore();
}

void Game::updatePlayers()
{
	players[0]->update(renderer, ball);
	players[1]->update(renderer, ball);
}