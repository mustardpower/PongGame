#pragma once
#include "Player.h"

class AIPlayer : public Player
{
public:
	const float deltaY();
	void update(SDL_Renderer* renderer, PongBall ball);
};