#include "Game.h"
#include "Block.h"
#include "CollisionComponent.h"
#include <thread>
using namespace std;
#include <unistd.h>

Block::Block(Game* game, char blockType)
: Actor(game)
{
	gameTemp = game;
	sc = new SpriteComponent(this);
	switch (blockType)
	{
	case 'A':
		sc->SetTexture(game->GetTexture("Assets/BlockA.png"));
		break;
	case 'B':
		sc->SetTexture(game->GetTexture("Assets/BlockB.png"));
		break;
	case 'C':
		sc->SetTexture(game->GetTexture("Assets/BlockC.png"));
		break;
	case 'D':
		sc->SetTexture(game->GetTexture("Assets/BlockD.png"));
		break;
	case 'E':
		sc->SetTexture(game->GetTexture("Assets/BlockE.png"));
		break;
	case 'F':
		sc->SetTexture(game->GetTexture("Assets/BlockF.png"));
		break;
	case 'G':
		sc->SetTexture(game->GetTexture("Assets/BlockG.png"));
		break;
	case 'H':
		sc->SetTexture(game->GetTexture("Assets/BlockH.png"));
		break;
	case 'I':
		sc->SetTexture(game->GetTexture("Assets/BlockI.png"));
		break;
	case 'Y':
		sc->SetTexture(game->GetTexture("Assets/ForeverBlock.png"));
		break;
	}

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(1.0f * BLOCK_SIZE, 1.0f * BLOCK_SIZE);
	mCollisionComponent = cc;

	game->AddBlock(this);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}

const CollisionComponent* Block::GetCollisionComponent()
{
	return mCollisionComponent;
}

void Block::OnPlayerTouch() {
	if (!touched && !fade) {
		touched = true;
		timer = 0;
	}
}
void Block::UpdateFade() {
	if (touched && !fade) {
		timer ++;
		sc->SetTexture(mGame->GetTexture("Assets/BlockGone.png"));
		if (timer >= 100) {
			fade = true;
			touched = false;
			SetState(ActorState::Destroy);
		}
	}
}