#pragma once
#include <SDL.h>
#include "PongBall.h"

class Player
{
private:
	float positionX;
	float positionY;
	int score;
public:
	Player();
	~Player();
	virtual const float deltaY();
	void draw(SDL_Renderer* renderer);
	SDL_Rect getRect(SDL_Renderer* renderer) const;
	void incrementScore();
	void moveDown();
	void moveUp();
	float getPositionX() const;
	float getPositionY() const;
	int getScore();
	bool isBallAbovePlayer(SDL_Renderer* renderer, PongBall ball);
	bool onLeftOfBall(SDL_Renderer* renderer, PongBall ball) const;
	void resetScore();
	void setPosition(int x, int y);
	virtual void update(SDL_Renderer* renderer, PongBall ball);
};