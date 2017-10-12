#include "DrawCircle.h"

void draw_circle(float centerX, float centerY, int radius, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx*dx + dy*dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
			}
		}
	}
}
