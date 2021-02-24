#include "Enemy.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

// the Enemy constructor initializes mRightSpeed and mDownSpeed to 0,
// and also creates an AnimSpriteComponent attached to the ship, 
// with associated textures.
Enemy::Enemy(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
{
	// Create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Enemy01.png"),
		game->GetTexture("Assets/Enemy02.png"),
		game->GetTexture("Assets/Enemy03.png"),
		game->GetTexture("Assets/Enemy04.png"),
		game->GetTexture("Assets/Enemy05.png"),
		game->GetTexture("Assets/Enemy06.png"),
	};
	asc->SetAnimTextures(anims);
}

// implements the enemy's movement
void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// Restrict position to left half of screen
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

// updates mRightSpeed and mDownSpeed based on the keyboard input from T,F,G,H keys
void Enemy::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	// H key moves the ship right
	if (state[SDL_SCANCODE_H])
	{
		mRightSpeed += 250.0f;
	}
	// F key moves the ship left
	if (state[SDL_SCANCODE_F])
	{
		mRightSpeed -= 250.0f;
	}
	// up/down
	// G key moves the ship down
	if (state[SDL_SCANCODE_G])
	{
		mDownSpeed += 300.0f;
	}
	// T key moves the ship up
	if (state[SDL_SCANCODE_T])
	{
		mDownSpeed -= 300.0f;
	}
}