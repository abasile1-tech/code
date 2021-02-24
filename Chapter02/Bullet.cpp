#include "Bullet.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

// the Bullet constructor initializes mRightSpeed and mDownSpeed to 0,
// and also creates an AnimSpriteComponent attached to the bullet, 
// with associated textures.
Bullet::Bullet(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
{
	// Create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Laser.png"),
	};
	asc->SetAnimTextures(anims);
}

// implements the bullet's movement
void Bullet::UpdateActor(float deltaTime)
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
void Bullet::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	// H key moves the bullet right
	if (state[SDL_SCANCODE_H])
	{
		mRightSpeed += 250.0f;
	}
	// F key moves the bullet left
	if (state[SDL_SCANCODE_F])
	{
		mRightSpeed -= 250.0f;
	}
	// up/down
	// G key moves the bullet down
	if (state[SDL_SCANCODE_G])
	{
		mDownSpeed += 300.0f;
	}
	// T key moves the bullet up
	if (state[SDL_SCANCODE_T])
	{
		mDownSpeed -= 300.0f;
	}
}
