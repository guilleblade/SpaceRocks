#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

//boolean to maintain program loop
bool quit = false;

//deltatime init() - for frame rate ind.
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

//variables for players angle
float x, y, playerAngle, oldAngle;

//for precision loss
float pos_X, pos_Y;

//floats for direction
float xDir, xDirOld;
float yDir, yDirOld;

//player speed
float playerSpeed = 400.0f;

//create rectangles for the new graphics
SDL_Rect playerPos;

//players center point
SDL_Point center;

//bullets
#include <vector>
#include "bullet.h"

vector<Bullet> bulletList;

//delcare sounds
Mix_Chunk* laser;
Mix_Chunk* explosion;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = pos_X;

			bulletList[i].posRect.y = pos_Y;

			bulletList[i].pos_X = pos_X;

			bulletList[i].pos_Y = pos_Y;

			bulletList[i].xDir = xDirOld;

			bulletList[i].yDir = yDirOld;

			bulletList[i].Reposition();

			break;
		}


	}

	

}

#include <iostream>
#include <cstdlib>
using namespace std;
#include "LargeRock.h"

vector<LargeRock> LargeRockList;

//small rocks
#include "SmallRock.h"

vector<SmallRock> SmallRockList;


int main(int argc, char* argv[])
{
	srand(time(NULL));

	SDL_Window* window;		//delcare a pointer

	//create a renderer variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);		//initialize SDL2

	//create an application window with the following settings
	window = SDL_CreateWindow(

		"Space Rocks",	//window title
		SDL_WINDOWPOS_UNDEFINED,	//initial x position
		SDL_WINDOWPOS_UNDEFINED,	//initial y position
		1024,						//width in pixels
		768,						//height in pixels
		SDL_WINDOW_OPENGL			//flags - see below

	);

	//check that the window was successfully created
	if (window == NULL)
	{
		printf("Could not create window %s\n", SDL_GetError());
		return 1;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//background image --- CREATE
	SDL_Surface* surface = IMG_Load("./RockAssets/Background.png");

	//create bkd texture
	SDL_Texture* bkgd;

	//place surface into the texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	//create the rectangles for the new graphics
	SDL_Rect bkgdPos;

	//set bkgdPos x, y, width and height
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;
	

	//Background image --- CREATE END

	//player image --- CREATE

	//create a SDL Surface
	surface = IMG_Load("./RockAssets/Player.png");

	//create player texture
	SDL_Texture* player;

	//place surface into the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free surface

	SDL_FreeSurface(surface);

	//set pos
	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 61;
	playerPos.h = 48;

	//center of the players sprite
	center.x = playerPos.w / 2;
	center.y = playerPos.h / 2;

	//pos_x and pos_y for precesion loss
	pos_X = playerPos.x;
	pos_Y = playerPos.y;

	//the player graphic is facing right so the xDirold is set to 1 no so bullets fo in correct direction
	xDir = 1;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;






	//player image --- CREATE END

	//SDL Event to handle input
	SDL_Event event;

	//create our bullet list
	for (int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, -1000.0f, -1000.0f);

		bulletList.push_back(tempBullet);

	}

	////creat our rock list
	//for (int i = 0; i < 1; i++)
	//{
	//	LargeRock tempRock(renderer, -1000.0f, -1000.0f);

	//	LargeRockList.push_back(tempRock);

	//}

	//for (int i = 0; i < 2; i++)
	//{
	//	SmallRock tempRock(renderer, -1000.0f, -1000.0f);

	//	SmallRockList.push_back(tempRock);

	//}

	//init audio play back
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//add laser sound
	laser = Mix_LoadWAV("./RockAssets/laser.WAV");
	explosion = Mix_LoadWAV("./RockAssets/retroExplosion.WAV");

	//LEVEL UP *************
	//rocks to start game
	int numberOfLargeRocks = 1;
	int numberOfSmallRocks = 2;

	//total rocks on levels
	int totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

	//total rocks currently destroyed 
	int totalRocksDestroyed = 0;

	/*int lvlCount = 1;
	int playerScore = 0;*/

	//creat our rock list
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		LargeRock tempRock(renderer, -1000.0f, -1000.0f);

		LargeRockList.push_back(tempRock);

	}

	for (int i = 0; i < numberOfSmallRocks; i++)
	{
		SmallRock tempRock(renderer, -1000.0f, -1000.0f);

		SmallRockList.push_back(tempRock);

	}

	//activate all large rocks for level 1
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		LargeRockList[i].Reposition();

	}

	//string currentLevel = to_string(lvlCount);


	//cout << "\tYou have now entered level " + currentLevel << endl;


	//basic game loop
	while (!quit)
	{
		
		//create deltaTime
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		//string currentLevel = to_string(lvlCount);


		
		//check for input for closing window and firing
		if (SDL_PollEvent(&event))
		{

			//close window by the windows x button
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}


			switch (event.type)
			{

				//look for keypress and release
			case SDL_KEYUP:

				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					CreateBullet();
					break;
				//case SDLK_z:
				//	LargeRockList[0].Reposition();
				//	break;
					//case SDLK_s:
					//{
					//	for (int i = 0; i < 2; i++)
					//	{
					//		if (SmallRockList[i].active == false)
					//		{
					//			SmallRockList[i].Reposition(LargeRockList[0].posRect.x, LargeRockList[0].posRect.y);
					//		}
					//	}
					//	//clear large rock
					//	LargeRockList.clear();

					//	break;
					//}
					//case SDLK_a:
					//{
					//	//clear large rock
					//	LargeRockList.clear();
					//	SmallRockList.clear();

					//	for (int i = 0; i < 1; i++)
					//	{
					//		LargeRock tempRock(renderer, -1000.0f, -1000.0f);

					//		LargeRockList.push_back(tempRock);

					//	}

					//	for (int i = 0; i < 2; i++)
					//	{
					//		SmallRock tempRock(renderer, -1000.0f, -1000.0f);

					//		SmallRockList.push_back(tempRock);

					//	}

					//	break;
					//}
				default:
					break;

				}






			}



		}

		//PLAYER MOVEMENT
		//get the keyboard state
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//check with arrows are pressed
		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			//if left button pressed set xDir to -1
			xDir = -1.0f;

		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			//right button pressed , set xDir to 1
			xDir = 1.0f;

		}
		else
		{
			xDir = 0.0f;
		}

		//check with arrows are pressed
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			//if left button pressed set xDir to -1
			yDir = -1.0f;

		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			//right button pressed , set xDir to 1
			yDir = 1.0f;

		}
		else
		{
			yDir = 0.0f;
		}

		//START UPDATE *******
		//player update if moving
		if (xDir != 0 || yDir != 0)
		{
			//get x and y
			x = playerPos.x = xDir;
			y = playerPos.y = yDir;

			//calculate angle
			playerAngle = atan2(yDir, xDir) * 180 / 3.14;

			//update old angle
			oldAngle = playerAngle;

			//update old direction
			xDirOld = xDir;
			yDirOld = yDir;

		}
		else
		{
			//update old angle
			oldAngle = playerAngle;
		}

		//get players new position
		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;

		//adjust for precision loss
		playerPos.x = (int)(pos_X + 0.5f);
		playerPos.y = (int)(pos_Y + 0.5f);

		//player wrapping
		//check to see if the player is off the screen
		if (playerPos.x < (0 - playerPos.w))
		{
			playerPos.x = 1024;
			pos_X = playerPos.x;
		}


		if (playerPos.x > 1024)
		{
			playerPos.x = (0 - playerPos.w);
			pos_X = playerPos.x;
		}

		if (playerPos.y < (0 - playerPos.w))
		{
			playerPos.y = 768;
			pos_Y = playerPos.y;
		}

		if (playerPos.y > 768)
		{
			playerPos.y = (0 - playerPos.w);
			pos_Y = playerPos.y;
		}




		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Update(deltaTime);
			}
		}

		for (int i = 0; i < LargeRockList.size(); i++)
		{
			if (LargeRockList[i].active == true)
			{
				LargeRockList[i].Update(deltaTime);
			}
		}

		for (int i = 0; i < SmallRockList.size(); i++)
		{
			if (SmallRockList[i].active == true)
			{
				SmallRockList[i].Update(deltaTime);
			}
		}

		//WRAPPING (COLLISION DETECTIONS)*******

		//check for collision of bullets with large rocks
		//for loop to scroll through all the players bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check to see if this bullet is active in the world
			if (bulletList[i].active == true)
			{
				//check all large rocks against active bullet
				for (int j = 0; j < LargeRockList.size(); j++)
				{

					if (SDL_HasIntersection(&bulletList[i].posRect, &LargeRockList[j].posRect))
					{
						//play explosion sound 
						Mix_PlayChannel(-1, explosion, 0);

						//LEVEL UP******
						//need two small rocks that are false as the number of small rocks grows
						int smallRockCounter = 0;

						//playerScore += 50;
						//string score = to_string(playerScore);
						//cout << "\tPlayer Score: " + score << endl;

						//create 2 small rocks
						for (int i = 0; i < SmallRockList.size(); i++)
						{
							if (SmallRockList[i].active == false)
							{
								SmallRockList[i].Reposition(LargeRockList[j].posRect.x, LargeRockList[j].posRect.y);

								//increase small rock counter
								smallRockCounter++;
							}

							//once we find 2 inactive rocks exit loop
							if (smallRockCounter == 2)
							{
								break;
							}

						}

						//reset the enemy
						LargeRockList[j].Deactivate();

						//reset the bullet
						bulletList[i].Deactivate();

						//LEVEL UP******
						totalRocksDestroyed++;

					}


				}
			}
		}

		//check for collision of bullets with small rocks
		//for loop to scroll through all players bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{

				//check all small rocks against active bullet
				//check all large rocks against active bullet
				for (int j = 0; j < SmallRockList.size(); j++)
				{

					if (SDL_HasIntersection(&bulletList[i].posRect, &SmallRockList[j].posRect))
					{
						//play explosion sound 
						Mix_PlayChannel(-1, explosion, 0);

						//playerScore += 100;
						//string score = to_string(playerScore);
					//	cout << "\tPlayer Score: " + score << endl;

						//reset the enemy
						SmallRockList[j].Deactivate();

						//reset the bullet
						bulletList[i].Deactivate();

						//level up*****

						//add one to total rocks destroyed
						totalRocksDestroyed++;

						//check to see if all rocks on the level destroyed
						if (totalRocksDestroyed >= totalNumberOfRocks)
						{
							//zero out total rocks Destroyed
							totalRocksDestroyed = 0;
							
							//change level
							//lvlCount++;
							//string currentLevel = to_string(lvlCount);
							

							//cout << "\tYou have now entered level " + currentLevel << endl;

							//add rocks to level
							numberOfLargeRocks++; //add one large rock
							numberOfSmallRocks += 2;//add 2 small rocks

							totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

							//clear rocks list
							LargeRockList.clear();
							SmallRockList.clear();

							//rebuild rock lists with new numbers
							for (int i = 0; i < numberOfLargeRocks; i++)
							{
								LargeRock tempRock(renderer, -1000.0f, -1000.0f);

								LargeRockList.push_back(tempRock);

							}

							for (int i = 0; i < numberOfSmallRocks; i++)
							{
								SmallRock tempRock(renderer, -1000.0f, -1000.0f);

								SmallRockList.push_back(tempRock);

							}

							for (int i = 0; i < numberOfLargeRocks; i++)
							{

								LargeRockList[i].Reposition();

							}

						}

					}


				}


			}

		}

		//WRAPPING (COLLISION DETECTION)********

		//END UPDATE*******







		//START DRAW ************

		//Draw section
		//clear the old buffer
		SDL_RenderClear(renderer);

		//draw bkgd
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

		//draw bullet
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Draw(renderer);
			}


		}

		for (int i = 0; i < LargeRockList.size(); i++)
		{
			if (LargeRockList[i].active == true)
			{
				LargeRockList[i].Draw(renderer);
			}
		}

		for (int i = 0; i < SmallRockList.size(); i++)
		{
			if (SmallRockList[i].active == true)
			{
				SmallRockList[i].Draw(renderer);
			}
		}

		SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);

		//draw new info onto the screen
		SDL_RenderPresent(renderer);

		//END DRAW ************

		
	}//ends game loop


		//close and destroy window
		SDL_DestroyWindow(window);

		//clean up
		SDL_Quit();

		return 0;
	
}