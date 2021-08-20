#include "bullet.h"

Bullet::Bullet(SDL_Renderer* renderer, float x, float y)
{

	active = false;

	speed = 800.0;

	SDL_Surface* surface = IMG_Load("./RockAssets/Projectile.png");

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

}

//WRAPPING******

void Bullet::Deactivate()
{
	active = false;

	posRect.x = -3000;

	posRect.y = -3000;

	pos_X = posRect.x;
	pos_Y = posRect.y;




}
//WRAPPING*****

void Bullet::Reposition()
{
	if (xDir == 1 && yDir == 0)
	{
		posRect.x += (40);
		posRect.y += (15);
	}
	else if (xDir == -1 && yDir == 0)
	{
		posRect.x -= 8;
		posRect.y += (14);
	}
	else if (xDir == 0 && yDir == 1)
	{
		posRect.x += 25;
		posRect.y += 38;
	}
	else if (xDir == 0 && yDir == -1)
	{
		posRect.x += 20;
		posRect.y += 16;
	}
	else if (xDir == 1 && yDir == -1)
	{
		posRect.x += 40;
		posRect.y -= 10;
	}
	else if (xDir == -1 && yDir == -1)
	{
		posRect.x -= 6;
		posRect.y -= 10;
	}
	else if (xDir == 1 && yDir == 1)
	{
		posRect.x += 40;
		posRect.y += 32;
	}
	else if (xDir == -1 && yDir == 1)
	{
		posRect.x -= 2;
		posRect.y += 32;
	}

	pos_X = posRect.x;
	pos_Y = posRect.y;


}





void Bullet::Update(float deltaTime)
{
	
	if (active)
	{
		//get bullets new pos
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;

		//adjust for precesion loss
		posRect.x = (int)pos_X + 0.5f;
		posRect.y = (int)pos_Y + 0.5f;

		//check if the bullets of the screen
		if (posRect.x < 0 || posRect.x > 1024 || posRect.y < 0 || posRect.y > 768)
		{

			active = false; 

			posRect.x = -1000;
			posRect.y = -1000;

			pos_X = posRect.x;
			pos_Y = posRect.y;




		}


	}

}

void Bullet::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);

}