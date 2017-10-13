#pragma once
#include "AIPlayer.h"
#include "PongBall.h"
#include <string>
#include <memory>
#include <array>

using namespace std;

enum GameState
{
	Menu, Playing, EndOfGame, Quiting
};

class Game
{
private:
	GameState gameState;
	PongBall ball;
	std::array<std::unique_ptr<Player>, 2> players;
	SDL_Renderer *renderer;
	int selectedMenuItemIndex;
	SDL_Window* window;
	const int WINNING_SCORE = 11;
	const int NUMBER_OF_MENU_CHOICES = 2;
	static const int WINDOW_HEIGHT;
	static const int WINDOW_WIDTH;

public:
	Game();
	~Game();
	void checkScore();
	void handleEvents();
	void initialize();
	bool isColliding(Player player);
	static const int getWindowWidth();
	static const int getWindowHeight();
	void handleGameEvent(SDL_Event anEvent);
	void handleMenuEvent(SDL_Event anEvent);
	void render();
	void renderEndOfGame();
	void renderGame();
	void renderMenu();
	void renderPlayers();
	void renderScores();
	void renderText(string text, SDL_Rect textLocation, SDL_Color foregroundColor);
	void resetGame();
	void runGameLoop();
	void update();
	void updateBall();
	void updateGame();
	void updatePlayers();
};