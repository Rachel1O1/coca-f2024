#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
: Component(owner, 50)
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	//update rotation
	float rotationV = mAngularSpeed;
	mOwner->SetRotation((rotationV * deltaTime) + (mOwner->GetRotation()));

	//update position
	Vector2 forwardV = (mOwner->GetForward() * mForwardSpeed);
	mOwner->SetPosition((forwardV * deltaTime) + (mOwner->GetPosition()));
}
