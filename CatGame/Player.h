#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	const int PLAYER_SIZE = 32;
	Player(class Game* game);
	const class CollisionComponent* GetCollisionComponent();
	void SetDeadTexture();
	void SetAnimation(const std::string& name);
	const std::string& GetAnimationName();

protected:
	class CollisionComponent* mCollisionComponent;
	class PlayerMove* mPlayerMove;
	class AnimatedSprite* mAnimatedSprite;
};
