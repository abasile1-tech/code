// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddle1Dir(0)
,mPaddle2Dir(0)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// initialize the positions and velocities of several balls
	ball1.ball_position.x = 20;
	ball1.ball_position.y = 20;
	ball1.ball_velocity.x = 222;
	ball1.ball_velocity.y = 222;

	ball2.ball_position.x = 600;
	ball2.ball_position.y = 600;
	ball2.ball_velocity.x = 222;
	ball2.ball_velocity.y = -222;

	ball3.ball_position.x = 1024.0f/2.0f;
	ball3.ball_position.y = 768.0f / 2.0f;
	ball3.ball_velocity.x = -200.0f;
	ball3.ball_velocity.y = 235.0f;

	mBallVector.push_back(ball1);
	mBallVector.push_back(ball2);
	mBallVector.push_back(ball3);

	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = 768.0f/2.0f;
	mPaddle2Pos.x = 1024.0f - thickness - 10.0; 
	mPaddle2Pos.y = 768.0f / 2.0f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle 1 direction based on W/S keys
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}

	// Update paddle 2 direction based on I/K keys
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle 1 position based on direction
	if (mPaddle1Dir != 0)
	{
		mPaddle1Pos.y += mPaddle1Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle1Pos.y < (paddleH/2.0f + thickness))
		{
			mPaddle1Pos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddle1Pos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// Update paddle 2 position based on direction
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// support for multiple balls
	float diff = 0.0;
	for (size_t i = 0; i < mBallVector.size(); i++) {
		mBallVector[i].ball_position.x += mBallVector[i].ball_velocity.x * deltaTime;
		mBallVector[i].ball_position.y += mBallVector[i].ball_velocity.y * deltaTime;

		// Bounce if needed
		// Did we intersect with paddle 1?
		diff = mPaddle1Pos.y - mBallVector[i].ball_position.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			// Our y-difference is small enough
			diff <= paddleH / 2.0f &&
			// We are in the correct x-position
			mBallVector[i].ball_position.x <= 25.0f && mBallVector[i].ball_position.x >= 20.0f &&
			// The ball is moving to the left
			mBallVector[i].ball_velocity.x < 0.0f)
		{
			mBallVector[i].ball_velocity.x *= -1.0f;
		}

		// Did we intersect with paddle 2?
		diff = mPaddle2Pos.y - mBallVector[i].ball_position.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			// Our y-difference is small enough
			diff <= paddleH / 2.0f &&
			// We are in the correct x-position
			mBallVector[i].ball_position.x <= 1024.0f - 20.0f && mBallVector[i].ball_position.x >= 1024.0f - 25.0f &&
			// The ball is moving to the right
			mBallVector[i].ball_velocity.x > 0.0f)
		{
			mBallVector[i].ball_velocity.x *= -1.0f;
		}

		// Did the ball go off the screen? (if so, end game)
		else if (mBallVector[i].ball_position.x <= 0.0f || mBallVector[i].ball_position.x >= 1024.0f)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (mBallVector[i].ball_position.y <= thickness && mBallVector[i].ball_velocity.y < 0.0f)
		{
			mBallVector[i].ball_velocity.y *= -1;
		}

		// Did the ball collide with the bottom wall?
		else if (mBallVector[i].ball_position.y >= (768 - thickness) &&
			mBallVector[i].ball_velocity.y > 0.0f)
		{
			mBallVector[i].ball_velocity.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle 1
	SDL_Rect paddle1{
		static_cast<int>(mPaddle1Pos.x),
		static_cast<int>(mPaddle1Pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw paddle 2
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	for (size_t i = 0; i < mBallVector.size(); i++) {
		SDL_Rect ball_obj{
				static_cast<int>(mBallVector[i].ball_position.x - thickness / 2),
				static_cast<int>(mBallVector[i].ball_position.y - thickness / 2),
				thickness,
				thickness
		};
		SDL_RenderFillRect(mRenderer, &ball_obj);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
