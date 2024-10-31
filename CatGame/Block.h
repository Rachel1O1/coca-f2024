#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	const int BLOCK_SIZE = 32;
	Block(class Game* game, char blockType);
	~Block();
	const class CollisionComponent* GetCollisionComponent();

protected:
	CollisionComponent* mCollisionComponent;
};
