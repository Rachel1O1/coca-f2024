#pragma once
#include "MoveComponent.h"

class PlayerMove : public MoveComponent
{
public:
	const int PLAYER_SPEED = 300;
	const int PLAYER_JUMP = 700;
	const int Y_ACCELERATION = 2000;
	const int FLAGPOLE_X = 6368;
	PlayerMove(class Player* owner);
	void PlayerDies();

protected:
	void UpdateAnimation();
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;
	class Player* mPlayer;
	float mYSpeed;
	bool mSpacePressed;
	bool mInAir;
	bool mLastSpacePressed;
	class Game* mGame;
	const class CollisionComponent* mPlayerCollisionComponent;
};
