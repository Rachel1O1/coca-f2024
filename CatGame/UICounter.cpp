#include "UICounter.h"
#include "Game.h"
#include "SpriteComponent.h"

UICounter::UICounter(Game* game, int lives)
: Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 102);
	sc->SetTexture(game->GetTexture("Assets/Counter/" + std::to_string(lives) + ".png"));
}

GameOver::GameOver(Game* game)
: Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 102);
	sc->SetTexture(game->GetTexture("Assets/GameOver.png"));
}