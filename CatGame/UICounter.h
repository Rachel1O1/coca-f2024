#pragma once
#include "Math.h"
#include "Actor.h"

class UICounter : public Actor
{
public:
	UICounter(class Game* game, int lives);
};

class GameOver : public Actor
{
public:
	GameOver(class Game* game);
};
