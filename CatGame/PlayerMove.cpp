#include "PlayerMove.h"
#include "SDL2/SDL_scancode.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "SpriteComponent.h"
#include <thread>
#include <vector>
#include "UICounter.h"

using namespace std;

vector<shared_ptr<thread>> disappear_blocks;

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
	mYSpeed = 0.0f;
	mPlayer = owner;
	mSpacePressed = false;
	mInAir = false;
	mLastSpacePressed = false;
	mGame = owner->GetGame();
	mPlayerCollisionComponent = owner->GetCollisionComponent();
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	float newSpeed = 0.0f;
	//process forwards/backwards movement
	if (keyState[SDL_SCANCODE_A])
	{
		newSpeed -= (1.0f);
	}
	if (keyState[SDL_SCANCODE_D])
	{
		newSpeed += (1.0f);
	}
	SetForwardSpeed(newSpeed * PLAYER_SPEED);

	mSpacePressed = false;
	if (keyState[SDL_SCANCODE_SPACE])
	{
		mSpacePressed = true;
	}
	if (mSpacePressed && !mLastSpacePressed && !mInAir)
	{
		mInAir = true;
		mYSpeed = (-1.0f * PLAYER_JUMP);
		mGame->PlaySoundOnce("Assets/Sounds/Jump.wav");
	}
	mLastSpacePressed = mSpacePressed;
}

void PlayerMove::Update(float deltaTime)
{
	Vector2 forwardV = (mOwner->GetForward() * mForwardSpeed);
	mOwner->SetPosition((forwardV * deltaTime) + (mOwner->GetPosition()));
	if (mOwner->GetPosition().x > FLAGPOLE_X)
	{
		mGame->TriggerReset();
		//mGame->StopBGMusic(true);
		//mPlayer->SetState(ActorState::Paused);
	}

	float newY = mOwner->GetPosition().y + (deltaTime * mYSpeed);
	if (newY > (1.0f * (mGame->WINDOW_HEIGHT + mPlayer->PLAYER_SIZE)))
	{
		//newY -= mGame->WINDOW_HEIGHT;
		//PlayerDies();
		mGame->TriggerReset();
	}
	mOwner->SetPosition(Vector2(mOwner->GetPosition().x, newY));

	size_t blocksTotal = mGame->GetBlocks().size();
	Vector2 offset = Vector2(0.0f, 0.0f);
	for (size_t i = 0; i < blocksTotal; i++)
	{
		CollSide result = mPlayerCollisionComponent->GetMinOverlap(
			mGame->GetBlocks()[i]->GetCollisionComponent(), offset);
		if (result != CollSide::None && !mGame->GetBlocks()[i]->fade)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			if (result == CollSide::Top)
			{
				if (mYSpeed > 0.0f)
				{
					mYSpeed = 0.0f;
				}
				mInAir = false;
				
				//added trigger touch();
				if(mGame->GetBlocks()[i]->temp){
					mGame->GetBlocks()[i]->OnPlayerTouch();
				}
			}
			else if ((result == CollSide::Bottom) && (mYSpeed < 0.0f))
			{
				mGame->PlaySoundOnce("Assets/Sounds/Bump.wav");
				mYSpeed = 0.0f;
			}
		}
		//update timer for disappearing blocks
		if(mGame->GetBlocks()[i]->touched){
			mGame->GetBlocks()[i]->UpdateFade();
		}
	}
	//

	mYSpeed += Y_ACCELERATION * deltaTime;

	float newCameraX = (mOwner->GetPosition().x - (mGame->WINDOW_WIDTH / 2));
	
	mGame->GetCameraPos().x = newCameraX;
	mGame->SetCounterPos(newCameraX );
	if (newCameraX < 0)
	{
		mGame->GetCameraPos().x = 0.0f;
		mGame->SetCounterPos(0.0f);
	}
	/*
	if (newCameraX > mGame->GetCameraPos().x)
	{
		mGame->GetCameraPos().x = newCameraX;
		if (newCameraX < 0)
		{
			mGame->GetCameraPos().x = 0.0f;
		}
	}
	*/


	if (mOwner->GetPosition().x < mGame->GetCameraPos().x)
	{
		mOwner->SetPosition(Vector2(mGame->GetCameraPos().x, mOwner->GetPosition().y));
	}

	UpdateAnimation();
}

void PlayerMove::UpdateAnimation()
{
	if (mPlayer->GetState() != ActorState::Paused)
	{
		bool movingLeft = (GetForwardSpeed() < 0);
		bool movingRight = (GetForwardSpeed() > 0);
		if (!mInAir && movingRight)
		{
			mPlayer->SetAnimation("runRight");
		}
		if (!mInAir && movingLeft)
		{
			mPlayer->SetAnimation("runLeft");
		}
		if (!mInAir && !movingRight && !movingLeft)
		{
			mPlayer->SetAnimation("idle");
		}
		if (mInAir && movingRight)
		{
			mPlayer->SetAnimation("jumpRight");
		}
		if (mInAir && movingLeft)
		{
			mPlayer->SetAnimation("jumpLeft");
		}
		const std::string& currentAnimationName = mPlayer->GetAnimationName();
		if ((mInAir && !movingRight && !movingLeft) &&
			((currentAnimationName.compare("runRight") == 0) ||
			 (currentAnimationName.compare("jumpRight") == 0) ||
			 (currentAnimationName.compare("idle") == 0)))
		{
			mPlayer->SetAnimation("jumpRight");
		}
		else if (mInAir && !movingRight && !movingLeft)
		{
			mPlayer->SetAnimation("jumpLeft");
		}
	}
}

void PlayerMove::PlayerDies()
{
	mPlayer->SetAnimation("dead");
	mPlayer->SetState(ActorState::Paused);
	//how to display gameover
	GameOver* gg = new GameOver(mGame);
	gg->SetPosition(Vector2(mGame->GetCameraPos().x + 300, mGame->GetCameraPos().y + 200));

	mGame->StopBGMusic(false);
}