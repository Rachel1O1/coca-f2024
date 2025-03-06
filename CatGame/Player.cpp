#include "Game.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "AnimatedSprite.h"

Player::Player(Game* game)
: Actor(game)
{
	AnimatedSprite* asc = new AnimatedSprite(this, 200);
	std::vector<SDL_Texture*> idle{GetGame()->GetTexture("Assets/Mario/Idle.png")};
	asc->AddAnimation("idle", idle);
	std::vector<SDL_Texture*> dead{GetGame()->GetTexture("Assets/Mario/Dead.png")};
	asc->AddAnimation("dead", dead);
	std::vector<SDL_Texture*> jumpLeft{GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};
	asc->AddAnimation("jumpLeft", jumpLeft);
	std::vector<SDL_Texture*> jumpRight{GetGame()->GetTexture("Assets/Mario/JumpRight.png")};
	asc->AddAnimation("jumpRight", jumpRight);
	std::vector<SDL_Texture*> runLeft{GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
									  GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
									  GetGame()->GetTexture("Assets/Mario/RunLeft2.png")};
	asc->AddAnimation("runLeft", runLeft);
	std::vector<SDL_Texture*> runRight{GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
									   GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
									   GetGame()->GetTexture("Assets/Mario/RunRight2.png")};
	asc->AddAnimation("runRight", runRight);
	mAnimatedSprite = asc;
	asc->SetAnimation("idle");

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(1.0f * PLAYER_SIZE, 1.0f * PLAYER_SIZE);
	mCollisionComponent = cc;

	PlayerMove* pm = new PlayerMove(this);
	mPlayerMove = pm;
}

const CollisionComponent* Player::GetCollisionComponent()
{
	return mCollisionComponent;
}

void Player::SetAnimation(const std::string& name)
{
	mAnimatedSprite->SetAnimation(name);
}

const std::string& Player::GetAnimationName()
{
	return mAnimatedSprite->GetAnimName();
}

void Player::PlayerDies()
{
	mPlayerMove->PlayerDies();
}