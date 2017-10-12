#pragma once
#include <SDL.h>
#include <memory>

class PongBall
{
private:
	const float defaultSpeedX = -0.31f;
	const float defaultSpeedY = 0.075f;
	float posX;
	float posY;
	float radius;
	float deltaY;
	float deltaX;
public:
	PongBall();
	void draw(SDL_Renderer* renderer);
	float getCenterX();
	float getCenterY();
	float getRadius();
	float getSpeedX();
	float getSpeedY();
	void flipX();
	void flipY();
	void resetPosition(SDL_Renderer* renderer);
	void resetSpeed();
	void setPosition(float x, float y);
	void updatePosition();
};