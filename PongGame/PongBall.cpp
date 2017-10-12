#include "PongBall.h"
#include "DrawCircle.h"

PongBall::PongBall()
{
	resetSpeed();
}

void PongBall::draw(SDL_Renderer * renderer)
{
	/* Get the Size of drawing surface */
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	radius = darea.w / 60;

	draw_circle(posX, posY, radius, renderer);
}

float PongBall::getCenterX()
{
	return posX;
}

void PongBall::flipX()
{
	deltaX = -deltaX;
	int max = 10;
	int min = -10;
	float randNum = (rand() % (max - min + 1) + min) * 0.01f;
	deltaY += randNum;
}

void PongBall::flipY()
{
	deltaY = -deltaY;
}

float PongBall::getCenterY()
{
	return posY;
}

float PongBall::getRadius()
{
	return radius;
}

float PongBall::getSpeedX()
{
	return deltaX;
}

float PongBall::getSpeedY()
{
	return deltaY;
}

void PongBall::resetPosition(SDL_Renderer* renderer)
{
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	setPosition(float(darea.w) / 2.0f, float(darea.h) / 2.0f);
}

void PongBall::resetSpeed()
{
	deltaX = defaultSpeedX;
	deltaY = defaultSpeedY;
}

void PongBall::setPosition(float x, float y)
{
	posX = x;
	posY = y;
}

void PongBall::updatePosition()
{
	posX += deltaX;
	posY += deltaY;
}

