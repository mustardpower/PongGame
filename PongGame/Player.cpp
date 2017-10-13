#include "Player.h"
#include "Game.h"

Player::Player()
{
	positionX = 0;
	positionY = 0;
	score = 0;
}

Player::~Player()
{
}

const float Player::deltaY()
{
	return 5.0f;
}

void Player::draw(SDL_Renderer* renderer)
{
	SDL_Rect rect = getRect(renderer);
	SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Player::getRect(SDL_Renderer* renderer) const
{
	SDL_Rect rect, darea;

	/* Get the Size of drawing surface */
	SDL_RenderGetViewport(renderer, &darea);

	SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);

	rect.w = darea.w / 40;
	rect.h = darea.h / 6;
	rect.x = positionX;
	rect.y = positionY;
	return rect;
}

void Player::incrementScore()
{
	score++;
}

float Player::getPositionX() const
{
	return positionX;
}

float Player::getPositionY() const
{
	return positionY;
}
int Player::getScore()
{
	return score;
}
bool Player::isBallAbovePlayer(SDL_Renderer* renderer, PongBall ball)
{
	float centreY = ball.getCenterY();
	return ball.getCenterY() - getPositionY() < 0;
}

bool Player::onLeftOfBall(SDL_Renderer* renderer, PongBall ball) const
{
	return getPositionX() - ball.getCenterX() < 0;
}

void Player::moveDown()
{
	if (positionY + deltaY() < Game::getWindowHeight())
	{
		positionY += deltaY(); 
	}
}

void Player::moveUp()
{
	if (positionY + deltaY() > 0)
	{
		positionY -= deltaY();
	}
}

void Player::resetScore()
{
	score = 0;
}

void Player::setPosition(int x, int y)
{
	positionX = x;
	positionY = y;
}

void Player::update(SDL_Renderer* renderer, PongBall ball)
{
}