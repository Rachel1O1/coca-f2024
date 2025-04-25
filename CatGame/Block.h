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

	void onPlayerTouch() {
        if (!touched && !fade) {
            touched = true;
            timer = 0;
        }
    }
	void update() {
        if (touched && !fade) {
            timer ++;
            if (timer >= 100) {
                fade = true;
				touched = false;
				sc->SetTexture(mGame->GetTexture("Assets/BlockGone.png"));
            }
        }
    }

protected:
	CollisionComponent* mCollisionComponent;
};
