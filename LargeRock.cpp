#include "LargeRock.h"

#include <iostream>
#include <cstdlib>
using namespace std;


LargeRock:: LargeRock(SDL_Renderer* renderer, float x, float y)
{

	active = false;

	speed = 200.0;

	SDL_Surface* surface = IMG_Load("./RockAssets/BigRock.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	posRect.x = x;
	posRect.y = y;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	rockAngle = 0.0f;

	rockCenter.x = posRect.w / 2;
	rockCenter.y = posRect.h / 2;


}

//WRAPPING******
void LargeRock::Deactivate()
{
	active = false;

	posRect.x = -2000;

	posRect.y = -2000;

	pos_X = posRect.x;
	pos_Y = posRect.y;




}
// WRAPPING******



void LargeRock::Reposition()
{
	active = true;

	//location on screen 1 = left, 2 = right, 3 = top, 4 = bottom
	int location = rand() % 4 + 1;

	//movement of asteroid 1 = left or right, 2 up or down
	int direction = rand() % 2 + 1;

	if (location == 1)//left
	{
		//of screen of the left
		posRect.x = -posRect.w;
		pos_X = posRect.x;

		//random from top to bottom
		//number for random up and down
		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_Y = posRect.y;

		if (direction == 1)//move right and up
		{
			xDir = 1;
			yDir = -1;

		}
		else
		{
			xDir = 1;
			yDir = 1;
		}
	}
	else if (location == 2)//right
	{

		//of screen of the left
		posRect.x = 1024;
		pos_X = posRect.x;

		//random from top to bottom
		//number for random up and down
		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_Y = posRect.y;

		if (direction == 1)//move right and up
		{
			xDir = -1;
			yDir = -1;

		}
		else
		{
			xDir = -1;
			yDir = 1;
		}

	}
	else if (location == 3)//top
	{
		//of screen of the left
		posRect.y = -posRect.h;
		pos_Y = posRect.y;

		//random from top to bottom
		//number for random up and down
		int SideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % SideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == 1)//move right and up
		{
			xDir = -1;
			yDir = 1;

		}
		else
		{
			xDir = 1;
			yDir = 1;
		}
	}
	else if (location == 4)//bottom
	{
		//of screen of the left
		posRect.y =768;
		pos_Y = posRect.y;

		//random from top to bottom
		//number for random up and down
		int SideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % SideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == 1)//move right and up
		{
			xDir = -1;
			yDir = -1;

		}
		else
		{
			xDir = 1;
			yDir = -1;
		}
	}
}

void LargeRock::Update(float deltaTime)
{
	if (active)
	{
		//get rocks new pos
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;

		//adjust for precesion loss
		posRect.x = (int)pos_X + 0.5f;
		posRect.y = (int)pos_Y + 0.5f;

		rockAngle += .1;

		//WRAPPING**********
		if (posRect.x < (0 - posRect.w))
		{
			posRect.x = 1024;
			pos_X = posRect.x;
		}


		if (posRect.x > 1024)
		{
			posRect.x = (0 - posRect.w);
			pos_X = posRect.x;
		}

		if (posRect.y < (0 - posRect.h))
		{
			posRect.y = 768;
			pos_Y = posRect.y;
		}

		if (posRect.y > 768)
		{
			posRect.y = (0 - posRect.h);
			pos_Y = posRect.y;
		}





	}


}


void LargeRock::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);

}

LargeRock::~LargeRock()
{
	//SDL_DestroyTexture(texture);

}