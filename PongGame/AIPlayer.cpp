#include "AIPlayer.h"

const float AIPlayer::deltaY()
{
	return 0.2f;
}

void AIPlayer::update(SDL_Renderer* renderer, PongBall ball)
{
	/* TO DO:
		IMPROVE LOGIC SO THAT THE AI MOVES TOWARDS WHERE THE BALL IS GOING
		CURRENTLY FAILS TO DEAL WITH STEEP VERTICAL MOVEMENT OF THE BALL
	*/
	SDL_Rect darea;
	SDL_RenderGetViewport(renderer, &darea);
	const int WINDOW_HEIGHT = darea.h;

	auto a = (getPositionX() - ball.getCenterX()) / ball.getSpeedX();
	auto py = a * ball.getSpeedY() + ball.getCenterY();

	if (py < 0)
	{
		// FIND X VALUE WHERE Y = 0

		a = -ball.getCenterY() / ball.getSpeedY(); // a is distance travelled when y = 0
		
		auto px = a * ball.getSpeedX() + ball.getCenterX(); // point where ball bounces given by (px, 0)

		a = (px - ball.getCenterX()) / ball.getSpeedX();
		py = a * - ball.getSpeedY();
		
	}
	else if (py > WINDOW_HEIGHT)
	{
		// FIND X VALUE WHERE Y = WINDOW_HEIGHT
		a = (WINDOW_HEIGHT - ball.getCenterY()) / ball.getSpeedY(); // a is distance travelled when y = WINDOW_HEIGHT

		auto px = a * ball.getSpeedX() + ball.getCenterX(); // point where ball bounces given by (px, WINDOW_HEIGHT)

		a = (px - ball.getCenterX()) / ball.getSpeedX();
		py = a * -ball.getSpeedY() + WINDOW_HEIGHT;
	}

	/* line of ball given by 
	
	a * ball_gradient + ball_centre

	a *	| x1 |	 | x2 |
		| y1 | + | y2 | 

	a *	| x1 |	 | x2 |  =  | px |
		| y1 | + | y2 |	    | py |

	// player is positioned on vertical line x = getPositionX();

	// find out a when ball crosses the player position

	// a * x1 + x2 = px
	//  a = (px - x2) / x1
	// py = a * y1 + y2
	
	*/

	auto middleOfBat = getPositionY() + (getRect(renderer).h / 2);
	if (py - middleOfBat < 0)
	{
		moveUp();
	}
	else
	{
		moveDown();
	}
}