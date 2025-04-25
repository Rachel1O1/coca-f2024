#pragma once
#include "Actor.h"
#include "SpriteComponent.h"

class Block : public Actor
{
public:
	const int BLOCK_SIZE = 32;
	bool fade = false;
	bool temp = false;
	bool touched = false;
	SpriteComponent* sc;
    int timer = 0;
	Game* gameTemp;
	Block(class Game* game, char blockType);
	~Block();
	const class CollisionComponent* GetCollisionComponent();

	void OnPlayerTouch();
	void UpdateFade();

protected:
	CollisionComponent* mCollisionComponent;
};
